#import <Foundation/Foundation.h>
#include <asl.h>
#include <fcntl.h>
#include <syslog.h>

// Tricky Preprocessor macros to warn when using NSLog,
#define __SHORT_FILE__              strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__
#define DO_PRAGMA(x)                _Pragma (#x)
//#define NSLog(...)                  DO_PRAGMA(message ("Are you sure you mean NSLog?  Try LogWarning/LogNotice/LogDebug instead."))

const char *InitLoggingClient(const char *logClientName);
aslclient GetAslClient();

// See asl.h (ASL_LEVEL_DEBUG, etc.)
void SetLoggingLevel(int level);

// Creates all the NSString and char* based logging C-functions and their conditional variants
#define __MAKE_LOG_FUNCTION_PROTO(NAME) \
    OBJC_EXPORT void NAME(NSString *format, ...); \
    OBJC_EXPORT void NAME ## If(bool test, NSString *format, ...); \
    OBJC_EXPORT void NAME ## C(const char *format, ...);\
    OBJC_EXPORT void NAME ## CIf(bool test, const char *format, ...);
__MAKE_LOG_FUNCTION_PROTO(LogEmergency)     // Always shown in console/system log
__MAKE_LOG_FUNCTION_PROTO(LogAlert)         // Always shown in console/system log
__MAKE_LOG_FUNCTION_PROTO(LogCritical)      // Always shown in console/system log
__MAKE_LOG_FUNCTION_PROTO(LogError)         // Always shown in console/system log
__MAKE_LOG_FUNCTION_PROTO(LogWarning)       // Always shown in console/system log
__MAKE_LOG_FUNCTION_PROTO(LogNotice)        // Only shown in console/system log on debug builds, but can be shown via API call
__MAKE_LOG_FUNCTION_PROTO(LogInfo)          // Use for debugging only, won't ever be in the console/system log, only in the debugging log
__MAKE_LOG_FUNCTION_PROTO(LogDebug)         // Use for debugging only, won't ever be in the console/system log, only in the debugging log
#undef __MAKE_LOG_FUNCTION_PROTO

#ifndef COMPILE_TIME_LOG_LEVEL
#ifdef DEBUG
#define COMPILE_TIME_LOG_LEVEL ASL_LEVEL_NOTICE
#else
#define COMPILE_TIME_LOG_LEVEL ASL_LEVEL_WARNING
#endif
#endif


@interface kcUtils : NSObject

+ (NSString *)getDocumentsDirectory;
+ (NSString *)getDocumentsDirectoryWithFilename:(NSString *)filename;
+ (NSString *)getPathFromBundle:(NSString *)filename;
+ (NSString *)copyFromBundle:(NSString *)filename;
+ (NSDate *)fileTimestamp:(NSString *)fullFilename;
+ (unsigned long long)currentMs;

#if TARGET_IPHONE_SIMULATOR
+ (NSString *)getMacAddress;
#endif

@end
