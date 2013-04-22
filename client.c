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
	int pid = msgget(ftok(MSG_FILE, PROC_ID), 0);
	int mid = shmget(ftok(MSG_FILE, PROC_ID), BUFFER,0);
	if(mid < 0) {
		printf("Share Memery Get Error!\n");
		return 0;
	}
	if(pid < 0) {
		printf("Message Queue Get Error!\n");
		return 0;
	}
	char *ptr = (char *)shmat(mid, 0, 0);
	while(1) {
		msgrcv(pid, &m, BUFFER+1, 1, 0);
		if(strcmp(m.msg, "server") == 0) {
			printf("Client Read Message From Share Memory:");
			puts(ptr);
			m.mtype = 2;
			strcpy(m.msg, "client");
			msgsnd(pid, &m, strlen(m.msg)+1, 0);
		}/* else if(strcmp(m.msg, "quit") == 0) {
			shmdt(ptr);
			break;
		}*/
	}
//	printf("****************Client Quit**************\n");
	return 0;
}
