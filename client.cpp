#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib");

#define SERVER_IP "127.0.0.1"
#define PORT 8000
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    
    //Initialize Winsock
    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
    {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return 1;
    }

    //Create socket
    SOCKET serverSocket; 
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Could not create socket." << std::endl ;
        WSACleanup();
        return 1;
    }

    //Setup server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    //Connect to server
    if(connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr <<"Connection Failed." << std::endl; 
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    } else{
        std::cout << "Connected to server!" << std::endl; 
    }
    
    //Send data
    const char* message = "Hello from client!";
    send(clientSocket, message, strlen(message), 0);

    //Receive response
    int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if(bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; //Null terminate the string
        std::cout << "Server response: " << buffer << std::endl;
    }

    //cleanup
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}