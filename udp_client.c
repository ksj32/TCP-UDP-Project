/* UDP 클라이언트(udp_client) */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 7777   // 서버의 포트 번호 
#define BUFSIZE 1024 //문자 배열 크기 상수로 선언
// argv[1]은 수와 점 표기의 IP 주소
main(int argc, char *argv[])
{    
	int sockfd;
    struct sockaddr_in servAddr;
//송신, 수신할 메시지 저장하는 문자 배열
    char sendBuffer[BUFSIZE], recvBuffer[BUFSIZE]; 
//송신, 수신할 메시지 크기 저장하는 변수
    int recvLen, servLen;
  /*argc=인자 수, 인자 수가 2개가 아니면 아래와 같은 방식으로 사용하라고 예문 출력*/
    if(argc != 2) {
       fprintf(stderr, "Usage: %s IP_address\n", argv[0]);
       exit(1);
    }
    /*AF_INET 프로토콜 사용, SOCK_DGRAM(UDP) 방식으로 데이터 전송, 0은 운영체제가 자동으로 소켓 타입에 맞게 설정하겠다는 뜻. 인터넷으로 연결된 프로세스 간에 통신하고 UDP 방법을 이용하는 소켓을 생성 
*/
    if((sockfd=socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
       perror("sock failed");
       exit(1);
    }
    memset(&servAddr, 0, sizeof(servAddr)); //메모리 크기 지정
    // servAddr에 IP 주소와 포트 번호 저장 
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(PORT);
//Ctrl+c로 정지할 때까지 무한 반복
    while(1) {
	// 송신할 문자열 입력 받아 sendBuffer에 저장
       printf("Input Message : ");
       fgets(sendBuffer, BUFSIZE, stdin); //stdin은 표준 입력 버퍼
       
       /* sockfd 소켓을 통해 servAddr을 주소로 갖는 서버에게 데이터를 보냄(sendto 함수를 사용해서). sendto() 시스템 콜 호출*/
       if(sendto(sockfd, sendBuffer, strlen(sendBuffer), 0, (struct sockaddr*)&servAddr, sizeof(servAddr)) != strlen(sendBuffer)) {
          perror("sendto failed");
          exit(1);
       }
       servLen = sizeof(servLen);
       // sockfd 소켓으로 들어오는 데이터를 받아 recvBuffer에 저장
       if((recvLen=recvfrom(sockfd, recvBuffer, BUFSIZE-1, 0, (struct sockaddr*)&servAddr, &servLen)) == -1) {
          perror("recvfrom failed");
          exit(1);
       }
       recvBuffer[recvLen] = '\0';
       // 서버가 송신한 문자열 출력
       printf("Server: %s\n", recvBuffer);
    }
    close(sockfd); //소켓 종료
    exit(0);
}
