#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "input.h"

#define BUF_SIZE 1024
#define MAX_COMMANDS 300
#define READ 0
#define WRITE 1
#define LISTA "tmp/lista.txt"

void single_exec(struct input in, char *fld){
    char *comando[300];
    char *temp;
    char ini[50] = "include/";
    perror("ola");
    char strid[10];
    sprintf(strid,"%d",in.id);
    int i = 0,fd_out,tofork; 
    int standardout = dup(STDOUT_FILENO);
    char caminho[50];
    perror("aaa");
    memset(caminho,0,sizeof(caminho));
    strcpy(caminho,fld);
    strcat(caminho,"/");
    strcat(caminho,strid);
    strcat(caminho,".txt");
    perror("pog");
    char *dupcomando = strdup(in.args);
    temp=strtok(dupcomando," ");
	while(temp!=NULL){
        comando[i]=temp;
		temp=strtok(NULL," ");
		i++;
	}
    comando[i] = NULL;
    strcat(ini,comando[0]);
    comando[0] = ini;
    write(1,ini,sizeof(comando[0]));
    perror("poggers");
    fd_out = open(caminho, O_CREAT | O_WRONLY | O_APPEND, 0660);
    dup2(fd_out,STDOUT_FILENO);
	tofork = fork();
    if(tofork == 0){
        execvp(comando[0],comando);
    }
    dup2(standardout,fd_out);
    //close(fd_out);
}

void multi_exec(struct input in, char *fld){
    char *varioscomandos[MAX_COMMANDS];
    char *temp;
    int i = 0,j;
    char *cpycomando = strdup(in.args);
    temp = strtok(cpycomando,"|");
    for(i = 0;temp!=NULL;i++){
        varioscomandos[i] = temp;
        temp = strtok(NULL,"!");
    }
    for(j=0;j<i;j++){
        strcpy(in.args,varioscomandos[j]);
        single_exec(in,fld);
        wait(NULL);
    }

}

void organizer(struct listinput *lista,int tamanho){
    int i,j;
    struct input temp;
    for(i = 1;i<tamanho;i++){
        //organizar lista
        
    }
}

void cpyinput(struct input *to,struct input from){
    to->id = from.id;
    to->pid = from.pid;
    to->pronto = from.pronto;
    to->time = from.time;
    strcpy(to->args,from.args);
}

void status(pid_t pid,int max){
    char fifores[20], msg[MAX_COMMANDS];
    struct input pronto[MAX_COMMANDS];
    struct input going[MAX_COMMANDS];
    struct input espera[MAX_COMMANDS];
    struct input temp;
    int i;
    for(i=0; i< BUF_SIZE;i++) msg[i] = '\0';
    memset(fifores,"\0",sizeof(fifores));
    sprintf(fifores,"%d",pid);
    int fd_sts = open(LISTA, O_RDONLY), bytes_read,p=-1,g=-1,e=-1;
    while((bytes_read = read(fd_sts, &temp, sizeof(struct input))) > 0){
        if(temp.pronto == 0 || temp.pronto==4){
            cpyinput(&espera[e++],temp);
        }else if(temp.pronto == 1 || temp.pronto == 5){
            cpyinput(&going[g++],temp);
        }else if(temp.pronto == 2|| temp.pronto == 6){
            cpyinput(&pronto[p++],temp);
        }
    }
    close(fd_sts);
    strcpy(msg,"Executing\n");
    char strid[10];
    int lista = open(fifores, O_WRONLY);
    for(i=0;i<g;i++){
        sprintf(strid,"%d",going[i].id);
        strcat(msg,strid);
        strcat(msg," ");
        strcat(msg, going[i].args);
        strcat(msg,"\n");
    }
    write(lista,msg,sizeof(msg));
    strcpy(msg,"\nScheduled\n");
    for(i=0;i<e;i++){
        sprintf(strid,"%d",espera[i].id);
        strcat(msg,strid);
        strcat(msg," ");
        strcat(msg, espera[i].args);
        strcat(msg,"\n");
    }
    write(lista,msg,sizeof(msg));
    strcpy(msg,"\nCompleted\n");
    for(i=0;i<p;i++){
        sprintf(strid,"%d",pronto[i].id);
        strcat(msg,strid);
        strcat(msg," ");
        strcat(msg, pronto[i].args);
        strcat(msg,"\n");
    }
    write(lista,msg,sizeof(msg));
    close(lista);
    unlink(fifores);
    _exit(0);
}

void response(pid_t pid, int id){
    char fifores[20],msg[50] = "TASK ", strid[10];
    memset(fifores,"\0",sizeof(fifores));
    sprintf(strid,"%d",id);
    strcat(msg,strid);
    strcat(msg, " Received\n");
    sprintf(fifores,"%d",pid);
    int lista = open(fifores, O_WRONLY);
    write(lista,msg,sizeof(msg));
    close(lista);
    unlink(fifores);
}

void insert(struct input in){
    perror("2");
    write(1,in.args,sizeof(in.args));
    int lista = open(LISTA, O_WRONLY | O_APPEND);
    write(lista, &in, sizeof(struct input));
    close(lista);
}

int main(int argc, char* argv[]){
    if(argc != 4){ 
        perror("por favor coloque a pasta de output, quantidade de multitask e politica de escalonamento");
        return 1;
    }
    int max = atoi(argv[2]);
    int politica = atoi(argv[3]);
    char saida[30];
    strcpy(saida,argv[1]);
    mkdir(saida,S_IRWXU | S_IRWXG | S_IRWXO);
    int bytes_read;
    pid_t pid = fork();
    if(pid == 0){
        mkfifo("server", 0666);
        int pos = 0;
        int lista = open(LISTA, O_CREAT, 0600);
        if(lista == -1) perror("erro ao abrir o log");
        close(lista);
        perror("mariana");
        struct input in;
        int fd ;
        int ligado = 1;
        
        while(ligado){//leitor
            fd = open("server", O_RDONLY);
            if (fd == -1){
                perror("erro ao abrir fifo para escrita");
            }
            read(fd,&in,sizeof(struct input));
            if(in.pronto == 8){
                int sts = fork();
                if (sts==0){
                    status(in.pid,max);
                }
            }else if(in.pronto == 0){
                pos++;
                write(1,in.args,sizeof(in.args));
                in.id = pos;
                insert(in);
                response(in.pid,in.id);
                //uni exec
            }else if(in.pronto == 4){
                pos++;
                in.id = pos;
                insert(in);
                response(in.pid,in.id);
                //multi exec
            }else if(in.pronto == 16){
                pos++;
                in.id = pos;
                insert(in);
                response(in.pid,in.id);
            }else{
                perror("erro de pronto");
            }
            close(fd);
        }
        

        _exit(0);
    }else{
        struct timeval inicio,fim;
        struct input in, tofork;
        int assist[max],i,vivo=1, atual, fd_lista, lido, new,status;
        double tempopassado;
        for(i=0;i<max;i++) assist[i]=0;
        perror("ola");
        while(vivo){
            lido = 2;
            for(i=max-1;0<=i;i--){
                if(assist[i] == 0)atual = i;
                lido = 1;
            }
            fd_lista = open(LISTA,O_RDWR);
            memset(tofork.args,'\0',sizeof(tofork.args));
            while(lido == 1 && (bytes_read = read(fd_lista, &in , sizeof(struct input)))>0){
                if(in.pronto == 0 || in.pronto == 4){ 
                    lido = 0;
                    assist[atual] = in.id;
                    in.pronto++;
                    tofork.id = in.id;
                    tofork.pid = in.pid;
                    strcpy(tofork.args,in.args);
                    perror("perros");
                    write(1,in.args,sizeof(in.args));
                    write(1,tofork.args,sizeof(tofork.args));
                    tofork.pronto = in.pronto;
                    tofork.time = 0;
                    lseek(fd_lista, -sizeof(struct input), SEEK_CUR);
                    write(fd_lista, &in, sizeof(struct input));
                }
            }
            close(fd_lista);
            new = 1;
            if (lido==0) new = fork();
            if(new==0){
                perror("ops");
                gettimeofday(&inicio,NULL);
                if(tofork.pronto==1){
                perror("slay");
                write(1,tofork.args,sizeof(tofork.args));
                single_exec(tofork,saida);

                }else if(tofork.pronto == 5){
                multi_exec(tofork,saida);
                }
                perror("ola");
                wait(&status);
                perror("hugo");
                gettimeofday(&fim,NULL);
                perror("pogo");
                if(WIFEXITED(status)){
                    fd_lista = open(LISTA,O_RDWR);
                    while((bytes_read = read(fd_lista, &in, sizeof(struct input)))>0 && in.id!=tofork.id);
                    in.pronto++;
                    tempopassado = (fim.tv_sec - inicio.tv_sec) * 1000.0;
                    tempopassado += (fim.tv_usec - inicio.tv_usec) / 1000.0;
                    in.time = (int)tempopassado;
                    lseek(fd_lista, -sizeof(struct input), SEEK_CUR);
                    write(fd_lista, &in, sizeof(struct input));
                    close(fd_lista);
                }
            }
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
