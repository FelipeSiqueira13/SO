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

struct comand{
    int tam;
    char* args[];
};
int eleva(int x,int n){
    int result=x,i;
    for(i=1;i<n;i++){
        result = result*x;
    }
    return result;
}
int toint(char str[])
{
    int len = strlen(str);
    int i, num = 0;
 
    for (i = 0; i < len; i++)
    {
        num = num + ((str[len - (i + 1)] - '0') * eleva(10, i));
    }
 
   return num;
}

void organizer(struct listinput *lista,int tamanho){
    int i,j;
    struct input temp;
    for(i = 1;i<tamanho;i++){
        //organizar lista
        
    }
}

void insert(int fd,struct input tarefa){
    off_t offset = lseek(fd, 0,SEEK_END);
    write(fd, &tarefa, sizeof(struct input));
}

int main()
{
    int i;
    perror("estou vivo");
    /*for(i=0;i<5;i++){
        lista->list[i].time = 24 - i;
    }
    *tamanhodaLista = 5;*/
    //char *comando[] = {"./hello","20",NULL};
    int bytes_read;
    perror("estou vivo");
    struct input a;
    perror("estou vivo");
    //execvp(comando[0],comando);
    pid_t pid = fork();
    if(pid == 0){
        mkfifo("server", 0666);
        int fd = open("server", O_RDONLY);
        if (fd == -1){
            perror("erro ao abrir fifo para escrita");
        }
        int segur = open("server", O_WRONLY);
        int log = open("log.txt", O_CREAT | O_WRONLY | O_APPEND, 0600);
        if(log == -1) perror("erro ao abrir o log");
        while((bytes_read = read(fd, &a, sizeof(struct input))) > 0){
            //escrever para o fifo
            //b = *a;
            printf("recebi algo\n");
            printf("ola amigo %d %d  __ %s\n\n",a.pid,a.time,a.args);
            insert(log, a);
        }
        close(log);
        _exit(0);
    }else{
        getchar();
        while(1){
            
        }


    }

/*  char cpy[300], *str,*saida;
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
    }*/


    unlink("server");
    return 0;
}
