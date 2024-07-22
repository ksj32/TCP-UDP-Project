/* UDP 서버(udp_server) */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> //IPv4 전용 기능 사용 헤더
#include <arpa/inet.h> //주소 변환 기능 사용 헤더
#include <unistd.h>
#define PORT 7777   // 포트 번호
#define BUFSIZE 1024 //문자 배열 크기 상수로 선언
main()
{    
	int sockfd;
    struct sockaddr_in servAddr;
    struct sockaddr_in clntAddr;
    char recvBuffer[BUFSIZE]; //클라이언트로부터 받을 메시지 저장 배열
    char sendBuffer[BUFSIZE]; //클라이언트에게 전송할 메시지 저장 배열
    int clntLen;
    int recvLen; //수신할 메시지 크기 지정 변수
    int i = 0;
	int count = 0;
    int sendLen;
/*AF_INET 프로토콜 사용, SOCK_DGRAM(UDP) 방식으로 데이터 전송, 0은 운영체제가 자동으로 소켓 타입에 맞게 설정하겠다는 뜻. 인터넷으로 연결된 프로세스 간에 통신하고 UDP 방법을 이용하는 소켓을 생성 
socket() 시스템 콜을 호출함
*/
    if((sockfd=socket(AF_INET, SOCK_DGRAM, 0)) == -1) { //에러 처리
       perror("socket failed");
       exit(1);
    }
    // servAddr를 0으로 초기화 
    memset(&servAddr, 0, sizeof(servAddr));
    // servAddr에 IP 주소와 포트 번호를 저장 
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(PORT);
    /* sockfd 소켓에 주소 정보 연결
bind 함수는 실패시 &#8211;1을 반환하기 때문에 &#8211;1일 경우 에러 처리*/
    if(bind(sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1) {
       perror("bind failed");
       exit(1);
    }
    // 프로그램 중지시킬 때까지 무한 반복
    while(1) {
       clntLen = sizeof(clntAddr);
       /* sockfd 소켓으로 들어오는 데이터를 받아 recvBuffer에 저장하고
          클라이언트 주소 정보를 clntAddr에 저장 */
       if((recvLen=recvfrom(sockfd, recvBuffer, BUFSIZE-1, 0, (struct sockaddr*)&clntAddr, &clntLen)) == -1) {
          perror("recvfrom failed");
          exit(1);
       }
       recvBuffer[recvLen] = '\0';
       // 받은 데이터를 출력 
       printf("Client: %s\n", recvBuffer);
       //udp_client에게 받은 메시지를 출력하고 송신할 메시지 입력
       printf("Input Message : ");
       fgets(sendBuffer, BUFSIZE, stdin); //stdin은 표준 입력 버퍼
       /*sendto함수를 이용하여 udp_client에게 메시지 송신
	sendto() 시스템 콜 호출*/
       if(sendto(sockfd, sendBuffer, strlen(sendBuffer), 0, (struct sockaddr*)&clntAddr, sizeof(clntAddr)) != strlen(sendBuffer)) {
          perror("sendto failed");
          exit(1);
//입력한 메시지와 송신된 메시지 크기가 다르면 sendto 실패로 에러 처리
       }
    }
}
