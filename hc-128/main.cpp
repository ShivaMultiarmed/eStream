#include "ecrypt-sync.h"
#include <vcruntime_string.h>
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#define KEY_SIZE 16
#define IV_SIZE 16
#define BUFFER_SIZE 1024

int main(int argc, char** argv)
{	
	ECRYPT_ctx ctx;
	int PORT;
	if (argc < 2) {
		return 1;
	}
	else {
		PORT = std::stoi(argv[1]);
		if (PORT != 8000 && PORT != 9000) {
			return 1;
		}
	}
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

		printf("Connection is established successfully.\n");
		while (true) {
			char buffer[BUFFER_SIZE] = { 0 };
			u8 K[KEY_SIZE] = { 0 }, IV[IV_SIZE] = { 0 }, in[BUFFER_SIZE] = { 0 }, out[BUFFER_SIZE] = { 0 };
			SSIZE_T bytesReadCount = recv(clientSocket, buffer, KEY_SIZE, 0);
			if (bytesReadCount < KEY_SIZE) {
				continue;
			}
			else {
				printf("\nReceived KEY:\n");
				for (int i = 0; i < KEY_SIZE; i++) {
					K[i] = buffer[i];
					printf("%0x", K[i]);
				}
			}
			memset(buffer, 0, sizeof(buffer));
			bytesReadCount = recv(clientSocket, buffer, IV_SIZE, 0);
			if (bytesReadCount < IV_SIZE) {
				continue;
			}
			else {
				printf("\nReceived IV:\n");
				for (int i = 0; i < IV_SIZE; i++) {
					IV[i] = buffer[i];
					printf("%0x", IV[i]);
				}
			 } 
			memset(buffer, 0, sizeof(buffer));
			bytesReadCount = recv(clientSocket, buffer, BUFFER_SIZE, 0);
			for (int i = 0; i < bytesReadCount; i++) {
				in[i] = buffer[i];
			}
			printf("\nReceived data: %s\n", buffer);
			ECRYPT_init();
			ECRYPT_keysetup(&ctx, K, KEY_SIZE * 8, IV_SIZE * 8);
			ECRYPT_ivsetup(&ctx, IV);
			ECRYPT_encrypt_bytes(&ctx, in, out, bytesReadCount);
			send(clientSocket, (char *) &out, bytesReadCount, 0);
			printf("Proccessed data:\n");
			for (int i = 0; i < bytesReadCount; i++) {
				printf("%0x", out[i]);
			}
			memset(buffer, 0, sizeof(buffer));
		}
	}
	closesocket(serverSocket);
	WSACleanup();
	return 0;
}