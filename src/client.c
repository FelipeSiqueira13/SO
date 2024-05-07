#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "input.h"

#define BUF_SIZE 1024
#define MAX_COMMANDS 300
#define READ 0
#define WRITE 1

int main(int argc, char** argv){
    int fd_send, fd_recieve;
    pid_t pid = getpid();
    char strpid[10];
    sprintf(strpid, "%d", pid);
    perror(":0");
    perror("slay");
    struct input in;
    in.pid = pid;
    in.pronto = 0;
    memset(in.args,"\0",sizeof(in.args));
    perror("ola");
    fd_send = open("server", O_WRONLY);
    if (fd_send == -1){
        perror("erro ao abrir fifo para escrita");
    }
    printf("ola4\n");

    if(strcmp(argv[1],"execute") == 0){
        perror("0");
        if(strcmp("-u",argv[3]) == 0){
            in.time = atoi(argv[2]);
            in.pronto = 0;
            in.id = 0;
            strcpy(in.args,argv[4]);
            write(1,in.args,sizeof(in.args));
            if (mkfifo(strpid, 0666) == -1) { 
                perror("Erro ao criar FIFO.");
            }
            write(fd_send, &in, sizeof(struct input));

            //exec_unico(in.args);
        }//elif (strcmp(argv[3],"-p") == 0){//execução de muitas tarefas}  
        else if(strcmp("-p", argv[3])==0){
            in.time = atoi(argv[2]);
            in.pronto = 4;
            in.id = 0;
            strcpy(in.args,argv[4]);
            if (mkfifo(strpid, 0666) == -1) { 
                perror("Erro ao criar FIFO.");
            }
            write(fd_send,&in,sizeof(struct input));
        }else{
            perror("Falta argumentos para a execução do programa.");
            _exit(EXIT_FAILURE);
        }        
    }else if(strcmp(argv[1],"status")==0){
        in.time = 0;
        in.pronto = 8;
        in.id = 0;
        perror("a");
        strcpy(in.args,"\0");
        if (mkfifo(strpid, 0666) == -1) { 
            perror("Erro ao criar FIFO.");
        }
        write(fd_send,&in,sizeof(struct input));
        close(fd_send);
        char msg[MAX_COMMANDS];
        for(int i=0; i< MAX_COMMANDS;i++) msg[i] = '\0';
        int bytes;
        fd_recieve = open(strpid,O_RDONLY);
        if (fd_recieve == -1) {
            perror("Erro ao abrir fifo2.");
            _exit(EXIT_FAILURE);
        }
        memset(msg,"\0",sizeof(msg));
        read(fd_recieve, msg, sizeof(msg));
        write(WRITE,msg,sizeof(msg));
        read(fd_recieve, msg, sizeof(msg));
        write(WRITE,msg,sizeof(msg));
        read(fd_recieve, msg, sizeof(msg));
        write(WRITE,msg,sizeof(msg));
        close(fd_recieve);
        unlink(strpid);
        perror("au");
        return 0;
    }else{
        perror("Falta argumentos para a execução do programa.");
        _exit(EXIT_FAILURE);
    }
    close(fd_send);
    fd_recieve = open(strpid,O_RDONLY);
    if (fd_recieve == -1) {
        perror("Erro ao abrir fifo2.");
        _exit(EXIT_FAILURE);
    }
    char msg[50];
    read(fd_recieve, msg, sizeof(msg));
    write(1,msg,sizeof(msg));
    close(fd_recieve);
    unlink(strpid);
    //while(1) printf("Estou vivo");

    return 0;
}
