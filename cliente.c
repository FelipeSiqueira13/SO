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

    pid_t pid = getpid();
    int number_of_commands = 4;
    char * commands[] = {
		"execute",
		"-u",
        "-p",
        "status"
	};
    struct input in;
    in.pid = pid;
    int fd = open("server", O_WRONLY);
    if (fd == -1){
        perror("erro ao abrir fifo para escrita");
    }
    printf("ola4\n");

    if(strcmp(argv[1],"execute") == 0){
        printf("aaa");
        if(strcmp("-u",argv[3]) == 0){
            in.time = atoi(argv[2]); 
            printf("a");
            char comand[300] = "./";
            strcat(comand, argv[4]);
            strcat(comand, " ");
           for(int i = 5; i < strlen(argv); i++){
                printf("b");
                strcat(comand, argv[i]);
                strcat(comand, " ");
			}
            printf("\n%s",comand);
            strcpy(in.args,comand);
            printf("\n%s",in.args);
            write(fd, &in, sizeof(struct input));   
            getchar();                              
            //exec_unico(in.args);
        }//elif (strcmp(argv[3],"-p") == 0){//execução de muitas tarefas}  
        else{
            printf("wow");
            perror("Falta argumentos para a execução do programa.");
            _exit(EXIT_FAILURE);
        }        
    }else{
        perror("Falta argumentos para a execução do programa.");
        _exit(EXIT_FAILURE);
    }
    //abrir o fifo com o número do pip 
    int fd_fifostatus = open(pid, O_RDONLY);
    if(fd_fifostatus == -1){
        perror("erro ao abrir fifo para leitura");
        return -1;
    }
    char buffer[BUF_SIZE];
	int read_bytes;
	while ((read_bytes = read(fd_fifostatus, buffer, sizeof(buffer))) > 0){
		write(1, buffer, read_bytes);
        getchar();
	}
	close(fd_fifostatus);
	
    //while(1) printf("Estou vivo");
    return 0;
}