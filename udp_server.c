/* UDP ����(udp_server) */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> //IPv4 ���� ��� ��� ���
#include <arpa/inet.h> //�ּ� ��ȯ ��� ��� ���
#include <unistd.h>
#define PORT 7777   // ��Ʈ ��ȣ
#define BUFSIZE 1024 //���� �迭 ũ�� ����� ����
main()
{    
	int sockfd;
    struct sockaddr_in servAddr;
    struct sockaddr_in clntAddr;
    char recvBuffer[BUFSIZE]; //Ŭ���̾�Ʈ�κ��� ���� �޽��� ���� �迭
    char sendBuffer[BUFSIZE]; //Ŭ���̾�Ʈ���� ������ �޽��� ���� �迭
    int clntLen;
    int recvLen; //������ �޽��� ũ�� ���� ����
    int i = 0;
	int count = 0;
    int sendLen;
/*AF_INET �������� ���, SOCK_DGRAM(UDP) ������� ������ ����, 0�� �ü���� �ڵ����� ���� Ÿ�Կ� �°� �����ϰڴٴ� ��. ���ͳ����� ����� ���μ��� ���� ����ϰ� UDP ����� �̿��ϴ� ������ ���� 
socket() �ý��� ���� ȣ����
*/
    if((sockfd=socket(AF_INET, SOCK_DGRAM, 0)) == -1) { //���� ó��
       perror("socket failed");
       exit(1);
    }
    // servAddr�� 0���� �ʱ�ȭ 
    memset(&servAddr, 0, sizeof(servAddr));
    // servAddr�� IP �ּҿ� ��Ʈ ��ȣ�� ���� 
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(PORT);
    /* sockfd ���Ͽ� �ּ� ���� ����
bind �Լ��� ���н� &#8211;1�� ��ȯ�ϱ� ������ &#8211;1�� ��� ���� ó��*/
    if(bind(sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1) {
       perror("bind failed");
       exit(1);
    }
    // ���α׷� ������ų ������ ���� �ݺ�
    while(1) {
       clntLen = sizeof(clntAddr);
       /* sockfd �������� ������ �����͸� �޾� recvBuffer�� �����ϰ�
          Ŭ���̾�Ʈ �ּ� ������ clntAddr�� ���� */
       if((recvLen=recvfrom(sockfd, recvBuffer, BUFSIZE-1, 0, (struct sockaddr*)&clntAddr, &clntLen)) == -1) {
          perror("recvfrom failed");
          exit(1);
       }
       recvBuffer[recvLen] = '\0';
       // ���� �����͸� ��� 
       printf("Client: %s\n", recvBuffer);
       //udp_client���� ���� �޽����� ����ϰ� �۽��� �޽��� �Է�
       printf("Input Message : ");
       fgets(sendBuffer, BUFSIZE, stdin); //stdin�� ǥ�� �Է� ����
       /*sendto�Լ��� �̿��Ͽ� udp_client���� �޽��� �۽�
	sendto() �ý��� �� ȣ��*/
       if(sendto(sockfd, sendBuffer, strlen(sendBuffer), 0, (struct sockaddr*)&clntAddr, sizeof(clntAddr)) != strlen(sendBuffer)) {
          perror("sendto failed");
          exit(1);
//�Է��� �޽����� �۽ŵ� �޽��� ũ�Ⱑ �ٸ��� sendto ���з� ���� ó��
       }
    }
}
