/* TCP 서버(tcp_server) */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>  // access, read, write 등의 함수를 위한 헤더
#include <stdio.h>   // 기본적인 입출력을 위한 헤더
#include <stdlib.h>  // 문자 변환 등을 위한 헤더
#include <string.h>  // 문자열을 위한 헤더
#define MAXLINE 1024 // 문자 배열 크기 지정용 상수

int main(int argc, char **argv){
	int server_sockfd, client_sockfd; // 소켓 통신을 위한 변수
	int state, client_len; // 소켓 통신시 상태 및 길이 파악을 위한 변수
	pid_t pid;
	FILE *fp;
	struct sockaddr_un clientaddr, serveraddr; // 소켓 통신을 위한 구조체
	char buf[MAXLINE];     // Client에서 보낸 메시지 저장할 배열
	char sendMsg[MAXLINE]; // Server가 보낼 메시지 저장할 배열
	// 경로 설정이 제대로 되어있는지 인자를 확인 (argc = 인자 개수)
	if(argc != 2){ 
		printf("Usage : %s [socket file name]\n", argv[0]);
		printf("example : %s /tmp/mysocket\n", argv[0]);
		exit(0);
	}
	if(access(argv[1], F_OK) == 0){			 // 파일 또는 디렉토리의 사용자 권한 체크 (F_OK = 파일 존재 여부) 
		unlink(argv[1]);			 // 파일 삭제 
	}
	client_len = sizeof(clientaddr); 		// 길이 파악
	/* AF_UNIX 프로토콜 사용, SOCK_STREAM(TCP) 방식으로 데이터 전송, 0은 운영체제가 자동으로 소켓 타입에 맞게 설정하겠다는 뜻 */
	if((server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
		perror("socket error : ");
		exit(0);
	}
	bzero(&serveraddr, sizeof(serveraddr)); 	// serveraddr의 사이즈만큼 0으로 대체
	serveraddr.sun_family = AF_UNIX;
	strcpy(serveraddr.sun_path, argv[1]);		// 문자열 복사
	/* 소켓에 IP주소와 포트번호를 지정하여 통신 준비를 함, bind시 setsocket() 시스템 콜 호출 */
	state = bind(server_sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)); 	
	// bind()는 실패하면 -1을 반환하므로 에러 처리
	if(state == -1)
	{
		perror("bind error : ");
		exit(0);
	}
	state = listen(server_sockfd, 5); 	// listen()함수 -1 반환 시 에러처리
	if(state == -1)
	{
		perror("listen error : ");
		exit(0);
	}
	while(1){ 		// 스레드처럼 연결될 때까지 계속 반복하고 있음
	client_sockfd = accept(server_sockfd, (struct sockaddr *)&clientaddr, &client_len);
	pid = fork(); 		// fork()는 부모일 경우 자식 프로세스 PID, 자식일 경우 0을 반환함
	if(pid == 0){  		// pid가 0이라는 것은 현재 자식 프로세스라는 뜻
			if(client_sockfd == -1){
				perror("Accept error : ");
				exit(0);
			}
			while(1){
				memset(buf, 0x00, MAXLINE); // 메모리 크기 설정
				// client에서 보낸 정보를 수신함
				if(read(client_sockfd, buf, MAXLINE) <= 0){
					close(client_sockfd);
					exit(0);
				}
                                // Client에서 보낸 메시지 출력
				printf("client : %s\n", buf); 
				// Server가 Client에게 송신할 메시지 입력
				printf("Put Message : ");
				scanf("%[^\n]", sendMsg);
				getchar(); // 버퍼 초기화
				// Client에게 메시지 송신
			        write(client_sockfd, sendMsg, sizeof(sendMsg));
		}	
	}
} 
	close(client_sockfd); // 연결을 해제함
}
