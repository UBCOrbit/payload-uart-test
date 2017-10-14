#include <jni.h>
#include <string>
#include <iostream>

#include <fcntl.h>
#include <unistd.h>

#include <android/log.h>

#define LOG_TAG "UART fd call"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

// File descriptor for the serial device
// Yes I know this is bad practice but whatevs
int fd = 0;

bool openfd() {
    fd = open("/dev/ttyHSL0", O_RDWR | O_NOCTTY, 0);
    if (fd == -1) {
        LOGE("Error opening /dev/ttyHSL0: %s", strerror(errno));
        return false;
    } else {
        LOGD("Successfully opened /dev/ttyHSL0");
        return true;
    }
}

void closefd() {
    if (close(fd) == -1)
        LOGE("Error closing /dev/ttyHSL0: %s", strerror(errno));
    else
        LOGD("Successfully closed /dev/ttyHSL0");
}

std::string readfd() {
    char buf[256];
    ssize_t len;

    len = read(fd, buf, 256);
    if (len == -1) {
        LOGE("Error reading from /dev/ttyHSL0: %s", strerror(errno));
        return "";
    } else if (len == 0) {
        LOGD("Received EOF from /dev/ttyHSL0");
        return "";
    }
    return std::string(buf, (unsigned long) len);
}

extern "C"
JNIEXPORT void JNICALL
Java_us_laelath_inforceserialio_MainActivity_runReadTest(
        JNIEnv *env,
        jobject /* this */) {
    if(!openfd())
        return;

    std::string msg = "";

    do {
        msg = readfd();
        if (msg != "") {
            std::cout << msg;
        }
    } while (msg != "");

    closefd();
}
