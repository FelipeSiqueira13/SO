#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define BUF_SIZE 1024
#define MAX_COMMANDS 300
#define READ 0
#define WRITE 1

int exec_command(char* arg){

	//Estamos a assumir numero maximo de argumentos
	char* exec_args[300];
	char* string;	
	int exec_ret = 0;
	int i = 0;
	char* command = strdup(arg);
	string=strtok(command," ");
	
	while(string != NULL){
		exec_args[i] = string;
		string = strtok(NULL," ");
		i++;
	}

	exec_args[i] = NULL;
	exec_ret = execvp(exec_args[0],exec_args);

	return exec_ret;
}

void uniexec(char arg[]){}

int main(int argc, char** argv){

    int number_of_commands = 4;
    char * commands[] = {
		"execute",
		"-u",
        "-p",
        "status"
	};

    struct input{
        int time;
        char* args[];
    };
    struct input in;

    if(strcmp(argv[1],"execute") == 0){
        in.time = argv[2];
        if(strcmp(argv[3],"-u") == 0){//execução de uma tarefa
            char nome[10] = "./";
            strcat(nome, argv[4]);
            in.args[0] = nome;
            int j = 5;
           for(int i = 1; i < strlen(argv); i++){
				in.args[i] = argv[j];
                j++;
			}
            exec_unico(in.args);
        }//elif (strcmp(argv[3],"-p") == 0){//execução de muitas tarefas}  
        else{
            perror("Falta argumentos para a execução do programa.");
            _exit(EXIT_FAILURE);
        }        
    }else{
        perros("Falta argumentos para a execução do programa.");
        _exit(EXIT_FAILURE);
    }

    return 0;
}
