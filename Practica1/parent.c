#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>


void sigint_handler(int) {
   
    FILE *fp;
    char *linea = NULL;
    size_t longitud_linea = 0;
    ssize_t caracteres_leidos;
    int cprocess , creead , cwrite , cseek;
    fp = fopen("syscalls.log", "r");
    if (fp == NULL) {
        printf("Error al abrir el archivo.\n");
        exit(1);
    }

     while ((caracteres_leidos = getline(&linea, &longitud_linea, fp)) != -1) {
        if (strstr(linea, "Cprocess") != NULL) {
            sscanf(linea, "Cprocess: %d", &cprocess);
        } else if (strstr(linea, "Cread") != NULL) {
            sscanf(linea, "Cread: %d", &creead);
        } else if (strstr(linea, "Cwrite") != NULL) {
            sscanf(linea, "Cwrite: %d", &cwrite);
        } else if (strstr(linea, "Cseek") != NULL) {
            sscanf(linea, "Cseek: %d", &cseek);
        }
    }

    fclose(fp);
    //Limpiar consola
    char command[100];
    sprintf(command, "clear");
    system(command);

    printf("\n");
    printf("Número de procesos Totales: %d\n", cprocess);
    printf("Read: %d\n", creead);
    printf("Write: %d\n", cwrite);
    printf("Seek: %d\n", cseek);
    printf("\n");
}

void main(){

    int status;

    int fd = open("syscalls.log",  O_RDWR | O_CREAT, 0777);
    if(fd == -1){
        perror("Error al abrir el archivo");
        close(fd);
    }
    if(signal(SIGINT, sigint_handler) == SIG_ERR) {
                perror("signal");
                exit(1);
    }

    pid_t pid1 , pid2 ;
    pid1 = fork();
    if (pid1 == 0){
        char *arg_ptr[2];
        arg_ptr[0]= "child.c";
        arg_ptr[1]= NULL;
        execv("/home/oem/Desktop/Practica1/child.bin", arg_ptr);
    }else if (pid1 > 0){
       
        pid2 = fork();
        if (pid2 == 0){
            char *arg_ptr[2];
            arg_ptr[0]= "child.c";
            arg_ptr[1]= NULL;
            execv("/home/oem/Desktop/Practica1/child.bin", arg_ptr);
        }else if (pid2 > 0){
                char command[100];
                sprintf(command, "%s %d %d %s", "sudo stap systemtap.stp ",pid1, pid2," > syscalls.log");
                system(command);
                waitpid(pid1, &status, 0); 
                waitpid(pid2, &status, 0);
        }else{
            perror("fork proceso 2");
            exit(1);
        }
    }else{
        perror("fork proceso 1");
        exit(1);
    }  
}