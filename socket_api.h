#ifndef __SOCKET_API_H
#define __SOCKET_API_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define READ_TIMEOUT 1
#define READ_SIZE 1024

/**
 * @brief Reads a message from a server via-socket
 *
 * @param sockfd socket to read from
 * @param message buffer to pass message to
 * @param hasFTPprotocol if set to 0 will ignore server message code protocol
 * @param print if set to non-zero will print message to terminal
 *
 * @return number of bytes read upon sucess and -1 upon failure
 */
int readMessage(int socket1, char *message, unsigned short hasFTPprotocol, unsigned short print);

/**
 * @brief Writes a message to a server via-socket
 *
 * @param sockfd socket to write to
 * @param message buffer with message to be sent
 * @param print if set to non-zero will print message to terminal
 */
int writeMessage(int socket1, char *message, unsigned short print);

/**
 * @brief Sets the timeout time of a socket
 *
 * @param sockfd socket to be edited
 * @param time timeout time
 *
 * @return Return 0 upon success and 1 upon failure
 */
int setReadTimeout(int sockfd, int time);

/**
 * @brief Handles a server address
 *
 * @param server struct hostent* with server address
 * @param serv_addr server address to be handled
 * @param portno port number
 */
void handleServerAddress(struct sockaddr_in* serv_addr, struct hostent* server, int portno);

/**
 * @brief Connects to a server by hostname and port
 *
 * @param hostName_str string of the hostname
 * @param port_str string of the port
 *
 * @return on success, a file descriptor for the socket is returned. On error, -1 is returned
 */
int connectToServer(char* hostName_str, char* port_str);

#endif /* __SOCKET_API */
