struct input{
    int time;
    int pronto; // uni 0 não pronto, 1 executando, 2 terminado
                // multi 4 não pronto, 5 executando, 6 terminado
                // 8 status
                // 16 shutdown
    int id;
    pid_t pid;
    char args[300];
};
