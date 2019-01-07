/*
 * File        : daemon_api21.c
 * Author      : Mars Kwok
 * Date        : Jul. 21, 2015
 * Description : This is used to watch process dead under api 20
 *
 * Copyright (C) Mars Kwok<Marswin89@gmail.com>
 *
 */
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

#include "constant.h"
#include "PolyLink_SDK_API20.h"
#include "ctype.h"
#include "stdlib.h"
#include "native-lib.h"
#include <android/log.h>
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
/**
 *  get the process pid by process name
 */
int find_pid_by_name(char *pid_name, int *pid_list){
    DIR *dir;
	struct dirent *next;
	int i = 0;
	pid_list[0] = 0;
	dir = opendir("/proc");
	if (!dir){
		return 0;
	}
	while ((next = readdir(dir)) != NULL){
		FILE *status;
		char proc_file_name[BUFFER_SIZE];
		char buffer[BUFFER_SIZE];
		char process_name[BUFFER_SIZE];

		if (strcmp(next->d_name, "..") == 0){
			continue;
		}
		if (!isdigit(*next->d_name)){
			continue;
		}
		sprintf(proc_file_name, "/proc/%s/cmdline", next->d_name);
		if (!(status = fopen(proc_file_name, "r"))){
			continue;
		}
		if (fgets(buffer, BUFFER_SIZE - 1, status) == NULL){
			fclose(status);
			continue;
		}
		fclose(status);
		sscanf(buffer, "%[^-]", process_name);
		if (strcmp(process_name, pid_name) == 0){
			pid_list[i ++] = atoi(next->d_name);
		}
	}
	if (pid_list){
    	pid_list[i] = 0;
    }
    closedir(dir);
    return i;
}

/**
 *  kill all process by name
 */
void kill_zombie_process(char* zombie_name){
    int pid_list[200];
    int total_num = find_pid_by_name(zombie_name, pid_list);
	LOGE("zombie process name is %s, and number is %d, killing...", zombie_name, total_num);
    int i;
    for (i = 0; i < total_num; i ++)    {
        int retval = 0;
        int daemon_pid = pid_list[i];
        if (daemon_pid > 1 && daemon_pid != getpid() && daemon_pid != getppid()){
            retval = kill(daemon_pid, SIGTERM);
            if (!retval){
				LOGE("kill zombie successfully, zombie`s pid = %d", daemon_pid);
            }else{
                LOGE("kill zombie failed, zombie`s pid = %d", daemon_pid);
            }
        }
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

JNIEXPORT void JNICALL Java_polylink_sdk_pl_c_nativ_PolyLinkAPI20_dos(JNIEnv *env, jobject jobj, jstring pkgName, jstring svcName, jstring daemonPath){
	if(pkgName == NULL || svcName == NULL || daemonPath == NULL){
		LOGE("native doDaemon parameters cannot be NULL !");
		return ;
	}

	char *pkg_name = (char*)env->GetStringUTFChars(pkgName, 0);
	char *svc_name = (char*)env->GetStringUTFChars( svcName, 0);
	char *daemon_path = (char*)env->GetStringUTFChars( daemonPath, 0);

	kill_zombie_process(NATIVE_DAEMON_NAME);

	int pipe_fd1[2];//order to watch child
	int pipe_fd2[2];//order to watch parent

	pid_t pid;
	char r_buf[100];
	int r_num;
	memset(r_buf, 0, sizeof(r_buf));
	if(pipe(pipe_fd1)<0){
		LOGE("pipe1 create error");
		return ;
	}
	if(pipe(pipe_fd2)<0){
		LOGE("pipe2 create error");
		return ;
	}

	char str_p1r[10];
	char str_p1w[10];
	char str_p2r[10];
	char str_p2w[10];

	sprintf(str_p1r,"%d",pipe_fd1[0]);
	sprintf(str_p1w,"%d",pipe_fd1[1]);
	sprintf(str_p2r,"%d",pipe_fd2[0]);
	sprintf(str_p2w,"%d",pipe_fd2[1]);


	if((pid=fork())==0){
		execlp(daemon_path,
				NATIVE_DAEMON_NAME,
				PARAM_PKG_NAME, pkg_name,
				PARAM_SVC_NAME, svc_name,
				PARAM_PIPE_1_READ, str_p1r,
				PARAM_PIPE_1_WRITE, str_p1w,
				PARAM_PIPE_2_READ, str_p2r,
				PARAM_PIPE_2_WRITE, str_p2w,
				(char *) NULL);
	}else if(pid>0){
		close(pipe_fd1[1]);
		close(pipe_fd2[0]);
		//wait for child
		r_num=read(pipe_fd1[0], r_buf, 100);
		LOGE("Watch >>>>CHILD<<<< Dead !!!");
		java_callback(env, jobj, DAEMON_CALLBACK_NAME);
	}
}

