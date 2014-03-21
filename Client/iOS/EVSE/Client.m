#import "Client.h"

#include <string.h>

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

@end


@implementation Client
@synthesize keyAndValues = keysVals;

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
        keysVals = [NSMapTable mapTableWithKeyOptions:NSPointerFunctionsStrongMemory valueOptions:NSPointerFunctionsStrongMemory];
        
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
    if(![self isConnected])
        return NO;
    
    // Entfernen aller Schlüssel und Werte
    [keysVals removeAllObjects];
    
    [inputStream close];
    [outputStream close];
    
    return NO;
}

- (BOOL)isConnected
{
    NSStreamStatus iStatus = [inputStream streamStatus];
    NSStreamStatus oStatus = [outputStream streamStatus];
    
    return  (BOOL)(((iStatus == NSStreamStatusOpen) || (iStatus == NSStreamStatusWriting) || (iStatus == NSStreamStatusReading))
             &&((oStatus == NSStreamStatusOpen) || (oStatus == NSStreamStatusWriting) || (oStatus == NSStreamStatusReading)));
}

- (BOOL)send:(NSString *)data
{
    if(![self isConnected])
        return NO;
    
    const uint8_t * raw = (const uint8_t *)[[data stringByAppendingString:@"\r\n"] UTF8String];
    const size_t len = strlen((const char *) raw);
    return [outputStream write:raw maxLength:len] > 0;
}

- (void)onData:(const uint8_t *)data length:(NSInteger)len
{
    // Daten-Delegation
    for(id<ClientDelegate> i in delegates)
        if([i respondsToSelector:@selector(client:onData:length:)])
            [i client:self onData:data length:len];
    
    // Zerlegen des Strings
    NSArray *tokens = [[NSString stringWithCString:(const char *)data encoding:NSASCIIStringEncoding] componentsSeparatedByString:@" "];
    for(NSString *tok in tokens)
    {
        NSArray *vals = [tok componentsSeparatedByString:@":"];
        if([vals count] != 2)
            continue;
        
        NSString *key = [vals firstObject];
        NSString *val = [vals lastObject];
        
        // Setzen des Wertes in der Map
        [keysVals setObject:val forKey:key];
    }
    
    // Aufrufen der Delegation
    NSEnumerator *it = [keysVals keyEnumerator];
    id pair;
    while((pair = [it nextObject]))
        for(id<ClientDelegate> i in delegates)
            if([i respondsToSelector:@selector(client:onKeyAndValue:value:)])
                [i client:self onKeyAndValue:pair value:[keysVals objectForKey:pair]];
    
}

- (void)inputStreamEvent:(NSStreamEvent)eventCode
{
    NSLog(@"inputStreamEvent:%lu", (unsigned long)eventCode);
    
    switch(eventCode)
    {
        case NSStreamEventOpenCompleted:
        {
            for(id<ClientDelegate> p in delegates)
                if([p respondsToSelector:@selector(client:openRequest:)])
                    [p client:self openRequest:YES];
            
            wasOpenRequestSend = NO;
            break;
        }
            
        case NSStreamEventEndEncountered:
        {
            for(id<ClientDelegate> p in delegates)
                if([p respondsToSelector:@selector(client:onDisconnect:)])
                    [p client:self onDisconnect:YES];
            break;
        }
            
        case NSStreamEventErrorOccurred:
        {
            if(wasOpenRequestSend)
            {
                for(id<ClientDelegate> p in delegates)
                    if([p respondsToSelector:@selector(client:openRequest:)])
                        [p client:self openRequest:NO];
            }
            break;
        }
            
        case NSStreamEventHasBytesAvailable:
        {
            uint8_t buffer[2048] = { 0 };
            while([inputStream hasBytesAvailable])
            {
                NSInteger len = [inputStream read:buffer maxLength:sizeof(buffer)];
                if(len <= 0)
                {
                    for(id<ClientDelegate> p in delegates)
                        if([p respondsToSelector:@selector(client:onDisconnect:)])
                            [p client:self onDisconnect:YES];
                }
                else
                {
                    [self onData:buffer length:len];
                }
            }
            break;
        }
            
        default:
            break;
    }
    
}

- (void)stream:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode
{
    if(aStream == inputStream)
        [self inputStreamEvent:eventCode];
}

@end
