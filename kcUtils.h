#import <Foundation/Foundation.h>

// Tricky Preprocessor macros to warn when using NSLog,
// allow NSDebugLog() and NSLogAlways() with conditional variants.
// allow NSDebugLog() and NSLogAlways() with conditional variants.
//#define __SHORT_FILE__              strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__
//#define DO_PRAGMA(x)                _Pragma (#x)
//#define NSLog(...)                  DO_PRAGMA(message ("Are you sure you mean NSLog?  Try NSDebugLog or NSLogAlways instead."))
//#define debug(format, ...)          CFShow((__bridge void *)[NSString stringWithFormat:@"[%s:%d] " format, __SHORT_FILE__, __LINE__, ##__VA_ARGS__]);
#define debug(format, ...)          NSLog(format, ##__VA_ARGS__)

#ifdef DEBUG
#define DEBUG_INTERNAL 1
#else
#define DEBUG_INTERNAL 1
#endif

#ifdef DEBUG_INTERNAL
#define NSDebugLog(...)             debug(__VA_ARGS__)
#define NSDebugLogIf(test, ...)     if (test) { debug(__VA_ARGS__); }
#else
#define NSDebugLog(...)
#define NSDebugLogIf(test, ...)
#endif

#define NSLogAlways(...)            debug(__VA_ARGS__)
#define NSLogAlwaysIf(test, ...)    if (test) { debug(__VA_ARGS__); }

@interface kcUtils : NSObject

+ (NSString *)getDocumentsDirectory;
+ (NSString *)getDocumentsDirectoryWithFilename:(NSString *)filename;
+ (NSString *)getPathFromBundle:(NSString *)filename;
+ (NSString *)copyFromBundle:(NSString *)filename;
+ (NSDate *)fileTimestamp:(NSString *)fullFilename;

#if TARGET_IPHONE_SIMULATOR
+ (NSString *)getMacAddress;
#endif

@end
