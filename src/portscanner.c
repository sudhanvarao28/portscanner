#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

typedef struct {
    char* ip;
    int port;
} thread_args_t;

DWORD WINAPI port_scanner(LPVOID lpParam) {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in addr;
    thread_args_t* args = (thread_args_t*)lpParam;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed to initialize Winsock. Error Code : %d\n", WSAGetLastError());
        return 1;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Could not create socket : %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(args->port);
    addr.sin_addr.s_addr = inet_addr(args->ip);

    int result = connect(sock, (struct sockaddr*)&addr, sizeof(addr));

    if (result == 0) {
        printf("\033[0;32mPORT %d is open for IP: %s\033[0m\n", args->port, args->ip);
    } else {
        printf("\033[0;31mPORT %d is not open for IP: %s\033[0m\n", args->port, args->ip);
    }

    closesocket(sock);
    WSACleanup();

    free(args);  // Free allocated memory for thread arguments
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        printf("Please enter the server IP address, range of ports to be scanned and number of max threads\n");
        printf("USAGE: %s IPv4 First_Port Last_Port Max_threads\n", argv[0]);
        exit(1);
    }

    char tIP[16] = {0};
    strcpy(tIP, argv[1]);
    int start_port = atoi(argv[2]);
    int end_port = atoi(argv[3]);

    int num_ports = end_port - start_port + 1;
    int max_threads = atoi(argv[4]);
    HANDLE* threads = malloc(max_threads * sizeof(HANDLE));;

    int thread_count = 0;

    for (int port = start_port; port <= end_port; port++) {
        thread_args_t* args = malloc(sizeof(thread_args_t));
        args->ip = tIP;
        args->port = port;

        threads[thread_count] = CreateThread(
            NULL,
            0,
            port_scanner,
            args,
            0,
            NULL
        );

        if (threads[thread_count] == NULL) {
            printf("Error creating thread for port %d\n", port);
            return 1;
        }
        
        thread_count++;

        if (thread_count == max_threads){
            WaitForMultipleObjects(thread_count, threads, TRUE, INFINITE);
        

            for (int i = 0; i < thread_count; i++) {
                CloseHandle(threads[i]);
            }

            thread_count = 0;
        }

    }

    WaitForMultipleObjects(num_ports, threads, TRUE, INFINITE);

    for (int i = 0; i < num_ports; i++) {
        CloseHandle(threads[i]);
    }

    return 0;
}
