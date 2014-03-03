#import <Foundation/Foundation.h>

@class Client;

@protocol ClientDelegate <NSObject>
@required
- (void)client:(Client *)p openRequest:(BOOL)isOpen;
- (void)client:(Client *)p onDisconnect:(BOOL)disconnected;

- (void)client:(Client *)p onData:(const uint8_t *)data length:(NSInteger)len;
- (void)client:(Client *)p onKeyAndValue:(NSString *)key value:(NSString *)val;
@end


@interface Client : NSObject<NSStreamDelegate>

+ (Client *)sharedClient;

- (void)addDelegate:(id<ClientDelegate>)delegate;
- (void)rmDelegate:(id<ClientDelegate>)delegate;

- (void)connect:(NSString *)host withPort:(UInt32)port;
- (BOOL)disconnect;

@end


