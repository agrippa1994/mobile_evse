#import "Client.h"

#include <string>
#include <sstream>

@interface Client()
{
    NSMutableSet *delegates;
    
    NSMutableArray *messages;
    
    NSInputStream *inputStream;
    NSOutputStream *outputStream;
    
    BOOL wasOpenRequestSend;
}

- (void)onData:(const uint8_t *)data length:(NSInteger)len;

- (void)inputStreamEvent:(NSStreamEvent)eventCode;
- (void)outputStreamEvent:(NSStreamEvent)eventCode;

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
        wasOpenRequestSend = NO;
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
    
    wasOpenRequestSend = YES;
    
    [inputStream open];
    [outputStream open];
}

- (BOOL)disconnect
{
    return NO;
}

- (void)onData:(const uint8_t *)data length:(NSInteger)len
{
    for(id<ClientDelegate> i in delegates)
        [i client:self onData:data length:len];
}

- (void)inputStreamEvent:(NSStreamEvent)eventCode
{
    NSLog(@"inputStreamEvent:%lu", (unsigned long)eventCode);
    
    switch(eventCode)
    {
        case NSStreamEventOpenCompleted:
            for(id<ClientDelegate> p in delegates) [p client:self openRequest:YES];
            break;
        case NSStreamEventEndEncountered:
            break;
        case NSStreamEventErrorOccurred:
            if(wasOpenRequestSend)
            {
                for(id<ClientDelegate> p in delegates) [p client:self openRequest:NO];
            }
            break;
        case NSStreamEventHasBytesAvailable:
 
            uint8_t buffer[2048] = { 0 };
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
            break;
    }

}

- (void)outputStreamEvent:(NSStreamEvent)eventCode
{
    NSLog(@"outputStreamEvent:%lu", (unsigned long)eventCode);
    
    switch(eventCode)
    {
        case NSStreamEventOpenCompleted:
            break;
        case NSStreamEventEndEncountered:
            break;
        case NSStreamEventErrorOccurred:
            break;
        case NSStreamEventHasBytesAvailable:
            break;
    }
}

- (void)stream:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode
{
    if(aStream == inputStream)
        [self inputStreamEvent:eventCode];
    else if(aStream == outputStream)
        [self outputStreamEvent:eventCode];
}

@end
