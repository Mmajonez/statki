#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "Game.cpp"

#define BUF_SIZE 1024
#define NUM_THREADS 200

struct thread_data_t
{
    int gameID;
    int socket;
    char buf[20];

};


void* ThreadBehavior(void* t_data)
{   
    struct thread_data_t* th_data = (struct thread_data_t*)t_data;
    Game g = games[th_data->gameID];
    bool start = true;

    while (1) {
        if (start == true) {
            memset(th_data->buf, 0, sizeof(th_data->buf));
            if ((read = read(th_data->socket, th_data->buf, sizeof(th_data->buf))) > 0)
            {
                if (convertToString(th_data->buf1, read - 1) == "p1") {
                    g.setShips(g.board1);
                }
                else if (convertToString(th_data->buf1, read - 1) == "p2") {
                    g.setShips(g.board2);
                }
            }
            
            memset(th_data->buf, 0, sizeof(th_data->buf));
            strcpy(th_data->buf, "1");
            write(th_data->socket, th_data->buf, sizeof(th_data->buf));
        }
        else {
            string s;
            cin >> s;
            int read;
            
            memset(th_data->buf, 0, sizeof(th_data->buf));
            while (1) {
                if ((read = read(th_data->socket, th_data->buf, sizeof(th_data->buf))) > 0)
                {
                    if (convertToString(th_data->buf1, read - 1) == '9') {
                        memset(th_data->buf, 0, sizeof(th_data->buf));
                        strcpy(th_data->buf, s);
                        write(th_data->socket, th_data->buf, sizeof(th_data->buf));
                    }
                    else if{
                        if (convertToString(th_data->buf1, read - 1) == '2') {
                            cout << "You hit the ship!" << endl;
                        }
                    }
                    else if{
                        if (convertToString(th_data->buf1, read - 1) == '3') {
                            cout << "You missed!" << endl;
                        }
                    }
                    else if{
                        if (convertToString(th_data->buf1, read - 1) == '4') {
                            cout << "You sunk the ship!" << endl;
                        }
                    }
                    else if{
                        if (convertToString(th_data->buf1, read - 1) == '5') {
                            cout << "You already tried this! Set another position" << endl;
                            memset(th_data->buf, 0, sizeof(th_data->buf));
                            strcpy(th_data->buf, s);
                            write(th_data->socket, th_data->buf, sizeof(th_data->buf));
                            continue;
                        }
                    }
                    else if{
                        if (convertToString(th_data->buf1, read - 1) == '6') {
                            cout << "You won!" << endl;
                        }
                    }
                    else if{
                        if (convertToString(th_data->buf1, read - 1) == '7') {
                            cout << "You lose!" << endl;
                        }
                    }
                    break;
                }
            }
        }
    }

    pthread_exit(NULL);
}


void handleConnection(int connection_socket_descriptor) {

    int create_result = 0;

    pthread_t thread1;
    
    struct thread_data_t t_data;
    t_data.socket = connection_socket_descriptor;

    create_result = pthread_create(&thread1, NULL, ThreadBehavior, (void*)&t_data);
    if (create_result) {
        printf("Błąd przy próbie utworzenia wątku. Kod błędu: % d\n", create_result);
            exit(-1);
    }

    //TODO (przy zadaniu 1) odbieranie -> wyĹwietlanie albo klawiatura -> wysyĹanie
}


int main(int argc, char* argv[])
{
    int connection_socket_descriptor;
    int connect_result;
    struct sockaddr_in server_address;
    struct hostent* server_host_entity;

    if (argc != 3)
    {
        fprintf(stderr, "Sposób użycia: %s server_name port_number\n", argv[0]);
        exit(1);
    }

    server_host_entity = gethostbyname(argv[1]);
    if (!server_host_entity)
    {
        fprintf(stderr, "%s: Nie można uzyskać adresu IP serwera.\n", argv[0]);
        exit(1);
    }

    connection_socket_descriptor = socket(PF_INET, SOCK_STREAM, 0);
    if (connection_socket_descriptor < 0)
    {
        fprintf(stderr, "%s: Błąd przy próbie utworzenia gniazda.\n", argv[0]);
            exit(1);
    }

    memset(&server_address, 0, sizeof(struct sockaddr));
    server_address.sin_family = AF_INET;
    memcpy(&server_address.sin_addr.s_addr, server_host_entity->h_addr, server_host_entity->h_length);
    server_address.sin_port = htons(atoi(argv[2]));

    connect_result = connect(connection_socket_descriptor, (struct sockaddr*)&server_address, sizeof(struct sockaddr));
    if (connect_result < 0)
    {
        fprintf(stderr, "%s: Błąd połączenia z serwerem(% s: % i).\n", argv[0], argv[1], atoi(argv[2]));
            exit(1);
    }

    handleConnection(connection_socket_descriptor);

    close(connection_socket_descriptor);
    return 0;

}