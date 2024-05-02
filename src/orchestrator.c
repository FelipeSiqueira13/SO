#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "input.h"

#define BUF_SIZE 1024
#define MAX_COMMANDS 300
#define READ 0
#define WRITE 1

struct listinput{
    struct input list[200];
};

void organizer(struct listinput *lista,int tamanho){
    int i,j;
    struct input temp;
    for(i = 1;i<tamanho;i++){
        for(j = 0;j<i;j++){
            if(lista->list[i].time < lista->list[j].time){
                temp.time = lista->list[i].time;
                strcpy(temp.args,lista->list[i].args);
                lista->list[i].time = lista->list[j].time;
                strcpy(lista->list[i].args,lista->list[j].args);
                lista->list[j].time = temp.time;
                strcpy(lista->list[j].args,temp.args);
            }
        }
    }
}

int main()
{
    int i;
    int fd = mkfifo("server", 0666);
    if(fd==-1){
        perror("erro na criação do fifo");
    }
    fd = open("server", O_RDONLY);
    if (fd == -1){
        perror("erro ao abrir fifo para escrita");
    }

    struct listinput lista;
    for(i=0;i<5;i++){
        lista.list[i].time = 24 - i;
    }
    strcpy(lista.list[0].args[0],"hello");
    strcpy(lista.list[0].args[1],"4");
    strcpy(lista.list[1].args[0],"hello");
    strcpy(lista.list[1].args[1],"6");
    strcpy(lista.list[2].args[0],"hello");
    strcpy(lista.list[2].args[1],"7");
    strcpy(lista.list[3].args[0],"hello");
    strcpy(lista.list[3].args[1],"13");
    strcpy(lista.list[4].args[0],"hello");
    strcpy(lista.list[4].args[1],"10");
    //char *comando[] = {"./hello","20",NULL};
    int bytes_read, end;
    
    struct input *a;
    //execvp(comando[0],comando);
    while((bytes_read = read(fd, a, 8)) > 0){
        //escrever para o fifo
        printf("recebi algo\n");
        printf("ola amigo %s",a->args);
    }
    //execvp
    organizer(&lista,5);
    for(i=0;i<5;i++){
        printf("%d  %s\n",lista.list[i].time,lista.list[i].args);
    }


    return 0;
}
