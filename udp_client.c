/* UDP Ŭ���̾�Ʈ(udp_client) */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 7777   // ������ ��Ʈ ��ȣ 
#define BUFSIZE 1024 //���� �迭 ũ�� ����� ����
// argv[1]�� ���� �� ǥ���� IP �ּ�
main(int argc, char *argv[])
{    
	int sockfd;
    struct sockaddr_in servAddr;
//�۽�, ������ �޽��� �����ϴ� ���� �迭
    char sendBuffer[BUFSIZE], recvBuffer[BUFSIZE]; 
//�۽�, ������ �޽��� ũ�� �����ϴ� ����
    int recvLen, servLen;
  /*argc=���� ��, ���� ���� 2���� �ƴϸ� �Ʒ��� ���� ������� ����϶�� ���� ���*/
    if(argc != 2) {
       fprintf(stderr, "Usage: %s IP_address\n", argv[0]);
       exit(1);
    }
    /*AF_INET �������� ���, SOCK_DGRAM(UDP) ������� ������ ����, 0�� �ü���� �ڵ����� ���� Ÿ�Կ� �°� �����ϰڴٴ� ��. ���ͳ����� ����� ���μ��� ���� ����ϰ� UDP ����� �̿��ϴ� ������ ���� 
*/
    if((sockfd=socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
       perror("sock failed");
       exit(1);
    }
    memset(&servAddr, 0, sizeof(servAddr)); //�޸� ũ�� ����
    // servAddr�� IP �ּҿ� ��Ʈ ��ȣ ���� 
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(PORT);
//Ctrl+c�� ������ ������ ���� �ݺ�
    while(1) {
	// �۽��� ���ڿ� �Է� �޾� sendBuffer�� ����
       printf("Input Message : ");
       fgets(sendBuffer, BUFSIZE, stdin); //stdin�� ǥ�� �Է� ����
       
       /* sockfd ������ ���� servAddr�� �ּҷ� ���� �������� �����͸� ����(sendto �Լ��� ����ؼ�). sendto() �ý��� �� ȣ��*/
       if(sendto(sockfd, sendBuffer, strlen(sendBuffer), 0, (struct sockaddr*)&servAddr, sizeof(servAddr)) != strlen(sendBuffer)) {
          perror("sendto failed");
          exit(1);
       }
       servLen = sizeof(servLen);
       // sockfd �������� ������ �����͸� �޾� recvBuffer�� ����
       if((recvLen=recvfrom(sockfd, recvBuffer, BUFSIZE-1, 0, (struct sockaddr*)&servAddr, &servLen)) == -1) {
          perror("recvfrom failed");
          exit(1);
       }
       recvBuffer[recvLen] = '\0';
       // ������ �۽��� ���ڿ� ���
       printf("Server: %s\n", recvBuffer);
    }
    close(sockfd); //���� ����
    exit(0);
}
