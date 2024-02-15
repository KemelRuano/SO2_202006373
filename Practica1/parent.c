#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>

void main(){

    printf("soy padre mi id es: %d \n",getpid());
    printf("\n");

    int fd = open("syscalls.log",  O_RDWR | O_CREAT, 0777);
    if(fd == -1){
        perror("Error al abrir el archivo");
        close(fd);
    }
    // Se crea el proceso hijo 1
    pid_t pid1 = fork();
    if (pid1 == 0){
        printf("Proceso hijo1 corrida1: %d \n" ,getpid());
    }else if (pid1 > 0){
        printf("proceso padre corrida 1: %d \n",getpid());
        // Se crea el proceso hijo 2
        pid_t pid2 = fork();
        if (pid2 == 0){
            printf("Proceso hijo2 corrida2: %d \n" ,getpid());
        }else if (pid2 > 0){
            printf("proceso padre corrida2: %d \n",getpid());
        }else{
            perror("fork proceso 2");
            exit(1);
        }
        
    }else{
        perror("fork proceso 1");
        exit(1);
    }
    printf("fin de proceso : %d \n",getpid());
     
}