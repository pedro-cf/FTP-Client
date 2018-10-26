#include "socket_api.h"
#include <stdlib.h>

int readMessage(int socket1, char *message, unsigned short hasFTPprotocol, unsigned short print) {
	bzero(message, READ_SIZE);
	int no_bytes=0, cur_read_bytes=0;
	unsigned short first = 1;
	char *end_code = (char *) malloc(sizeof(char)*5);
	char *code;
	while (1) {
		cur_read_bytes = 0;
		if ((cur_read_bytes = read(socket1, message+strlen(message), READ_SIZE)) < 0) {
			printf("< No message received (timed out after %d seconds).\n\n", READ_TIMEOUT);
			return 0;
		}
		no_bytes += cur_read_bytes;
		
		if (!hasFTPprotocol || strlen(message) < 4)
			break;

		if (first) {
			code = strndup(message, 4);
			code[3] = ' ';
			if (findCharCount(message, '\n') > 1)
				sprintf(end_code, "\n%s", code);
			else
				sprintf(end_code, "%s", code);
			first = 0;
		}

		if (strstr(message, end_code))
			break;
	}

	if (print)
		printf("< %s", message);
	return no_bytes;
}

int writeMessage(int socket1, char *message, unsigned short print) {

	if (write(socket1, message, strlen(message)) < 0) {
		printf("ERROR: writing to socket failure\n");
		return 1;
	}
	if (print)
		printf("> %s", message);

	return 0;
}

int setReadTimeout(int sockfd, int time) {
	struct timeval timeout;
	timeout.tv_sec = time;
	timeout.tv_usec = 0;
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeout, sizeof(timeout)) < 0) {
		perror("ERROR: setsockopt failed");
		return 1;
	}

	return 0;
}

void handleServerAddress(struct sockaddr_in* serv_addr, struct hostent* server, int portno) {
	bzero((char*) &*serv_addr, sizeof(*serv_addr));
	serv_addr->sin_family = AF_INET;
	bcopy((char*) server->h_addr, (char*) &serv_addr->sin_addr.s_addr, server->h_length);
	serv_addr->sin_port = htons(portno);
}

int connectToServer(char* hostName_str, char* port_str) {
	int port;
	struct hostent* host;
	int socket1;
	struct sockaddr_in serverAddress;

	// Get host
	host = gethostbyname(hostName_str);
	if (host == NULL ) {
		perror("ERROR: no such host");
		return -1;
	}

	// Get port number
	port = atoi(port_str);
	if (port < 0 || port > 65535) {
		perror("ERROR: no such port");
		return -1;
	}

	// Open socket
	socket1 = socket(AF_INET, SOCK_STREAM, 0);
	if (socket1 < 0) {
		perror("ERROR: opening socket");
		return -1;
	}

	// Timeout settings
	if (setReadTimeout(socket1, READ_TIMEOUT)) {
		perror("ERORR: failed to set socket timeout time");
		return -1;
	}

	// Handle server address
	handleServerAddress(&serverAddress, host, port);

	printf("\nConnecting to %s:%d...",
			inet_ntoa(*((struct in_addr *) host->h_addr)), port);
	fflush(stdout);

	// Connect to server
	if (connect(socket1, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) < 0) {
		perror("Failed to connect");
		return -1;
	} else {
		printf("Connected!\n");
	}

	return socket1;
}
