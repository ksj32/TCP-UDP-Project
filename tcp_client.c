/* TCP Ŭ���̾�Ʈ(tcp_client) */

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
	int client_len;
	int client_sockfd;	
	FILE *fp_in;
	char buf_in[MAXLINE];  //Server�� �۽��� �޽��� ������ �迭
	char buf_get[MAXLINE]; //Client�� ������ �޽��� ������ �迭
	char result;  
	int i;
	struct sockaddr_un clientaddr;
	if(argc != 2){ //���ڰ� 2������ Ȯ�� (argc = ���� ����)
		printf("Usage : %s [socket file name]\n", argv[0]);
		printf("example : %s /tmp/mysocket\n", argv[0]);
		exit(0);
	}
	client_sockfd = socket(AF_UNIX, SOCK_STREAM, 0); //���� ����
	if(client_sockfd == -1){
		perror("error : "); //���� ó��
		exit(0);
	}
	/* bzero : �޸� ������ size ����Ʈ��ŭ 0���� ä��� �Լ�
	-> ����ü ������ 0���� �ʱ�ȭ �Ͽ� ���ʿ��� �����͸� ������ ������ �����ִ� ���� �����ϱ� ���� */
	bzero(&clientaddr, sizeof(clientaddr));		
	clientaddr.sun_family = AF_UNIX; 			//sun_family�� AF_UNIX�� ����
	strcpy(clientaddr.sun_path, argv[1]); 		//���ڿ� ����
	client_len = sizeof(clientaddr);
	if(connect(client_sockfd,(struct sockaddr *)&clientaddr, client_len) < 0)
	{
		perror("connect error: ");
		exit(0);
	}
	while(1)
	{
		memset(buf_in, 0x00, MAXLINE); //�޸� ũ��
		memset(buf_get, 0x00, MAXLINE);
		printf("Put Message : "); //Server�� �۽��� �޽��� �Է�
		fgets(buf_in, MAXLINE, stdin); 

		write(client_sockfd, buf_in, strlen(buf_in)); 	//server�� ����	(=send)	
		read(client_sockfd, buf_get, MAXLINE); 			//server�� ���� �޽��� ���� (=receive)
		printf("server : %s\n", buf_get); //���Ź��� �޽����� �����
	}
	close(client_sockfd); //������ ������
	exit(0);
}
