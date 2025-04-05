#include "ecrypt-sync.h"
#include <vcruntime_string.h>
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8000
#define KEY_SIZE 16
#define IV_SIZE 16
#define BUFFER_SIZE 1024

int main(int argc, char** argv)
{	
	ECRYPT_ctx ctx;
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "WSAStartup failed\n";
		return 1;
	}
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in address = { AF_INET, htons(PORT), INADDR_ANY };
	if (bind(serverSocket, (sockaddr*)&address, sizeof(address)) < 0) {
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}
	if (listen(serverSocket, 1) < 0) {
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}
	while (true) {
		SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
		if (clientSocket == INVALID_SOCKET) {
			continue;
		}

		u_long mode = 1;

		if (ioctlsocket(clientSocket, FIONBIO, &mode) != 0) {
			closesocket(clientSocket);
		}

		printf("Connection is established successfully.");
		while (true) {
			char buffer[BUFFER_SIZE] = { 0 };
			u8 K[KEY_SIZE] = { 0 }, IV[IV_SIZE] = { 0 }, in[BUFFER_SIZE] = { 0 }, out[BUFFER_SIZE] = { 0 };
			SSIZE_T bytesReadCount = recv(clientSocket, buffer, KEY_SIZE, 0);
			if (bytesReadCount < KEY_SIZE) {
				continue;
			}
			else {
				printf("Received KEY: %s", buffer);
				for (int i = 0; i < KEY_SIZE; i++) {
					K[i] = buffer[i];
				}
			}
			memset(buffer, 0, sizeof(buffer));
			bytesReadCount = recv(clientSocket, buffer, IV_SIZE, 0);
			if (bytesReadCount < IV_SIZE) {
				continue;
			}
			else {
				printf("Received IV: %s", buffer);
				for (int i = 0; i < IV_SIZE; i++) {
					IV[i] = buffer[i];
				}
			 } 
			memset(buffer, 0, sizeof(buffer));
			bytesReadCount = recv(clientSocket, buffer, BUFFER_SIZE, 0);
			for (int i = 0; i < bytesReadCount; i++) {
				in[i] = buffer[i];
			}
			printf("Received data: %s", buffer);
			ECRYPT_init();
			ECRYPT_keysetup(&ctx, K, KEY_SIZE * 8, IV_SIZE * 8);
			ECRYPT_ivsetup(&ctx, IV);
			ECRYPT_encrypt_bytes(&ctx, in, out, BUFFER_SIZE);

			char* output = new char[bytesReadCount];
			for (int i = 0; i < bytesReadCount; i++) {
				output[i] = out[i];
			}
			send(clientSocket, output, bytesReadCount, 0);
			delete[] output;
			memset(buffer, 0, sizeof(buffer));
		}
	}
	closesocket(serverSocket);
	WSACleanup();
	return 0;
}