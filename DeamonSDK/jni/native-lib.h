#ifndef SOCKETPROCESS2_NATIVE_LIB_H
#define SOCKETPROCESS2_NATIVE_LIB_H

#endif  //SOCKETPROCESS2_NATIVE_LIB_H

#include <sys/select.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
#include <android/log.h>
#include <sys/types.h>
#include <sys/un.h>
#include <errno.h>
#include <stdlib.h>
#include <linux/signal.h>
#include <jni.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#define LOG_TAG "PolyLink_sdk_c"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
void child_do_work();
int child_create_channel();
void child_listen_msg();
