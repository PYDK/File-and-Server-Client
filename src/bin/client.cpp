#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <netdb.h>
#include <netinet/in.h>
#include "pack109.hpp"

#define KEY 0x2A //42
using namespace std;

vec encrypt(vec u_bytes) {
    vec e_bytes = vector<u8>(u_bytes.size());
    for(int i = 0; i < u_bytes.size(); i++) {
        e_bytes[i] = u_bytes[i] ^ KEY;
    }
    return e_bytes;
}

int read(Request fileR, int sockfd) {
    printf("---------------------------------------------\n");
    printf("Receiving file \"%s\".\n", fileR.name.c_str());

    /* Send message to the server */
    vec file = pack109::serialize(fileR);
    vec msg_bytes = encrypt(file);
    printf("Sending %ld bytes.\n", msg_bytes.size());
    int n = write(sockfd, (unsigned char*)&msg_bytes[0], msg_bytes.size());
    printf("Message Sent.\n");
    if (n < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }

    vec received(200000, 0);
    received.resize(200000);
    n = read(sockfd, (unsigned char*)&received[0], received.size());
    printf("Received %d bytes\n", n-1);
    if(n == 1) {
        printf("No bytes were received.\n");
        return 0;
    } else if (n <= 0) {
        printf("ERROR reading from socket.\n");
        exit(1);
    }
    received.resize(n, 0);
    vec u_file = encrypt(received);
    File file1 = pack109::deserialize_file(u_file);
    char* directory = (char*)malloc(file1.name.length() + 10);
    bzero(directory, file1.name.length() + 12);
    strcat(directory, "./received/");
    strcat(directory, file1.name.c_str());
    FILE* file2 = fopen(directory, "w+b");
    vec bytes = file1.bytes;
    if (file2 != NULL) {
        fseek(file2, 0, SEEK_SET);
        for (int i = 0; i < bytes.size(); i++) {
            fputc(bytes[i], file2);
        }
    }
    fclose(file2);
    printf("Saved file in: \"%s\"\n", directory);

    free(directory);
    return -1;
}

void send(char* path, File fileSent, int sockfd) { //tutorialspoint.com
    printf("---------------------------------------------\n");
    printf("Sending file \"%s\".\n", path);
    vec file = pack109::serialize(fileSent);
    vec e_bytes = encrypt(file);
    printf("Read file: %ld bytes\n", fileSent.bytes.size());
    /* Send message to the server */
    printf("Sending %ld bytes.\n", e_bytes.size());
    int n = write(sockfd, (unsigned char*)&e_bytes[0], e_bytes.size()+1);

    if (n < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }
    printf("Done.\n");
}

int main(int argc, char** argv) {
    char default_address[20] = "localhost";
    char default_port[10] = "8081";
    char* address = default_address;
    char* port = default_port;
    char* token = NULL, *argument = NULL, *path = NULL;
    for(int i = 1; i < argc; i++) {
        argument = (char*)malloc(strlen(argv[i]));
        strcpy(argument, argv[i]);
        if(strcmp(argument, "--hostname") == 0 && ++i < argc) {
            argument = (char*)realloc(argument, strlen(argv[i]));
            strcpy(argument, argv[i]);
            token = strtok(argument, ":");
            if(token == NULL) {
                printf("Invalid Hostname. Please type in this format. ip_address:port_number\n");
                exit(1);
            } else if(token[0] == argv[i][0]) {
                strcpy(address, token);
            } else {
                strcpy(port, token);
            }
            if((token = strtok(NULL, ":")) != NULL) {
                strcpy(port, token);
            }
            if(address == NULL) {
                address = default_address;
            }
            if(port == NULL) {
                port = default_port;
            }
            if (argument != NULL) {
                free(argument);
            }
        }
        else {
            printf("Invalid Arguments.\n");
            exit(1);
        }
    }

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent* server;

    portno = atoi(port);

    /* Create a socket point */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    server = gethostbyname(address);

    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    /* Now connect to the server */
    printf("Connecting to %s:%s\n", address, port);
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        exit(1);
    }
    printf("Successfully connected to server.\n");

    int reply = -1;
    do {
        printf("---------------------------------------------\n");
        printf("Would you like to:\n1:Send file\n2:Request file\n3:Quit Program\nChoose 1, 2, or 3.\n");
        cin >> reply;
        string filename = "", s_path;
        FILE* file1;
        vec bytes;
        File fileSent;
        int count = 0;
        Request r;
        switch(reply) {
            case 1:
                printf("Please type the path of the file.\n");
                cin >> s_path;
                path = (char*)malloc(s_path.length());
                strcpy(path, s_path.c_str());
                for (int j = strlen(path) - 1; j >= 0; j--) {
                    if (path[j] == '/' || path[j] == '\\') {
                        break;
                    }
                    count++;
                }
                for (int j = strlen(path) - count; j < strlen(path); j++) {
                    filename += path[j];
                }
                if (access(path, R_OK) == 0) {
                    file1 = fopen(path, "rb");
                    if (file1 != NULL) {
                        fseek(file1, 0, SEEK_SET);
                        int byte = fgetc(file1);
                        while (byte != EOF) {
                            bytes.push_back((unsigned int)byte);
                            byte = fgetc(file1);
                        }
                    }
                    fclose(file1);
                    fileSent = { name: filename, bytes : bytes };
                    send(path, fileSent, sockfd);
                }
                break;
            case 2:
                printf("Please type the name of the file to be requested.\n");
                cin >> filename;
                r = {name: filename};
                read(r, sockfd);
                break;
            case 3:
                break;
            default:
                printf("Invalid input.\n");
                exit(1);
        }
    } while(reply != 3);

}