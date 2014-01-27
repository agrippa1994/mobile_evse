#import "Client.h"

#include <string>
#include <sstream>

#include <boost/serialization/access.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>


#define serializer(archive) template<class T> void serialize(T & archive, const unsigned int)
#define serializer_non_intrusive(arch, cl) template<class T> void serialize(T & arch, cl, const unsigned int)

namespace boost {
    namespace serialization {
        
        serializer_non_intrusive(t, SystemInformation & info)
        {
            t & info.RaspberryPI;
        }
        
        serializer_non_intrusive(t, SystemInformation::stRaspberryPI & rpi)
        {
            t & rpi.Temperatures;
            t & rpi.Voltages;
            t & rpi.Frequencies;
        }
        
        serializer_non_intrusive(t, SystemInformation::stRaspberryPI::stTemperatures & temp)
        {
            t & temp.temperature;
        }
        
        serializer_non_intrusive(t, SystemInformation::stRaspberryPI::stVoltages & volt)
        {
            t & volt.core;
            t & volt.sdram_c;
            t & volt.sdram_i;
            t & volt.sdram_p;
        }
        
        serializer_non_intrusive(t, SystemInformation::stRaspberryPI::stFrequencies & f)
        {
            t & f.arm;
            t & f.core;
            t & f.h264;
            t & f.isp;
            t & f.v3d;
            t & f.uart;
            t & f.pwm;
            t & f.emmc;
            t & f.pixel;
            t & f.vec;
            t & f.hdmi;
            t & f.dpi;
        }
    }
}




struct MessageBase
{
    enum eMessage { Message_None, Message_StartLoading, Message_StopLoading, Message_SystemInformation };
    
    explicit MessageBase(eMessage msg) : _msg(msg) { }
    
    serializer(t)
    {
        t & _msg;
    }
    
    eMessage _msg;
};

@interface Client()
{
    NSMutableSet *delegates;
    
    NSMutableArray *messages;
    
    NSInputStream *inputStream;
    NSOutputStream *outputStream;
}

- (BOOL)sendStream:(const std::stringstream &)stream;
- (void)onData:(const uint8_t *)data length:(NSInteger)len;

@end


@implementation Client

+ (Client *)sharedClient
{
    static Client *cl = nil;
    if(cl == nil)
    {
        cl = [[Client alloc] init];
    }
    
    return cl;
}

- (id)init
{
    self = [super init];
    if(self)
    {
        delegates = [NSMutableSet set];
        messages = [NSMutableArray array];
    }
    
    return self;
}

- (void)dealloc
{
    [delegates removeAllObjects];
}

- (void)addDelegate:(id<ClientDelegate>)delegate
{
    [delegates addObject:delegate];
}

- (void)rmDelegate:(id<ClientDelegate>)delegate
{
    [delegates removeObject:delegate];
}

- (void)connect:(NSString *)host withPort:(UInt32)port
{
    CFReadStreamRef readStream;
    CFWriteStreamRef writeStream;
    
    CFStreamCreatePairWithSocketToHost(NULL, (__bridge CFStringRef)host, port, &readStream, &writeStream);
    
    inputStream = (__bridge NSInputStream *)readStream;
    outputStream = (__bridge NSOutputStream *)writeStream;
    
    [inputStream setDelegate:self];
    [outputStream setDelegate:self];
    
    [inputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    [outputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    
    [inputStream open];
    [outputStream open];
}

- (BOOL)disconnect
{
    return NO;
}

- (BOOL)startLoading:(NSInteger)amps
{
    try
    {
        struct stLoad : MessageBase
        {
            stLoad() : MessageBase(MessageBase::Message_StartLoading) { }
            
            serializer(t)
            {
                boost::serialization::base_object<MessageBase>(* this);
                t & amps;
            }
            
            // vars
            std::uint8_t amps;
            
        } load;
        
        load.amps = (std::uint8_t)amps;
        
        std::stringstream ss;
        boost::archive::text_oarchive ia(ss);
        
        ia & load;
        
        return [self sendStream:ss];
    }
    
    catch(...)
    {
        return NO;
    }
    
    return NO;
}

- (BOOL)stopLoading
{
    try
    {
        struct stStop : MessageBase
        {
            stStop() : MessageBase(MessageBase::Message_StopLoading) { }
            
            serializer(t)
            {
                boost::serialization::base_object<MessageBase>(* this);
                t & amps;
            }
            
            // vars
            std::uint8_t amps;
            
        } stop;
        
        std::stringstream ss;
        boost::archive::text_oarchive ia(ss);
        
        ia & stop;
        
        return [self sendStream:ss];
    }
    
    catch(...)
    {
        return NO;
    }
    
    return NO;
}


- (BOOL)sendStream:(const std::stringstream &)stream
{
    if([outputStream streamStatus] != NSStreamStatusOpen)
        return NO;
    
    const std::string & data = stream.str();
    if(data.length() <= 0)
        return NO;
    
    return [outputStream write:(const uint8_t *)data.c_str() maxLength:data.length()] > 0;
}

- (void)onData:(const uint8_t *)data length:(NSInteger)len
{
    std::stringstream ss(std::string((const char *)data, len));
    boost::archive::text_iarchive ia(ss);
    
    MessageBase msg(MessageBase::Message_None);
    
    ia & msg;
    
    switch(msg._msg)
    {
        case MessageBase::Message_StartLoading:
        {
            struct stLoad
            {
                serializer(t)
                {
                    t & success;
                }
                bool success;
            } load = { NO };
            
            ia & load;
            
            for(id<ClientDelegate> i in delegates)
            {
                [i client:self loadingReply:load.success];
            }
        }
        break;
            
        case MessageBase::Message_StopLoading:
        {
            struct stStop
            {
                serializer(t)
                {
                    t & success;
                }
                bool success;
            } load = { NO };
            
            ia & load;
            
            for(id<ClientDelegate> i in delegates)
            {
                [i client:self loadingReply:load.success];
            }
        }
        break;
            
        case MessageBase::Message_SystemInformation:
        {
            SystemInformation info = { 0 };
            
            ia & info;
            
            for(id<ClientDelegate> i in delegates)
            {
                [i client:self systemInformation:info];
            }
        }
        break;
            
        default:
        break;
    }
    
}

- (void)stream:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode
{
    switch(eventCode)
    {
        case NSStreamEventOpenCompleted:
            if(aStream == inputStream)
            {
                for(id<ClientDelegate> p in delegates) [p client:self openRequest:YES];
            }
            break;
        case NSStreamEventEndEncountered:
            NSLog(@"NSStreamEventEndEncountered");
            break;
        case NSStreamEventErrorOccurred:
            NSLog(@"NSStreamEventErrorOccured");
            break;
        case NSStreamEventHasBytesAvailable:
            if(aStream == inputStream)
            {
                uint8_t buffer[1024];
                while([inputStream hasBytesAvailable])
                {
                    NSInteger len = [inputStream read:buffer maxLength:sizeof(buffer)];
                    if(len > 0)
                    {
                        [self onData:buffer length:len];
                    }
                    else if(len == -1)
                    {
                        
                    }
                }
            }
            break;
    }
}

@end
