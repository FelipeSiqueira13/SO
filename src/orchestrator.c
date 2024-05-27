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
    char strid[10];
    sprintf(strid,"%d",in.id);
    int i = 0,fd_out,tofork; 
    int standardout = dup(STDOUT_FILENO);
    char caminho[50];
    memset(caminho,0,sizeof(caminho));
    strcpy(caminho,fld);
    strcat(caminho,"/");
    strcat(caminho,strid);
    strcat(caminho,".txt");
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
    memset(fifores,'\0',sizeof(fifores));
    sprintf(fifores,"%d",pid);
    int fd_sts = open(LISTA, O_RDONLY), bytes_read,p=0,g=0,e=0;
    while((bytes_read = read(fd_sts, &temp, sizeof(struct input))) > 0){
        if(temp.pronto == 0 || temp.pronto==4){
            cpyinput(&espera[e],temp);
            e++;
        }else if(temp.pronto == 1 || temp.pronto == 5){
            cpyinput(&going[g++],temp);
            g++;
        }else if(temp.pronto == 2|| temp.pronto == 6){
            cpyinput(&pronto[p],temp);
            p++;
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
        if (going[i].pronto == 1) {
            char *chave;
            chave = strtok(going[i].args, " ");
            strcat(msg, chave);
        } else if (going[i].pronto == 5) {
            char *chavearray[MAX_COMMANDS];
            char *string;	
	        int k=0;

	        char* command = strdup(going[i].args);

	        string=strtok(command,"|");

            while(string!=NULL){
                chavearray[k]=string;
                string=strtok(NULL,"|");
                k++;
            }

            char *chavefinal;
            for (int j=0;j<k;j++) {
                chavefinal = strtok(chavearray[j], " ");
                strcat(msg,chavefinal);
                if (j != k-1) strcat(msg," | ");
            }
        }
        strcat(msg," \n");
    }
    strcat(msg,"\nScheduled\n");
    for(i=0;i<e;i++){
        sprintf(strid,"%d",espera[i].id);
        strcat(msg,strid);
        strcat(msg," ");
        if (espera[i].pronto == 0) {
            char *chave;
            chave = strtok(espera[i].args, " ");
            strcat(msg, chave);
        } else if (espera[i].pronto == 4) {
            char *chavearray[MAX_COMMANDS];
            char *string;	
	        int i=0;

	        char* command = strdup(espera[i].args);

	        string=strtok(command,"|");

            while(string!=NULL){
                chavearray[i]=string;
                string=strtok(NULL,"|");
                i++;
            }

            char *chavefinal;
            for (int j=0;j<i;j++) {
                chavefinal = strtok(chavearray[j], " ");
                strcat(msg,chavefinal);
                if (j != i-1) strcat(msg," | ");
            }
        }    
        strcat(msg,"\n");
    }
    strcat(msg,"\nCompleted\n");
    for(i=0;i<p;i++){
        sprintf(strid,"%d",pronto[i].id);
        strcat(msg,strid);
        strcat(msg," ");
        if (pronto[i].pronto == 2) {
            char *chave;
            chave = strtok(pronto[i].args, " ");
            strcat(msg, chave);
        } else if (pronto[i].pronto == 6) {
            char *chavearray[MAX_COMMANDS];
            char *string;	
	        int i=0;

	        char* command = strdup(pronto[i].args);

	        string=strtok(command,"|");

            while(string!=NULL){
                chavearray[i]=string;
                string=strtok(NULL,"|");
                i++;
            }

            char *chavefinal;
            for (int j=0;j<i;j++) {
                chavefinal = strtok(chavearray[j], " ");
                strcat(msg,chavefinal);
                if (j != i-1) strcat(msg," | ");
            }
        }
        strcat(msg," ");
        sprintf(strid,"%d",pronto[i].time);
        strcat(msg,strid);
        strcat(msg, " ms");
        strcat(msg,"\n");
    }
    write(lista,msg,sizeof(msg));
    close(lista);
    unlink(fifores);
    _exit(0);
}

void response(pid_t pid, int id){
    char fifores[20],msg[50] = "TASK ", strid[10];
    memset(fifores,'\0',sizeof(fifores));
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
                in.id = pos;
                gettimeofday(&in.inicio,NULL);
                insert(in);
                response(in.pid,in.id);
                //uni exec
            }else if(in.pronto == 4){
                pos++;
                gettimeofday(&in.inicio,NULL);
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
        struct timeval fim;
        struct input in, tofork;
        int i,vivo=1, atual, fd_lista, lido, new,status, act;
        double tempopassado;
        int cont = open("tmp/assis.txt", O_CREAT | O_WRONLY, 0640);
        int array[max];
        int zero = 0, temp, bytes_read2;
        for(i = 0; i < max;i++) write(cont,&zero,sizeof(zero));
        close(cont);
        while(vivo){
            cont = open("tmp/assis.txt", O_RDONLY);
            i = 0;
            while (bytes_read = read(cont, &temp, sizeof(temp))>0)
            {
                if(temp != 0)act++;
            }
            close(cont);
            lido = 2;
            act = 0;
            if(max<=act){
                wait(NULL);
            }else{
                lido = 1;
            }
            fd_lista = open(LISTA,O_RDWR);
            memset(tofork.args,'\0',sizeof(tofork.args));
            while(lido == 1 && (bytes_read = read(fd_lista, &in , sizeof(struct input)))>0){
                if(in.pronto == 0 || in.pronto == 4){ 
                    cont = open("tmp/assis.txt", O_RDWR);
                    while( ((bytes_read2 = read(cont, &temp , sizeof(temp)))>0) && temp!=0);
                    if(temp == 0){
                        in.pronto++;
                        lido = 0;
                        lseek(cont, -sizeof(temp), SEEK_CUR);
                        write(cont, &in.id, sizeof(temp));
                        tofork.id = in.id;
                        tofork.pid = in.pid;
                        strcpy(tofork.args,in.args);
                        tofork.pronto = in.pronto;
                        tofork.time = 0;
                        lseek(fd_lista, -sizeof(struct input), SEEK_CUR);
                        write(fd_lista, &in, sizeof(struct input));
                    }
                    close(cont);
                }
            }
            close(fd_lista);
            new = 1;
            if (lido==0) new = fork();
            if(new==0){
                if(tofork.pronto==1){
                single_exec(tofork,saida);

                }else if(tofork.pronto == 5){
                multi_exec(tofork,saida);
                }
                wait(&status);
                gettimeofday(&fim,NULL);
                if(WIFEXITED(status)){
                    fd_lista = open(LISTA,O_RDWR);
                    while((bytes_read = read(fd_lista, &in, sizeof(struct input)))>0 && in.id!=tofork.id);
                    in.pronto++;
                    cont = open("tmp/assis.txt", O_RDWR);
                    while( bytes_read2 = read(cont, &temp , sizeof(temp))>0 && temp!=in.id);
                    if(temp == in.id){
                        lseek(cont, -sizeof(temp), SEEK_CUR);
                        write(cont, &zero, sizeof(temp));
                    }
                    close(cont);
                    tempopassado = (fim.tv_sec - in.inicio.tv_sec) * 1000.0;
                    tempopassado += (fim.tv_usec - in.inicio.tv_usec) / 1000.0;
                    in.time = (int)tempopassado;
                    lseek(fd_lista, -sizeof(struct input), SEEK_CUR);
                    write(fd_lista, &in, sizeof(struct input));
                    close(fd_lista);
                }
            }
        }
        
    }

    unlink("server");
    return 0;
}
