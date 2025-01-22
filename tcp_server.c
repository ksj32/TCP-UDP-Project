/* TCP ����(tcp_server) */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h> //access, read, write ���� �Լ��� ���� ���
#include <stdio.h> //�⺻���� ������� ���� ���
#include <stdlib.h> //���� ��ȯ ���� ���� ���
#include <string.h> //���ڿ��� ���� ���
#define MAXLINE 1024 //���� �迭 ũ�� ������ ���

int main(int argc, char **argv){
	int server_sockfd, client_sockfd; //���� ����� ���� ����
	int state, client_len; //���� ��Ž� ���� �� ���� �ľ��� ���� ����
	pid_t pid;
	FILE *fp;
	struct sockaddr_un clientaddr, serveraddr; //���� ����� ���� ����ü
	char buf[MAXLINE]; //Client���� ���� �޽��� ������ �迭
	char sendMsg[MAXLINE]; //Server�� ���� �޽��� ������ �迭
	//��� ������ ����� �Ǿ��ִ��� ���ڸ� Ȯ�� argc = ���� ����
	if(argc != 2){ 
		printf("Usage : %s [socket file name]\n", argv[0]);
		printf("example : %s /tmp/mysocket\n", argv[0]);
		exit(0);
	}
	if(access(argv[1], F_OK) == 0){			// ���� �Ǵ� ���丮�� ����� ���� üũ (F_OK = ���� ���� ����)  
		unlink(argv[1]);					// ���� ���� 
	}
	client_len = sizeof(clientaddr); //���� �ľ�
	/*AF_UNIX �������� ���, SOCK_STREAM(TCP) ������� ������ ����, 0�� �ü���� �ڵ����� ���� Ÿ�Կ� �°� �����ϰڴٴ� �� */
	if((server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0))<0){
		perror("socket error : ");
		exit(0);
	}
	bzero(&serveraddr, sizeof(serveraddr)); //serveraddr�� �����ŭ 0���� ��ü
	serveraddr.sun_family = AF_UNIX;
	strcpy(serveraddr.sun_path, argv[1]); //���ڿ� ����
	/* ���Ͽ� IP�ּҿ� ��Ʈ��ȣ�� �����Ͽ� ��� �غ� ��
	bind�� setsocket() �ý��� �� ȣ�� */
	state = bind(server_sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)); //bind()�� �����ϸ� -1�� ��ȯ�ϹǷ� ���� ó��
	if(state == -1)
	{
		perror("bind error : ");
		exit(0);
	}
	state = listen(server_sockfd, 5); //listen() -1 ��ȯ �� ����ó��
	if(state == -1)
	{
		perror("listen error : ");
		exit(0);
	}
	while(1){ //������ó�� ����� ������ ��� �ݺ��ϰ� ����
	client_sockfd = accept(server_sockfd, (struct sockaddr *)&clientaddr, &client_len);
	pid = fork(); //fork()�� �θ��� ��� �ڽ� ���μ��� PID, �ڽ��� ��� 0�� ��ȯ��
	if(pid == 0){ //pid�� 0�̶�� ���� ���� �ڽ� ���μ������ ��
			if(client_sockfd == -1){
				perror("Accept error : ");
				exit(0);
			}
			while(1){
				memset(buf, 0x00, MAXLINE); //�޸� ũ�� ����
				//client���� ���� ������ ������
				if(read(client_sockfd, buf, MAXLINE) <= 0){
					close(client_sockfd);
					exit(0);
				}
                //Client���� ���� �޽��� ���
				printf("client : %s\n", buf); 
				//Server�� Client���� �۽��� �޽��� �Է�
				printf("Put Message : ");
				scanf("%[^\n]", sendMsg);
				getchar(); //���� �ʱ�ȭ
				//Client���� �޽��� �۽�
			    write(client_sockfd, sendMsg, sizeof(sendMsg));
		}	
	}
} 
	close(client_sockfd); //������ ������
}
