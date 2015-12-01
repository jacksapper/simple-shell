#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_CMD_LENGTH 		128
#define MAX_PARAMS 			16
int run(char** param);
void get(char * command, char** param);
int main()
{
	char* param[1 + MAX_PARAMS];
	char cmd[1 + MAX_CMD_LENGTH];
	while(1)
	{
		char* uname = getenv("USER");

		printf("%s@topgnushell -->", uname);
		if(fgets(cmd, sizeof(cmd), stdin) == NULL) break;
		cmd[strlen(cmd)-1] = (cmd[strlen(cmd)-1] == '\n') ? '\0' : cmd[strlen(cmd)-1];
		get(cmd, param);
		if(strcmp(param[0], "exit") == 0) break;
		if(strcmp(param[0], "cd") == 0)
			if(chdir(param[1]) == -1) printf("Line 36");
		if(run(param) == 0) break;
	}
	return 0;
}
int run(char** param)
{
	pid_t p = fork();
	if(p < 0)
	{
		printf("cannot fork process");
		return 1;
	}
	else if (p == 0)
	{
		execvp(param[0], param);
		return 0;
	}
	else
	{
		int c;
		waitpid(p, &c, 0);
		return 1;
	}
}
void get(char* command, char** param)
{
	int i;
	for(i = 0; i < MAX_PARAMS; i++)
	{
		param[i] = strsep(&command, " ");
		if(param[i] == NULL) break;
	}
}
