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
    int i,*tamanhodaLista;
    mkfifo("server", 0666);
    int fd = open("server", O_RDONLY);
    if (fd == -1){
        perror("erro ao abrir fifo para escrita");
    }

    struct listinput *lista;
    for(i=0;i<5;i++){
        lista->list[i].time = 24 - i;
    }
    strcpy(lista->list[0].args,"./hello 10");
    strcpy(lista->list[1].args,"./hello 7");
    strcpy(lista->list[2].args,"./hello 3");
    strcpy(lista->list[3].args,"./hello 9");
    strcpy(lista->list[4].args,"./hello 1");
    *tamanhodaLista = 5;
    //char *comando[] = {"./hello","20",NULL};
    int bytes_read, end;
    struct input a;
    char hello[300];
    //execvp(comando[0],comando);
    while((bytes_read = read(fd, &a, sizeof(struct input))) > 0){
        //escrever para o fifo
        //b = *a;
        printf("recebi algo\n");
        printf("ola amigo %d %d %s\n\n",a.pid,a.time,a.args);
        
        
        //printf("ola amigo %s\n",b.args);
        //write(1,a,bytes_read);
        //write(1,bytes_read,8);
    }
    //execvp
    organizer(&lista,5);



    unlink("server");
    return 0;
}
