#import "kcUtils.h"

#if TARGET_IPHONE_SIMULATOR
#include <sys/socket.h>
#include <sys/sysctl.h>
#include <net/if.h>
#include <net/if_dl.h>
#endif


@implementation kcUtils

+ (NSString *)getDocumentsDirectory {
    return [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
}

+ (NSString *)getDocumentsDirectoryWithFilename:(NSString *)filename {
    NSString *documentsDirectory = [kcUtils getDocumentsDirectory];
    NSString *localFilePath = [documentsDirectory stringByAppendingPathComponent:filename];
    return localFilePath;
}

+ (NSString *)getPathFromBundle:(NSString *)filename {
    NSString *bundlePath = [[NSBundle mainBundle] pathForResource:[filename stringByDeletingPathExtension] ofType:[filename pathExtension]];
    return bundlePath;
}

+ (NSString *)copyFromBundle:(NSString *)filename {
    NSString *localFilePath = [kcUtils getDocumentsDirectoryWithFilename:filename];
    if (![[NSFileManager defaultManager] fileExistsAtPath:localFilePath isDirectory:nil]) {
        NSString *bundlePath = [[NSBundle mainBundle] pathForResource:[filename stringByDeletingPathExtension] ofType:[filename pathExtension]];
        NSError *error = nil;
        [[NSFileManager defaultManager] copyItemAtPath:bundlePath toPath:localFilePath error:&error];
        if (error != nil) {
            return nil;
        }
    }
    
    return localFilePath;
}

+ (NSDate *)fileTimestamp:(NSString *)fullFilename {
    if ([[NSFileManager defaultManager] fileExistsAtPath:fullFilename isDirectory:nil]) {
        NSError *error = nil;
        NSDictionary *attr = [[NSFileManager defaultManager] attributesOfItemAtPath:fullFilename error:&error];
        if (attr) {
            return [attr objectForKey:NSFileModificationDate];
        }
    }
    return nil;
}

#if TARGET_IPHONE_SIMULATOR
+ (NSString *)getMacAddress {
    int                 mgmtInfoBase[6];
    char                *msgBuffer;
    size_t              length;
    unsigned char       macAddress[6];
    struct if_msghdr    *interfaceMsgStruct;
    struct sockaddr_dl  *socketStruct;
    NSString            *errorFlag = NULL;
    
    // Setup the management Information Base (mib)
    mgmtInfoBase[0] = CTL_NET;        // Request network subsystem
    mgmtInfoBase[1] = AF_ROUTE;       // Routing table info
    mgmtInfoBase[2] = 0;
    mgmtInfoBase[3] = AF_LINK;        // Request link layer information
    mgmtInfoBase[4] = NET_RT_IFLIST;  // Request all configured interfaces
    
    // With all configured interfaces requested, get handle index
    if ((mgmtInfoBase[5] = if_nametoindex("en0")) == 0) {
        errorFlag = @"if_nametoindex failure";
    } else {
        // Get the size of the data available (store in len)
        if (sysctl(mgmtInfoBase, 6, NULL, &length, NULL, 0) < 0) {
            errorFlag = @"sysctl mgmtInfoBase failure";
        } else {
            // Alloc memory based on above call
            if ((msgBuffer = (char *)malloc(length)) == NULL) {
                errorFlag = @"buffer allocation failure";
            } else {
                // Get system information, store in buffer
                if (sysctl(mgmtInfoBase, 6, msgBuffer, &length, NULL, 0) < 0) {
                    errorFlag = @"sysctl msgBuffer failure";
                }
            }
        }
    }
    
    // Befor going any further...
    if (errorFlag != NULL) {
        NSDebugLog(@"Error: %@", errorFlag);
        return errorFlag;
    }
    
    // Map msgbuffer to interface message structure
    interfaceMsgStruct = (struct if_msghdr *) msgBuffer;
    
    // Map to link-level socket structure
    socketStruct = (struct sockaddr_dl *) (interfaceMsgStruct + 1);
    
    // Copy link layer address data in socket structure to an array
    memcpy(&macAddress, socketStruct->sdl_data + socketStruct->sdl_nlen, 6);
    
    // Read from char array into a string object, into traditional Mac address format
    NSString *macAddressString = [NSString stringWithFormat:@"%02X:%02X:%02X:%02X:%02X:%02X",
                                  macAddress[0], macAddress[1], macAddress[2],
                                  macAddress[3], macAddress[4], macAddress[5]];
    NSDebugLog(@"Mac Address: %@", macAddressString);
    
    // Release the buffer memory
    free(msgBuffer);
    
    return macAddressString;
}
#endif

@end
