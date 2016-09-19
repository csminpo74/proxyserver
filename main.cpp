#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#define  BUFF_SIZE   1024
using namespace std;
using std::string;

void errorflag(char *Message) {
	fputs(Message, stderr);
	fputc('\n', stderr);
	exit(1);	//오류가 발생시 프로그램 종료

}
int main(int argc, char **argv) {
	SOCKET server_sock, remote_sock;
	SOCKADDR_IN server_addr, remote_addr;
	string base = "hacking";
	int remote_addr_size;
	char   buff_rcv[BUFF_SIZE + 5];
	char   buff_snd[BUFF_SIZE + 5];

	server_sock = socket(PF_INET, SOCK_STREAM, 0);				//새로운 소켓생성후 대입					

	if (server_sock == -1)
		cout << "socket() error";
		exit(1);

	memset(&server_addr, 0, sizeof(server_addr));				//주소 저장가능한 구조체에 주소 설정중

	server_addr.sin_family = AF_INET;							//IPv4 인터넷 프로토롤
	server_addr.sin_port = htons(8080);							//사용할 port 번호 8080
	server_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");	//32bit IPv4 주소

	if (bind(server_sock, (SOCKADDR*)&server_addr, sizeof(server_addr)) == -1)
		errorflag("bind 실패");									//서버 소켓에 필요한 정보를 할당, 커널에 등록

	if (listen(server_sock, 5) == -1)	//remote 쪽 접속 대기
		errorflag("서버 대기 상태 모드 에러");

	remote_addr_size = sizeof(remote_addr);
	remote_sock = accept(server_sock, (SOCKADDR*)&remote_addr, &remote_addr_size);

	if (remote_sock == -1)
		errorflag("클라이언트 연락 수락 실패");

	recv(remote_sock, buff_rcv, BUFF_SIZE,0);
	if (base.find("hacking") != string::npos) {
		base = "ABCDEFG";
	}
	sprintf(buff_snd, "%d : %s", strlen(buff_rcv), buff_rcv);
	send(remote_sock, buff_snd, strlen(buff_snd) + 1,0);
	closesocket(remote_sock);	//소켓들을 정리해주고
	closesocket(server_sock);
	return 0;
}
