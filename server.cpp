#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib");

#define PORT 8000
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData; 
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];

    //Initialize Winsock
    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
    {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return 1;
    }

    //Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Could not create socket." << std::endl;
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family =AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; //Accept connections on any IP address
    serverAddr.sin_port = htons(PORT);

    //Bind
    if(bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Bind failed. " << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    //Listen
    listen(serverSocket, 3);
    std::cout << "server listening on port " << PORT << "..." << std::endl;

    //Accept incoming connection
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
    if(clientSocket == INVALID_SOCKET){
        std::cerr << "Accept failed. " << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    } else {
        std::cout << "Client connected!" << std::endl; 
    }

    //Receive Data
    int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if(bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; //Null terminate the string
        std::cout << "Received: " << buffer << std::endl;

    //Send response
    const char* response = "Message received";
    send(clientSocket, response, strlen(response), 0);
    }

    //Cleanup
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0; 
}