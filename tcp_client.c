/* TCP 클라이언트(tcp_client) */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h> //access, read, write 등의 함수를 위한 헤더
#include <stdio.h> //기본적인 입출력을 위한 헤더
#include <stdlib.h> //문자 변환 등을 위한 헤더 
#include <string.h> //문자열을 위한 헤더
#define MAXLINE 1024 //문자 배열 크기 지정용 상수

int main(int argc, char **argv){
	int client_len;
	int client_sockfd;	
	FILE *fp_in;
	char buf_in[MAXLINE];  //Server로 송신할 메시지 저장할 배열
	char buf_get[MAXLINE]; //Client가 수신할 메시지 저장할 배열
	char result;  
	int i;
	struct sockaddr_un clientaddr;
	if(argc != 2){ //인자가 2개인지 확인 (argc = 인자 개수)
		printf("Usage : %s [socket file name]\n", argv[0]);
		printf("example : %s /tmp/mysocket\n", argv[0]);
		exit(0);
	}
	client_sockfd = socket(AF_UNIX, SOCK_STREAM, 0); //소켓 설정
	if(client_sockfd == -1){
		perror("error : "); //에러 처리
		exit(0);
	}
	/* bzero : 메모리 공간을 size 바이트만큼 0으로 채우는 함수
	-> 구조체 변수를 0으로 초기화 하여 불필요한 데이터를 쓰레기 값으로 남겨주는 것을 방지하기 위함 */
	bzero(&clientaddr, sizeof(clientaddr));		
	clientaddr.sun_family = AF_UNIX; 			//sun_family는 AF_UNIX를 뜻함
	strcpy(clientaddr.sun_path, argv[1]); 		//문자열 복사
	client_len = sizeof(clientaddr);
	if(connect(client_sockfd,(struct sockaddr *)&clientaddr, client_len) < 0)
	{
		perror("connect error: ");
		exit(0);
	}
	while(1)
	{
		memset(buf_in, 0x00, MAXLINE); //메모리 크기
		memset(buf_get, 0x00, MAXLINE);
		printf("Put Message : "); //Server로 송신할 메시지 입력
		fgets(buf_in, MAXLINE, stdin); 

		write(client_sockfd, buf_in, strlen(buf_in)); 	//server에 전송	(=send)	
		read(client_sockfd, buf_get, MAXLINE); 			//server가 보낸 메시지 수신 (=receive)
		printf("server : %s\n", buf_get); //수신받은 메시지를 출력함
	}
	close(client_sockfd); //연결을 해제함
	exit(0);
}
