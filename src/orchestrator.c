#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <math.h>
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

struct comand{
    int tam;
    char* args[];
};

int toint(char str[])
{
    int len = strlen(str);
    int i, num = 0;
 
    for (i = 0; i < len; i++)
    {
        num = num + ((str[len - (i + 1)] - '0') * pow(10, i));
    }
 
   return num;
}

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
    int *live = 1;
    int bytes_read, end;
    struct input a;
    char hello[300];
    //execvp(comando[0],comando);
    pid_t pid = fork();
    if(pid = 0){
        mkfifo("server", 0666);
        int fd = open("server", O_RDONLY);
        if (fd == -1){
            perror("erro ao abrir fifo para escrita");
        }
        while((bytes_read = read(fd, &a, sizeof(struct input))) > 0){
            //escrever para o fifo
            //b = *a;
            printf("recebi algo\n");
            printf("ola amigo %d %d %s\n\n",a.pid,a.time,a.args);
            lista->list[*tamanhodaLista].time = a.time;
            lista->list[*tamanhodaLista].pid = a.pid;
            strcpy(lista->list[*tamanhodaLista].args,a.args);
            *tamanhodaLista = *tamanhodaLista + 1;
            organizer(lista,tamanhodaLista);
        }
    }


    char cpy[300], *str,*saida;
    struct comand comand;
    int fd;
    while(*live){
        strcpy(cpy,lista->list[0].args);
        str = strtok(cpy, " ");
        strcpy(comand.args[0],str);
        *saida = toint(lista->list[0].pid);
        i = 1;
        while( str != NULL){
            str = strtok(NULL, " ");
            strcpy(comand.args[i],str);
            i++;
        }
        fd = open(saida, O_WRONLY | O_CREAT | O_TRUNC,  0644);
        dup2(fd,STDOUT_FILENO);
        close(fd);
        execl(comand.args[0],comand.args);
    }



    unlink("server");
    return 0;
}
