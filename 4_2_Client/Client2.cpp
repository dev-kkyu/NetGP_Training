#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���� ���� API ��� �� ��� ����

#include <WS2tcpip.h>
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ

using namespace std;

class portList {
private:
	int port;

	static int count;
public:
	portList() {
		port = 0;
		++count;
	}
	portList(int num) {
		port = num;
		++count;
	}
	~portList() {
		--count;
	}

	static int isCount() {
		return count;
	}
	int isPort() {
		return port;
	}
};
int portList::count = 0;

char* GetIPbyName(const char* name)
{
	struct hostent* ptr = gethostbyname(name);

	if (ptr == NULL) {
		cout << "�����͸� �޾ƿ��µ� �����Ͽ����ϴ�." << endl;
		return NULL;
	}

	return ptr->h_addr;
}

int main(int argc, char* argv[])
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	cout << "�Էµ� �ּ� : " << argv[1] << endl;

	char* ServerIP = GetIPbyName(argv[1]);
	char str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, ServerIP, str, sizeof(str));

	cout << "ȣ��Ʈ IP : " << str << "\n\n";
	
	portList* list[10]{};

	for (int i = stoi(argv[2]); i <= stoi(argv[3]); ++i) {		// �����࿡�� �Է¹��� ��Ʈ ����
		// ���� ����
		SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

		// connect()
		struct sockaddr_in serveraddr;
		memset(&serveraddr, 0, sizeof(serveraddr));
		serveraddr.sin_family = AF_INET;
		inet_pton(AF_INET, str, &serveraddr.sin_addr);		// �Է��� ����
		serveraddr.sin_port = htons(i);							//i��° ��Ʈ ȣ��

		retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

		if (retval == SOCKET_ERROR) {
			cout << i << " ��Ʈ ȣ�� ����" << endl;
		}
		else {
			cout << i << " ��Ʈ ȣ�� ����" << endl;
			list[portList::isCount() - 1] = new portList(i);
		}

		// ���� �ݱ�
		closesocket(sock);
	}

	// ������ ��Ʈ ���� ���
	cout << "\nLISTENING ������ ��Ʈ��ȣ : ";
	for (int i = 0; i < portList::isCount(); ++i) {
		cout << list[i]->isPort() << " ";
	}
	cout << endl;
	cout << portList::isCount() << " �� ����" << endl;

	// ���� ����
	WSACleanup();
	return 0;
}