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

typedef void (*TelemHandler)(std::string);

using namespace std;

struct sockaddr_in server;
TelemHandler th;
WSADATA wsa;
SOCKET s;

bool socket_init(const char* addr)
{
    printf("Initialising Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Failed. Error Code : %d ;)\nTo quit press enter...", WSAGetLastError());
        return 1;
    }

    printf("Initialised.\n");
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d ;)\nTo quit press enter...", WSAGetLastError());
        return 1;
    }

    printf("Socket created.\n");

    server.sin_addr.s_addr = inet_addr(addr);
    server.sin_family = AF_INET;
    server.sin_port = htons(59666);
    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("Connect error. ;)\nTo quit press enter...");
        return 1;
    }
    printf("Connected. :(\n\n");
    //("Everything is bad!!!\n\n");
    return 0;
}

void socket_stop(){
    closesocket(s);
    WSACleanup();
}

void socket_work(SOCKET sc, TelemHandler th){//, bool fff){
    char recv_msg[2000];
    int recv_size;

    while (true) {
        memset(recv_msg, NULL, 2000);
        try{
            if ((recv_size = recv(sc, recv_msg, 2000, 0)) != SOCKET_ERROR) {
                if(recv_msg[0] != NULL){
                    std::string s = recv_msg;
                    th(s);
                }
            }
        }
        catch(std::exception){

        }
    }
}


enum{
    T_ROLL,
    T_PITCH,
    T_YAW,
    T_LAT,
    T_LNG,
    T_ALT,
    T_GPSST,
    T_GPSSATCNT,
    T_GSPEED,
    T_VSPEED,
    T_AX,
    T_AY,
    T_AZ,
    T_GX,
    T_GY,
    T_GZ,
    T_MX,
    T_MY,
    T_MZ,
    T_MODE,
    T_BATT,
    T_BATTREM,
    T_BATTCUR,
    T_ARMED,
    T_SONAR,
    T_GROLL,
    T_GPITCH,
    T_GYAW,
    T_LANDGEAR,
    T_LAND,
    T_RESERVED
};

#endif // TCLIENT_H
