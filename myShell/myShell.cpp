// myShell.cpp : Simple Command Line Interpreter
// Names : Oliver Escalante, Jacob Berry, Nicholas Haga
// Group : 1

#include <Windows.h>
#include <iostream>
#include <vector>

//Thread function
DWORD WINAPI SendCommand(LPVOID Param) {
	std::system((const char*)Param);
	return 0;
}

int main()
{
	std::cout << "Welcome to myShell" << "\n";

	std::vector<const char*> validCMD{
		"dir", "help", "vol", 
		"path", "tasklist", "notepad", 
		"echo", "color", "ping"};

	DWORD ThreadID;
	HANDLE ThreadHandle;

	while (true)
	{
		char userIn[41] = "";
		char* token = NULL;
		char* ctxt = NULL;
		std::vector<char*> tknArgument;
		std::cout << "\n==> ";
		fgets(userIn, 40, stdin);
		std::cout << '\n';

		// Pull command from user input
		token = strtok_s(userIn, " \n", &ctxt);

		// Store the pulled token into tknArgument and pull the next argument
		while (token != NULL) {
			tknArgument.push_back(_strdup(token));
			memset(token, 0, strlen(token));
			token = strtok_s(NULL, " ", &ctxt);
		};

		if (tknArgument[0] != NULL) {
			bool valid = false;

			// Ensure thread will only be created from a set of commands
			for (const char* vCmd : validCMD) {
				if (strcmp(vCmd, tknArgument[0]) == 0) {
					valid = true;
				}
			}

			if (strcmp("exit", tknArgument[0]) == 0){
				break;
			};

			// De-tokenize the command so system() will run
			for (char* str : tknArgument) {
				strcat_s(userIn, 40, str);
				if (strcmp(str, tknArgument.back()) != 0) {
					strcat_s(userIn, 40, " ");
				}
			}

			if (valid) {
				ThreadHandle = CreateThread(
					NULL,
					0,
					SendCommand,
					&userIn,
					0,
					&ThreadID
				);

				WaitForSingleObject(ThreadHandle, INFINITE);

				CloseHandle(ThreadHandle);
			}
		}
	}

	std::cout << "\nThank you for using myShell!";
	return 0;
}