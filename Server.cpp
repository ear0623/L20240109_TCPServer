#include "iostream"
#include "WinSock2.h"

#pragma comment(lib,"ws2_32")// �߰����� �ҷ����� ���

using namespace std;

int main()
{
	WSAData wsaData;
	int Result = WSAStartup(MAKEWORD(2, 2), &wsaData);//dll�� �ҷ���//����Ʈ������ �о �Ҽ������� ����
	if (Result != 0)
	{
		cout << "Windsock dll error" <<GetLastError() << endl;//������ �ִ� ������ �ַ��ڵ� ǥ��
		exit(-1);
	}

	SOCKET ListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);//UDPTCP������// ����������//���Ͽ� ��ȣ�� ��//
	//���ϵ�ũ����// ��ǻ�� �ڿ�//����FD//������� ���α����ٸ���.
	if (ListenSocket == INVALID_SOCKET)
	{
		cout << "Invalid_Socket" << GetLastError()<<endl;
		exit(-1);
	}

	//�ۿ��� �η�� �ּ� ip
	//iptime ��Ʈ��ũ�� ������ ��ǻ���� ���//�̵�//������//��ε���//����� 3com//Cisco//fkdnxlddkfrhflwma//
	//��6�� 2^64//�缳Ip/�ȿ��������°Ÿ���//ip�� �ڿ��̶� ���������.//��->������ߵǼ� �Ź��ٲ�.->�����Ҷ��� ����

	//Whois//icen.org//
	//NIC�� �ϳ�//

	SOCKADDR_IN ListenSockAddr;
	//port 2bite ���ڷ� �Ҵ�// 1��1����//Wellknowport//443//RSA2048��ȣȭ//����Ű3��2048//Ǫ�°� ���� ���»���� �ð��ȿ� ��Ǫ�°ɷ�//443802221//3479,80 �ý�//
	//porttoscanning//ping ��Ʈ�� Icmp//3306MySQL//
	memset(&ListenSockAddr, 0, sizeof(ListenSockAddr));//0���� ä���ּ���
	//ZeroMemory(&ListenSockAddr, 0, sizeof(ListenSockAddr))// ������ �ִ�.
	ListenSockAddr.sin_family = AF_INET;
	ListenSockAddr.sin_addr.s_addr = INADDR_ANY;//union ��ũ�η� ������//Ư�� ip�־������//�ַ� config�����۾���.//
	ListenSockAddr.sin_port = htons(40211);//1024 ������ ���̾�//�ȶ��Ҷ�
	//��ī��� ����������
	Result = bind(ListenSocket, (SOCKADDR*)&ListenSockAddr, sizeof(ListenSockAddr));//����
	if (Result == SOCKET_ERROR)
	{
		cout << "Can`t Bind : " << GetLastError() << endl;//������ �ִ� ������ �ַ��ڵ� ǥ��
		exit(-1);
	}//port ���ϳ� �� ���̽� // ���� ����//

	Result= listen(ListenSocket, 5);//backlog ����ο�//multithread���� �ǹ̰� ������.�޴»���� �������̶�/
	if (Result == SOCKET_ERROR)
	{
		cout << "Can`t Bind : " << GetLastError() << endl;//������ �ִ� ������ �ַ��ڵ� ǥ��
		exit(-1);
	}
	SOCKADDR_IN ClientSockAddr;
	memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
	int ClientSocketAddrLength = sizeof(ClientSockAddr);
	SOCKET ClientSocket =accept(ListenSocket, (SOCKADDR*)&ClientSockAddr, &ClientSocketAddrLength);//���¾� ����,tkdlwmsms vhdlsxjfh//���� ��ſ� ����
	//blocking�Լ��� ���ö����� ��������.//���� ���Ҷ��� �Ⱦ�//
	if (ClientSocket == INVALID_SOCKET)
	{
		cout << "accept fail : " << GetLastError() << endl;
		exit(-1);
	}//listen�� ������//

	//send/recv
	const char Buffer[] = { "Hellow World" };
	int SentByte = send(ClientSocket, Buffer, strlen(Buffer), 0);// �ѹ��� �������� �Ǻ�����Ǻ�����,,
	if (SentByte < 0)//�ڷ� ����������. ������
	{
		cout << "accept fail : " << GetLastError() << endl;
		exit(-1);
	}
	else if (SentByte == 0)
	{
		cout << "Disconnected : " << GetLastError() << endl;
		exit(-1);
	}
	else
	{
		cout << "Sent Byte : " <<SentByte<<"," << Buffer<< endl;

	}

	char RecvBuffer[1024] = { 0, };//�ִ����ũ��//�ӽ�����
	int RecvByte = recv(ClientSocket, RecvBuffer, sizeof(RecvBuffer), 0);//�ִ���۱���//os�� ���������� ������
	if (RecvByte < 0)//�ڷ� ����������. ������
	{
		cout << "accept fail : " << GetLastError() << endl;
		exit(-1);
	}
	else if (RecvByte == 0)//��밡 ������//
	{
		cout << "Disconnected : " << GetLastError() << endl;
		exit(-1);
	}
	else
	{
		cout << "Sent Byte : " << RecvByte << "," << RecvBuffer << endl;//������ �ִٰ� ����

	}

	closesocket(ClientSocket);
	closesocket(ListenSocket);

	WSACleanup();//End;
	//window����
	return 0;
}