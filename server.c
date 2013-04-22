#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#define BUFFER 512
#define PROC_ID 0xff
#define MSG_FILE "server.c"

struct msg {
	long mtype;				// message type
	char msg[BUFFER+1];
};

int main() {
	struct msg m;
	char input[BUFFER];
	int pid = msgget(ftok(MSG_FILE, PROC_ID), IPC_CREAT|0666);	// 服务器创建一个消息队列
	int mid = shmget(ftok(MSG_FILE, PROC_ID), BUFFER, IPC_CREAT|0666);	// 服务器创建一个共享内存空间，大小为BUFFER字节
	if(mid < 0) {
		printf("Share Memery Create Error!\n");
		return 0;
	}
	if(pid < 0) {
		printf("Message Queue Create Error!\n");
		return 0;
	}
	char *ptr = (char *)shmat(mid, 0, 0);
	if(ptr == (char *)-1) {
		printf("Get Address Error!\n");
		return 0;
	}

	int id = fork();				// 创建一个子进程
	if(id==0) {
		execlp("./client", NULL, NULL);			// 在子进程中调用客户端进程
	} else {
		while(1) {
			printf("Yu:");
			gets(input);
			if(strcmp(input,"quit")==0) { // 输入quit时候退出程序,并向客户端发出消息提示退出
				m.mtype = 1;
				strcpy(m.msg, "quit");
				msgsnd(pid, &m, strlen(m.msg)+1, 0);
				break;
			}
			m.mtype = 1;
			strcpy(m.msg, "server");
			strcpy(ptr, input);
			msgsnd(pid, &m, strlen(m.msg)+1, 0);
			msgrcv(pid, &m, BUFFER+1, 2, 0);
			if(strcmp(m.msg, "client")==0) {
				printf("***************Client Reveived!***************\n");
			}
		}
		shmdt(ptr);			// 对共享内存操作结束，进行删除操作
	}
	printf("****************Server Quit**************\n");
	return 0;
}
