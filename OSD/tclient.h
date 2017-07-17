#ifndef TCLIENT_H
#define TCLIENT_H
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <thread>
#include <stdio.h>
#include <conio.h>
#include <winsock2.h>
#include <windows.h>
#include <algorithm>
#include <vector>

typedef void (*TelemHandler)(std::vector <std::string>);

using namespace std;

TelemHandler th;
WSADATA wsa;
SOCKET s;
struct sockaddr_in server;

void socket_init(const char* addr)
{
    printf("Initialising Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return;
    }

    printf("Initialised.\n");
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d", WSAGetLastError());
    }

    printf("Socket created.\n");

    server.sin_addr.s_addr = inet_addr(addr);
    server.sin_family = AF_INET;
    server.sin_port = htons(59666);
    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("Connect error. :)\n");
        getchar();
        return;
    }
    printf("Connected!!! :(\n");
}

void socket_stop(){
    closesocket(s);
    WSACleanup();
}

void socket_work(SOCKET sc, TelemHandler th){
    char recv_msg[100];
    int recv_size;

    while (1) {
        memset(recv_msg, NULL, 100);
        if ((recv_size = recv(sc, recv_msg, 100, 0)) != SOCKET_ERROR) {
            if(recv_msg[0] != NULL){
                cout << recv_msg << endl;
            }
        }
    }
}

#endif // TCLIENT_H
