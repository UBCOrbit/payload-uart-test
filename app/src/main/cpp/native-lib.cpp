#include <jni.h>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include <android/log.h>

#define LOG_TAG "serial fd call"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// File descriptor for the serial device
// Yes I know this is bad practice but whatevs
int serialFD = 0;

extern "C"
JNIEXPORT jboolean JNICALL
Java_us_laelath_inforceserialio_MainActivity_openSerialDevice(
        JNIEnv *env,
        jobject /* this */) {
    // Open the file descriptor
    serialFD = open("/dev/ttyHSL0", O_RDWR | O_NOCTTY | O_NONBLOCK, 0);
    if (serialFD == -1) {
        LOGE("/dev/ttyHSL0 failed to open with error: %s", strerror(errno));
        return (jboolean) false;
    } else {
        LOGI("Successfully opened /dev/ttyHSL0");
        return (jboolean) true;
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_us_laelath_inforceserialio_MainActivity_closeSerialDevice(
        JNIEnv *env,
        jobject /* this */) {
    // Close the file descriptor
    int err = close(serialFD);
    if (err == -1) {
        LOGE("/dev/ttyHSL0 failed to close with error: %s", strerror(errno));
        return (jboolean) false;
    } else {
        LOGI("Successfully closed /dev/ttyHSL0");
        return (jboolean) true;
    }
}
