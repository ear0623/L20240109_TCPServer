#define _CRT_SECURE_NO_WARNINGS

#include "iostream"
#include "WinSock2.h"
#include <stdlib.h>
#include "time.h"


#pragma comment(lib,"ws2_32")// 추가없이 불러오는 방법

using namespace std;

int main()
{
	srand((unsigned int)time(nullptr));

	WSAData wsaData;
	int Result = WSAStartup(MAKEWORD(2, 2), &wsaData);//dll을 불러옴//바이트단위로 밀어서 소수점으로 만듬
	if (Result != 0)
	{
		cout << "Windsock dll error" << GetLastError() << endl;//가지고 있는 마지막 애러코드 표시
		exit(-1);
	}

	SOCKET ListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);//UDPTCP쓸꺼야// 연결지향형//소켓에 번호를 줘//
	//파일디스크립터// 컴퓨터 자원//소켓FD//윈도우는 내부구조다르다.
	if (ListenSocket == INVALID_SOCKET)
	{
		cout << "Invalid_Socket" << GetLastError() << endl;
		exit(-1);
	}

	//밖에서 부루는 주소 ip
	//iptime 네트워크용 리눅스 컴퓨터임 사실//미디어덱//리얼텍//브로드컴//라우터 3com//Cisco//fkdnxlddkfrhflwma//
	//ㅍ6는 2^64//사설Ip/안에서만쓰는거말함//ip도 자원이라 나눠써야함.//모뎀->돌려써야되서 매번바뀜.->게임할때가 문제

	//Whois//icen.org//
	//NIC당 하나//

	SOCKADDR_IN ListenSockAddr;
	//port 2bite 숫자로 할당// 1대1연결//Wellknowport//443//RSA2048암호화//숫자키3개2048//푸는게 전재 없는사람은 시간안에 못푸는걸로//443802221//3479,80 플스//
	//porttoscanning//ping 포트는 Icmp//3306MySQL//
	memset(&ListenSockAddr, 0, sizeof(ListenSockAddr));//0으로 채워주세요
	//ZeroMemory(&ListenSockAddr, 0, sizeof(ListenSockAddr))// 저런게 있다.
	ListenSockAddr.sin_family = AF_INET;//v4를 찾으려면 주소체계인 family
	ListenSockAddr.sin_addr.s_addr = INADDR_ANY;//union 매크로로 만들어놈//특정 ip넣어줘야함//주로 config에서작업함.//
	ListenSockAddr.sin_port = htons(40211);//1024 밑으로 많이씀//똑똑할때
	//램카드랑 연결할차레
	Result = bind(ListenSocket, (SOCKADDR*)&ListenSockAddr, sizeof(ListenSockAddr));//연결
	if (Result == SOCKET_ERROR)
	{
		cout << "Can`t Bind : " << GetLastError() << endl;//가지고 있는 마지막 애러코드 표시
		exit(-1);
	}//port 당하나 가 베이스 // 예외 있음//

	Result = listen(ListenSocket, 5);//backlog 대기인원//multithread이후 의미가 없어짐.받는사람이 여러명이라서/
	if (Result == SOCKET_ERROR)
	{
		cout << "Can`t Bind : " << GetLastError() << endl;//가지고 있는 마지막 애러코드 표시

		exit(-1);
	}
	SOCKADDR_IN ClientSockAddr;
	memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
	int ClientSocketAddrLength = sizeof(ClientSockAddr);
	SOCKET ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientSockAddr, &ClientSocketAddrLength);//들어온애 정보,tkdlwmsms vhdlsxjfh//실제 통신용 소켓
	//blocking함수라서 들어올때까지 멈춰있음.//실제 일할때는 안씀//
	if (ClientSocket == INVALID_SOCKET)
	{
		cout << "accept fail : " << GetLastError() << endl;

		exit(-1);
	}//listen은 문지기//

	//send/recv

	while (true)
	{

		int FirstNumber = rand() % 10000;
		int SecondNumber = rand() % 9999 + 1;
		int OperaterIndex = rand() % 5;
		char Operator[5] = { '+','-','*','/','%'};

		char Buffer[1024] = { 0, };
		sprintf(Buffer, "%d%c%d", FirstNumber, Operator[OperaterIndex], SecondNumber);
		//sprintf_s(Buffer,1024, "%d%c%d", FirstNumber, Operator[OperaterIndex], SecondNumber);
		//buffer overflow 공격;
		//sprintf_버퍼에 저장된 문자열을 넣어준다.


		int SentByte = send(ClientSocket, Buffer, (int)(strlen(Buffer) + 1), 0);// 한번에 못보내면 또보내고또보내고,,
		if (SentByte < 0)//자료 못받은거임. 에러로
		{
			cout << "accept fail : " << GetLastError() << endl;
			continue;
			//exit(-1);
		}
		else if (SentByte == 0)
		{
			cout << "Disconnected : " << GetLastError() << endl;
			continue;
			//exit(-1);
		}
		else
		{
			cout << "Sent Byte : " << SentByte << "," << Buffer << endl;

		}

		char RecvBuffer[1024] = { 0, };//최대버퍼크기//임시저장//서버에서 0은 끈자는 예기


		int RecvByte = recv(ClientSocket, RecvBuffer, sizeof(RecvBuffer), 0);//최대버퍼길이//os로 싸짊어저서 보내줌
		if (RecvByte < 0)//자료 못받은거임. 에러로
		{
			cout << "accept fail : " << GetLastError() << endl;
			exit(-1);
		}
		else if (RecvByte == 0)//상대가 나간거//
		{
			cout << "Disconnected : " << GetLastError() << endl;
			exit(-1);//원래 예외처리 // 절대 안끄는게 맞음/
		}
		else
		{
			cout << "Sent Byte : " << RecvByte << ",    " << RecvBuffer << endl;//가지고 있다가 보냄

		}

	}
	closesocket(ClientSocket);
	closesocket(ListenSocket);

	WSACleanup();//End;
	//window전용//멀티프로세스는 윈도우지원없음

	return 0;
}