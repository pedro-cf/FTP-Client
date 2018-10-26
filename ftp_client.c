#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "socket_api.h"
#include "rcom_util.h"

#define PRINT_MESSAGES 1
#define PRINT_DATA 0

int handlePASV(char *buffer_in) {

	int i, j, locs[5];
	for (i = 0, j = 0; i < strlen(buffer_in); i++)
		if (buffer_in[i] == ',')
			locs[j++] = i+1;


	char *p1, *p2;

	//printf("sizeP1=%d\n", locs[4]-locs[3]-1);
	//printf("sizeP2=%d\n", (int)strlen(buffer_in) -3 -locs[4]);

	p1=strndup(buffer_in+locs[3], locs[4]-locs[3]-1);
	p2=strndup(buffer_in+locs[4], strlen(buffer_in) -3 -locs[4]);

	int dataport = atoi(p1)*256 + atoi(p2);
	printf("\n- Data Port = %s*256+%s = %d\n", p1, p2, dataport);

	return dataport;
}

void usage_error(char* argv[]) {
	printf( "ERROR: invalid arguments\n"
			"Usage: %s ftp://[<user>:<password>@]<host>/<url-path>\n",
			argv[0]);
	exit(1);
}

int main(int argc, char *argv[]) {

	char *user = "anonymous";
	char *pass = "";
	char *host;
	char *path;
	char *filename;

	// Check arguments
	if (argc != 2) {
		usage_error(argv);
	}



	char *str1 = strndup(argv[1], 6);
	//printf("link=%s\n", str1);
	if (strcmp(str1, "ftp://") != 0) {
		usage_error(argv);
	}

	int _a = findChar(argv[1], '@');
	if (_a != -1) { // ftp://<user>:<password>@<host>/<url-path>

		int user_i = 6;
		int pass_i = findLastChar(argv[1], ':') + 1;
		int host_i = findChar(argv[1], '@') + 1;
		int path_i = findChar_ignore_2(argv[1], '/') + 1;
		int file_i = findLastChar(argv[1], '/') + 1;

																		//printf("\n\n%s\n", argv[1]);
		user = strndup(argv[1]+user_i, pass_i - user_i -1); 			//printf("user=%s\n", user);
		pass = strndup(argv[1]+pass_i, host_i - pass_i -1); 			//printf("pass=%s\n", pass);
		host = strndup(argv[1]+host_i, path_i - host_i -1); 			//printf("host=%s\n", host);
		path = strndup(argv[1]+path_i, file_i - path_i); 				//printf("path=%s\n", path);
		filename = strndup(argv[1]+file_i, strlen(argv[1]) - file_i);	//printf("file=%s\n", filename);

		if (!strlen(user) || !strlen(pass) || !strlen(host) || !strlen(path) || !strlen(filename))
			usage_error(argv);

	} else { // ftp://<host>/<url-path>

		int host_i = 6;
		int path_i = findChar_ignore_2(argv[1], '/') + 1;
		int file_i = findLastChar(argv[1], '/') + 1;
																		//printf("\n\n%s\n", argv[1]);
																		//printf("user=%s\n", user);
																		//printf("pass=%s\n", pass);
		host = strndup(argv[1]+host_i, path_i - host_i -1); 			//printf("host=%s\n", host);
		path = strndup(argv[1]+path_i, file_i - path_i); 				//printf("path=%s\n", path);
		filename = strndup(argv[1]+file_i, strlen(argv[1]) - file_i);	//printf("file=%s\n", filename);
	
		if (!strlen(host) || !strlen(path) || !strlen(filename))
			usage_error(argv);

	}

	int mainSocket, dataSocket;

	char message_out[255];
	char message_in[READ_SIZE];
	char data_in[READ_SIZE];

	// Connect to FTP server on port 21
	mainSocket = connectToServer(host, "21");
	if (mainSocket == -1) exit(1);

	// Read connection response message from server
	readMessage(mainSocket, message_in, 1, PRINT_MESSAGES);

	// Write USER message to server
	sprintf(message_out, "user %s\n", user);
	writeMessage(mainSocket, message_out, PRINT_MESSAGES);

	readMessage(mainSocket, message_in, 1, PRINT_MESSAGES);

	// Write PASS message to server
	bzero(message_out, 255);
	sprintf(message_out, "pass %s\n", pass);
	writeMessage(mainSocket, message_out, PRINT_MESSAGES);

	readMessage(mainSocket, message_in, 1, PRINT_MESSAGES);

	// Write CWD <PATH> message to server
	bzero(message_out, 255);
	sprintf(message_out, "cwd %s\n", path);
	writeMessage(mainSocket, message_out, PRINT_MESSAGES);


	readMessage(mainSocket, message_in, 1, PRINT_MESSAGES);

	// Write PASV message to server
	writeMessage(mainSocket, "pasv\n", PRINT_MESSAGES);

	readMessage(mainSocket, message_in, 1, PRINT_MESSAGES);

	// Obtain the data port out of the PASV reponse
	int dataport = handlePASV(message_in);
	char dataport_str[8];
	sprintf(dataport_str, "%d", dataport);

	// Connect to the data port
	dataSocket = connectToServer(host, dataport_str);
	if (dataSocket == -1) exit(1);

	// Write RETR message to server
	bzero(message_out, 255);
	sprintf(message_out, "retr %s\n", filename);
	writeMessage(mainSocket, message_out, PRINT_MESSAGES);

	// Read the number of bytes beeing received
	readMessage(mainSocket, message_in, 1, PRINT_MESSAGES);

	int no_bytes_i = findLastChar(message_in, '(') + 1;
	int no_bytes_f = findLastChar(message_in, ' ') + 1;
	char *no_bytes_str = strndup(message_in + no_bytes_i, no_bytes_f - no_bytes_i); //printf("number of bytes=%s\n", no_bytes_str);
	int no_bytes = atoi(no_bytes_str);

//	char fullfilepath[255];
//	bzero(fullfilepath, 255);
//	sprintf(fullfilepath, "/home/ %s", filename)

	char newFileName[255];
	sprintf(newFileName, "%s", filename);

	while (1) {
		if (fileExists(newFileName)) {
			char tmp[255];
			sprintf(tmp, "%s", newFileName);
			sprintf(newFileName, "1%s", tmp);
		}
		else
			break;
	}

	FILE *file = fopen(newFileName, "w");
	if (file == NULL) {
		perror("file error");
		exit(1);
	}
	
	printf("\nDownloading %s...", filename);

	printf("\n                      \n");

	int no_bytes_left = no_bytes;
	int no_bytes_read = 0;
	while(no_bytes_left > 0) {
		if (no_bytes_read > 0)
			fwrite(data_in, 1, MIN(no_bytes_left, no_bytes_read), file); //mimum because we dont want to write excess bytes
		no_bytes_left -= no_bytes_read;
		no_bytes_read = 0;

		float a =  no_bytes - no_bytes_left;
		printf("\rDownload progress: %d%%", (int)((a)/(float)no_bytes*100));
		if((no_bytes_read =readMessage(dataSocket, data_in, 0, PRINT_DATA)) < 0) {
			perror("something went wrong reading file data. stopping otherwise file corrupted");
			fclose(file);
			remove(newFileName);
			return 1;
		}
	}
	fclose(file);
	if (no_bytes_left)
		printf("\n\nFailed to download file %s\n\n", filename);
	else
		printf("\n\nFile %s successfuly downloaded.\n\n", filename);
	close(mainSocket);
	close(dataSocket);
	return 0;

}
