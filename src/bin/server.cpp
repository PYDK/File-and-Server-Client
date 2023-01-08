#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "pack109.hpp"
#include "hashmap.hpp"

#define KEY 0x2A //42
using namespace std;

vec encrypt(vec u_bytes) {
    vec e_bytes = vector<u8>(u_bytes.size());
    for (int i = 0; i < u_bytes.size(); i++) {
        e_bytes[i] = u_bytes[i] ^ KEY;
    }
    return e_bytes;
}

int start_serv(char* address, char* port) {
    int sockfd, sockfd2, newsockfd, portno, portno2, clilen;
    struct sockaddr_in serv_addr, cli_addr;
    int  n;
    hashmap::HashMap fileserver;
    portno = atoi(port);
    portno2 = portno + 1;

    int pid = fork();
    if(pid < 0) {
        perror("ERROR on fork");
        exit(1);
    }
    if(pid == 0) {
        /* First call to socket() function */
        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if (sockfd < 0) {
            perror("ERROR opening socket");
            exit(1);
        }

        /* Initialize socket structure */
        bzero((char*)&serv_addr, sizeof(serv_addr));
        

        serv_addr.sin_family = AF_INET;
        if (strcmp(address, "localhost") == 0) {
            serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        } else {
            serv_addr.sin_addr.s_addr = inet_addr(address);
        }
        serv_addr.sin_port = htons(portno);

        /* Now bind the host address using bind() call.*/
        if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
            perror("ERROR on binding");
            exit(1);
        }

        /* Now start listening for the clients, here process will
           * go in sleep mode and will wait for the incoming connection
        */
        printf("Server listening on: %s:%s\n", address, port);
        listen(sockfd, 5);
        clilen = sizeof(cli_addr);

        /* Accept actual connection from the client */
        while(1) {
            newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, (unsigned int*)&clilen);
            if (newsockfd < 0) {
                perror("ERROR on accept");
                exit(1);
            }

            char* client_ip = inet_ntoa(cli_addr.sin_addr);
            printf("---------------------------------------------\n");
            printf("New Connection: %s:%d\n", client_ip, cli_addr.sin_port);

            int tid = fork();
            if(tid < 0) {
                perror("ERROR on fork");
                exit(1);
            }
            if(tid == 0) {
                close(sockfd);
            } else if(tid > 0) {
                close(newsockfd);
                continue;
            }
            while (newsockfd != -1) {
                vec received(200000, 0);
                /* If connection is established then start communicating */
                n = read(newsockfd, (unsigned char*)&received[0], received.capacity() - 1);

                printf("---------------------------------------------\n");
                printf("Reading %d bytes\n", n - 1);
                if (n <= 0) {
                    printf("Connection with %s:%d closed.\n", client_ip, cli_addr.sin_port);
                    exit(1);
                }
                received.resize(n, 0);
                vec u_bytes = encrypt(received);
                vec s_type;
                int index = 2;
                if (u_bytes[index] == 0xaa) {
                    s_type.push_back(u_bytes[index++]);
                    int type_size = u_bytes[index];
                    s_type.push_back(u_bytes[index++]);
                    for (int i = 0; i < type_size; i++) {
                        s_type.push_back(u_bytes[index + i]);
                    }
                }
                string type = pack109::deserialize_string(s_type);

                int send_soc;
                struct sockaddr_in con_addr;
                send_soc = socket(AF_INET, SOCK_STREAM, 0);

                bzero((char*)&con_addr, sizeof(con_addr));
                con_addr.sin_family = AF_INET;
                con_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
                con_addr.sin_port = htons(portno2);

                if (connect(send_soc, (struct sockaddr*)&con_addr, sizeof(con_addr)) < 0) {
                    perror("ERROR connecting");
                    exit(1);
                }

                if (type.compare("File") == 0) {
                    n = write(send_soc, (unsigned char*)&received[0], received.size() + 1);
                    close(send_soc);
                } else if (type.compare("Request") == 0) {
                    n = write(send_soc, (unsigned char*)&received[0], received.size() + 1);
                    received.clear();
                    received.resize(200000, 0);
                    n = read(send_soc, (unsigned char*)&received[0], received.size() + 1);
                    received.resize(n);
                    n = write(newsockfd, (unsigned char*)&received[0], received.size() + 1);
                    close(send_soc);
                }
                s_type.clear();
            }
            printf("Terminating connection with %s:%d\n", client_ip, cli_addr.sin_port);
            close(newsockfd);
            printf("Connection terminated.\n");
        }
    } else {
        fileserver = hashmap::HashMap();
        /* First call to socket() function */
        sockfd2 = socket(AF_INET, SOCK_STREAM, 0);

        if (sockfd2 < 0) {
            perror("ERROR opening socket");
            exit(1);
        }

        /* Initialize socket structure */
        bzero((char*)&serv_addr, sizeof(serv_addr));

        serv_addr.sin_family = AF_INET;
        if (strcmp(address, "localhost") == 0) {
            serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        } else {
            serv_addr.sin_addr.s_addr = inet_addr(address);
        }
        serv_addr.sin_port = htons(portno2);

        /* Now bind the host address using bind() call.*/
        if (bind(sockfd2, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
            perror("ERROR on binding");
            exit(1);
        }

        /* Now start listening for the clients, here process will
           * go in sleep mode and will wait for the incoming connection
        */
        printf("Server listening on: %s:%d\n", address, portno2);
        listen(sockfd2, 5);
        clilen = sizeof(cli_addr);

        /* Accept actual connection from the client */
        while(1) {
            newsockfd = accept(sockfd2, (struct sockaddr*)&cli_addr, (unsigned int*)&clilen);
            if (newsockfd < 0) {
                perror("ERROR on accept");
                exit(1);
            }

            vec received(200000, 0);
            /* If connection is established then start communicating */
            n = read(newsockfd, (unsigned char*)&received[0], received.capacity() - 1);

            if (n <= 0) {
                printf("ERROR reading from socket.\n");
                exit(1);
            }
            received.resize(n, 0);
            vec u_bytes = encrypt(received);
            vec s_type;
            int index = 2;
            if (u_bytes[index] == 0xaa) {
                s_type.push_back(u_bytes[index++]);
                int type_size = u_bytes[index];
                s_type.push_back(u_bytes[index++]);
                for (int i = 0; i < type_size; i++) {
                    s_type.push_back(u_bytes[index + i]);
                }
            }
            string type = pack109::deserialize_string(s_type);
            if (type.compare("File") == 0) {
                File f_received = pack109::deserialize_file(u_bytes);
                printf("Received File: \"%s\"\n", f_received.name.c_str());
                fileserver.insert(f_received.name, f_received);
                printf("File Stored.\n");
                close(newsockfd);
            } else if (type.compare("Request") == 0) {
                Request f_received = pack109::deserialize_request(u_bytes);
                printf("Received %s: \"%s\"\n", type.c_str(), f_received.name.c_str());
                if (fileserver.contains(f_received.name)) {
                    File reqFile = fileserver.get(f_received.name);
                    vec file = pack109::serialize(reqFile);
                    vec e_bytes = encrypt(file);
                    printf("Sending \"%s\" -- %ld bytes\n", reqFile.name.c_str(), e_bytes.size());
                    n = write(newsockfd, (unsigned char*)&e_bytes[0], e_bytes.size() + 1);
                    printf("Message sent.\n");
                } else {
                    printf("File not Found.\n");
                }
            }
            close(newsockfd);
        }
        close(sockfd);

    }
    

    
    return 0;
}

int main(int argc, char** argv) {
    char default_address[20] = "localhost";
    char default_port[10] = "8081";
    char* address = default_address;
    char* port = default_port;
    char* token = NULL, *argument = NULL;
    for (int i = 1; i < argc; i++) {
        argument = (char*)malloc(strlen(argv[i]));
        strcpy(argument, argv[i]);
        if (strcmp(argument, "--hostname") == 0 && ++i < argc) {
            argument = (char*)realloc(argument, strlen(argv[i]));
            strcpy(argument, argv[i]);
            token = strtok(argument, ":");
            if (token == NULL) {
                printf("Invalid Hostname. Please type in this format. ip_address:port_number\n");
                exit(1);
            } else if (token[0] == argv[i][0]) {
                strcpy(address, token);
            } else {
                strcpy(port, token);
            }
            if ((token = strtok(NULL, ":")) != NULL) {
                strcpy(port, token);
            }
            if (address == NULL) {
                address = default_address;
            }
            if (port == NULL) {
                port = default_port;
            }
        } else {
            printf("Invalid Arguments.\n");
            exit(1);
        }

    }
    if (argument != NULL) {
        free(argument);
    }

    start_serv(address, port);
}