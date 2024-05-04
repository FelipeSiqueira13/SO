struct input{
    int time;
    int pronto;// 0 não pronto, 1 executando, 2 terminado
    pid_t pid;
    char args[300];
};
