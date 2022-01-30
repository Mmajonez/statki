#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <vector>
#include "Game.cpp"

#define SERVER_PORT 1234
#define QUEUE_SIZE 300

//struktura zawierająca dane, które zostaną przekazane do wątku
struct thread_data_t
{
    int gameID;
    int player1;
    int player2;
    char buf[20];
};

string convertToString(char* buf, int size)
{
    string s = "";
    for (int i = 0; i < size; i++) {
        s = s + buf[i];
    }
    return s;
}


void* ThreadBehavior(void* t_data)
{
    pthread_detach(pthread_self());

    struct thread_data_t* th_data = (struct thread_data_t*)t_data;
    Game g;
    games[th_data->gameID] = g;
    bool start1 = false;
    bool strat2 = false;

    int read;

    memset(th_data->buf1, 0, sizeof(th_data->buf1));
    memset(th_data->buf2, 0, sizeof(th_data->buf2));
    strcpy(th_data->buf1, "p1");
    write(th_data->player1, th_data->buf1, sizeof(th_data->buf1));
    strcpy(th_data->buf2, "p2");
    write(th_data->player2, th_data->buf2, sizeof(th_data->buf2));

    memset(th_data->buf1, 0, sizeof(th_data->buf1));
    memset(th_data->buf2, 0, sizeof(th_data->buf2));
    while (1) {
        if (start1 == false) {

            if ((read = read(th_data->player1, th_data->buf1, sizeof(th_data->buf1))) > 0)
            {
                if (convertToString(th_data->buf1, read - 1) == '1') {
                    start1 = true;
                }
            }
            else
            {
                memset(th_data->buf2, 0, sizeof(th_data->buf2));
                strcpy(th_data->buf2, "8");
                write(th_data->player2, th_data->buf2, sizeof(th_data->buf2));
                goto endgame;
            }
        }
        if (start2 == false) {
            if ((read = read(th_data->player2, th_data->buf2, sizeof(th_data->buf2))) > 0)
            {
                if (convertToString(th_data->buf2, read - 1) == '1') {
                    start2 = true;
                }
            }
            else
            {
                memset(th_data->buf1, 0, sizeof(th_data->buf1));
                strcpy(th_data->buf1, "8");
                write(th_data->player1, th_data->buf1, sizeof(th_data->buf1));
                goto endgame;
            }
        }
        if (start2 == true && start1 == true) {

            do {
                memset(th_data->buf1, 0, sizeof(th_data->buf1));
                buf1[0] = '9';
                write(th_data->player1, th_data->buf1, sizeof(th_data->buf1));
                wait(10);
                memset(th_data->buf1, 0, sizeof(th_data->buf1));
                if ((read = read(th_data->player1, th_data->buf1, sizeof(th_data->buf1))) > 0)
                {
                    pair <int, int> coord = getCoord(convertToString(th_data->buf1, read - 1));
                    char out = g.shoot(g.board2, coord.first, coord.second);
                    memset(th_data->buf1, 0, sizeof(th_data->buf1));
                    buf1[0] = out;
                    write(th_data->player1, th_data->buf1, sizeof(th_data->buf1));
                    if (out == '6') {
                        memset(th_data->buf2, 0, sizeof(th_data->buf2));
                        buf2[0] = '7';
                        write(th_data->player2, th_data->buf2, sizeof(th_data->buf2));
                    }
                }
                else
                {
                    memset(th_data->buf1, 0, sizeof(th_data->buf1));
                    strcpy(th_data->buf1, "8");
                    write(th_data->player1, th_data->buf1, sizeof(th_data->buf1));
                    goto endgame;
                }
            } while (out != 5)

                do {
                    memset(th_data->buf2, 0, sizeof(th_data->buf2));
                    buf2[0] = '9';
                    write(th_data->player2, th_data->buf2, sizeof(th_data->buf2));
                    wait(10);
                    memset(th_data->buf2, 0, sizeof(th_data->buf2));
                    if ((read = read(th_data->player2, th_data->buf2, sizeof(th_data->buf2))) > 0)
                    {
                        pair <int, int> coord = getCoord(convertToString(th_data->buf2, read - 1));
                        char out = g.shoot(g.board1, coord.first, coord.second);
                        memset(th_data->buf2, 0, sizeof(th_data->buf2));
                        buf2[0] = out;
                        write(th_data->player2, th_data->buf2, sizeof(th_data->buf2));
                        if (out == '6') {
                            memset(th_data->buf1, 0, sizeof(th_data->buf1));
                            buf1[0] = '7';
                            write(th_data->player1, th_data->buf1, sizeof(th_data->buf1));
                        }
                    }
                    else
                    {
                        memset(th_data->buf2, 0, sizeof(th_data->buf2));
                        strcpy(th_data->buf2, "8");
                        write(th_data->player2, th_data->buf2, sizeof(th_data->buf2));
                        goto endgame;
                    }
                } while (out != 5)
        }
    }
endgame:
    ;

    pthread_exit(NULL);
}

//funkcja obsługująca połączenie z nowym klientem

void handleConnection(int connection_socket_descriptor1, int connection_socket_descriptor2, int ID) {
    int create_result = 0;

    pthread_t thread1;

    struct thread_data_t t_data;

    t_data.player1 = connection_socket_descriptor1;
    t_data.player1 = connection_socket_descriptor2;

    t_data.gameid = ID;
    create_result = pthread_create(&thread1, NULL, ThreadBehavior, (void*)&t_data);
    if (create_result) {
        printf("Błąd przy próbie utworzenia wątku, kod błędu: %d\n", create_result);
        exit(-1);
    }
}

int main(int argc, char* argv[])
{
    int connection_socket_descriptor;
    int connect_result;
    struct sockaddr_in server_address;
    struct hostent* server_host_entity;

    memset(&server_address, 0, sizeof(struct sockaddr));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);

    server_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_descriptor < 0)
    {
        fprintf(stderr, "%s: Błąd przy próbie utworzenia gniazda..\n", argv[0]);
        exit(1);
    }
    setsockopt(server_socket_descriptor, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse_addr_val, sizeof(reuse_addr_val));

    bind_result = bind(server_socket_descriptor, (struct sockaddr*)&server_address, sizeof(struct sockaddr));
    if (bind_result < 0)
    {
        fprintf(stderr, "%s: Błąd przy próbie dowiązania adresu IP i numeru portu do gniazda.\n", argv[0]);
        exit(1);
    }

    listen_result = listen(server_socket_descriptor, QUEUE_SIZE);
    if (listen_result < 0) {
        fprintf(stderr, "%s: Błąd przy próbie ustawienia wielkości kolejki.\n", argv[0]);
        exit(1);
    }

    int players = 0;
    int connection = 0;
    while (1)
    {
        connection_socket_descriptor = accept(server_socket_descriptor, NULL, NULL);
        if (connection_socket_descriptor < 0)
        {
            fprintf(stderr, "%s: Błąd przy próbie utworzenia gniazda dla połączenia.\n", argv[0]);
            exit(1);
        }
        if (players % 2 == 1)
        {
            handleConnection(connection, connection_socket_descriptor, players / 2);
        }
        else
        {
            connection = connection_socket_descriptor;
        }
        players++;
    }
    close(server_socket_descriptor);

    return 0;

}
