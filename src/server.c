#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    
     if (argc<3){
        printf ("Please enter the server IP address"
                " and PORT\n");
        printf ("USAGE: %s IPv4 Port\n", 
                argv[0]);
        exit(1);
    }

    char tip[16] = {0};
    strcpy(tip, argv[1]);
    
    int port = atoi(argv[2]);
    
    
    WSADATA wsa;
    SOCKET sock, client_sock;
    struct sockaddr_in addr;
    int addrlen = sizeof(addr);

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed to initialize Winsock. Error Code: %d\n", WSAGetLastError());
        return EXIT_FAILURE;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Could not create socket : %d\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }

    addr.sin_family=AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(tip);

    if(bind(sock, (struct sockaddr*)&addr, sizeof(addr))== SOCKET_ERROR){
        printf("Bind failed. Error Code: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return EXIT_FAILURE;
    }

    if ( listen(sock,5)<0){
        printf("Listen failed. Error Code: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return EXIT_FAILURE;
    }

    while(1){
        if ( client_sock = accept(sock,(struct sockaddr*)&addr, &addrlen )== INVALID_SOCKET ){
            printf("Accept failed. Error Code: %d\n", WSAGetLastError());
            continue; // Continue listening for the next connection
        }
        printf("Connection accepted from %s:%d\n", 
            inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
    }
    

    return 0;
}