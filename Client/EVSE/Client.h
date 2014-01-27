#import <Foundation/Foundation.h>

struct SystemInformation
{
    struct stRaspberryPI
    {
        struct stTemperatures
        {
            float temperature;
        }
        Temperatures;
        
        struct stVoltages
        {
            float core, sdram_c, sdram_i, sdram_p;
        }
        Voltages;
        
        struct stFrequencies
        {
            uint64_t arm, core, h264, isp, v3d, uart, pwm, emmc, pixel, vec, hdmi, dpi;
        }
        Frequencies;
        
    }
    RaspberryPI;
};

@class Client;

@protocol ClientDelegate <NSObject>

- (void)client:(Client *)p openRequest:(BOOL)isOpen;
- (void)client:(Client *)p onDisconnect:(BOOL)disconnected;

- (void)client:(Client *)p loadingReply:(BOOL)wasLoadingStarted;
- (void)client:(Client *)p stopLoadingReply:(BOOL)wasLoadingStopped;

- (void)client:(Client *)p systemInformation:(const struct SystemInformation)info;

@end


@interface Client : NSObject<NSStreamDelegate>

+ (Client *)sharedClient;

- (void)addDelegate:(id<ClientDelegate>)delegate;
- (void)rmDelegate:(id<ClientDelegate>)delegate;

- (void)connect:(NSString *)host withPort:(UInt32)port;
- (BOOL)disconnect;

- (BOOL)startLoading:(NSInteger)amps;
- (BOOL)stopLoading;

@end


