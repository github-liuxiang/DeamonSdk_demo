
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include<sys/file.h>
#include "PolyLink_SDK_API21.h"
#include "constant.h"
#include "native-lib.h"
#include "jni.h"
#include <android/log.h>
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

/**
 * call java callback
 */
void java_callback(JNIEnv *env, jobject jobj, char* method_name);

void waitfor_self_observer(char* observer_file_path){
	int lockFileDescriptor = open(observer_file_path, O_RDONLY);
	if (lockFileDescriptor == -1){
		LOGE("Watched >>>>OBSERVER<<<< has been ready before watching...");
		return ;
	}

	void *p_buf = malloc(sizeof(struct inotify_event));
	if (p_buf == NULL){
		LOGE("malloc failed !!!");
		return;
	}
	int maskStrLength = 7 + 10 + 1;
	char *p_maskStr = (char *) malloc(maskStrLength);
	if (p_maskStr == NULL){
		free(p_buf);
		LOGE("malloc failed !!!");
		return;
	}
	int fileDescriptor = inotify_init();
	if (fileDescriptor < 0){
		free(p_buf);
		free(p_maskStr);
		LOGE("inotify_init failed !!!");
		return;
	}

	int watchDescriptor = inotify_add_watch(fileDescriptor, observer_file_path, IN_ALL_EVENTS);
	if (watchDescriptor < 0){
		free(p_buf);
		free(p_maskStr);
		LOGE("inotify_add_watch failed !!!");
		return;
	}


	while(1){
		size_t readBytes = read(fileDescriptor, p_buf, sizeof(struct inotify_event));
		if (4 == ((struct inotify_event *) p_buf)->mask){
			LOGE("Watched >>>>OBSERVER<<<< has been ready...");
			free(p_maskStr);
			free(p_buf);
			return;
		}
	}
}

void notify_daemon_observer(unsigned char is_persistent, char* observer_file_path){
	if(!is_persistent){
		int lockFileDescriptor = open(observer_file_path, O_RDONLY);
		while(lockFileDescriptor == -1){
			lockFileDescriptor = open(observer_file_path, O_RDONLY);
		}
	}
	remove(observer_file_path);
}

void notify_and_waitfor(char *observer_self_path, char *observer_daemon_path){
    LOGE("Watched >>>>OBSERVER<<<< has been ready...1");
	int observer_self_descriptor = open(observer_self_path, O_RDONLY);
    LOGE("Watched >>>>OBSERVER<<<< has been ready...2");
	if (observer_self_descriptor == -1){
		observer_self_descriptor = open(observer_self_path, O_CREAT, S_IRUSR | S_IWUSR);
	}
    LOGE("Watched >>>>OBSERVER<<<< has been ready...3");
	int observer_daemon_descriptor = open(observer_daemon_path, O_RDONLY);
    LOGE("Watched >>>>OBSERVER<<<< has been ready...4");
	while (observer_daemon_descriptor == -1){
//        LOGE("Watched >>>>OBSERVER<<<< has been ready...4.1",observer_daemon_descriptor);
		usleep(1000);
//        LOGE("Watched >>>>OBSERVER<<<< has been ready...4.2");
		observer_daemon_descriptor = open(observer_daemon_path, O_RDONLY);
//        LOGE("Watched >>>>OBSERVER<<<< has been ready...4.3");
	}
    LOGE("Watched >>>>OBSERVER<<<< has been ready...5");
	remove(observer_daemon_path);
	LOGE("Watched >>>>OBSERVER<<<< has been ready...");
}


/**
 *  Lock the file, this is block method.
 */
int lock_file(char* lock_file_path){
    LOGE("start try to lock file >> %s <<", lock_file_path);
    int lockFileDescriptor = open(lock_file_path, O_RDONLY);
    if (lockFileDescriptor == -1){
        lockFileDescriptor = open(lock_file_path, O_CREAT, S_IRUSR);
    }
	int lockRet = flock(lockFileDescriptor, (int)LOCK_EX);
    if (lockRet == -1){
        LOGE("lock file failed >> %s <<", lock_file_path);
        return 0;
    }else{
		LOGE("lock file success  >> %s <<", lock_file_path);
        return 1;
    }
}


JNIEXPORT void JNICALL Java_polylink_sdk_pl_c_nativ_PolyLinkAPI21_dos(JNIEnv *env, jobject jobj, jstring indicatorSelfPath, jstring indicatorDaemonPath, jstring observerSelfPath, jstring observerDaemonPath){
	if(indicatorSelfPath == NULL || indicatorDaemonPath == NULL || observerSelfPath == NULL || observerDaemonPath == NULL){
		LOGE("parameters cannot be NULL !");
		return ;
	}
	LOGE("Persistent");
    env->GetStringUTFChars(indicatorSelfPath, 0);
	char* indicator_self_path = (char*)env->GetStringUTFChars( indicatorSelfPath, 0);
	char* indicator_daemon_path = (char*)env->GetStringUTFChars( indicatorDaemonPath, 0);
	char* observer_self_path = (char*)env->GetStringUTFChars( observerSelfPath, 0);
	char* observer_daemon_path = (char*)env->GetStringUTFChars(observerDaemonPath, 0);

	int lock_status = 0;
	int try_time = 0;
	while(try_time < 3 && !(lock_status = lock_file(indicator_self_path))){
		try_time++;
		LOGE("Persistent lock myself failed and try again as %d times", try_time);
		usleep(10000);
	}
	if(!lock_status){
		LOGE("Persistent lock myself failed and exit");
		return ;
	}

//	notify_daemon_observer(observer_daemon_path);
//	waitfor_self_observer(observer_self_path);
	notify_and_waitfor(observer_self_path, observer_daemon_path);

	lock_status = lock_file(indicator_daemon_path);
	if(lock_status){
		LOGE("Watch >>>>DAEMON<<<<< Daed !!");
		remove(observer_self_path);// it`s important ! to prevent from deadlock
        java_callback(env, jobj, DAEMON_CALLBACK_NAME);
	}

}
/**
 * call java callback
 */
void java_callback(JNIEnv *env, jobject jobj, char* method_name){
    jclass cls = env->GetObjectClass( jobj);
    jmethodID cb_method = env->GetMethodID( cls, method_name, "()V");
    env->CallVoidMethod( jobj, cb_method);
}