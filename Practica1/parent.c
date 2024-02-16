#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pthread.h>


void main(){

 

    int fd = open("syscalls.log",  O_RDWR | O_CREAT, 0777);
    if(fd == -1){
        perror("Error al abrir el archivo");
        close(fd);
    }
    // Se crea el proceso hijo 1
    pid_t pid1 , pid2 ;
    pid1 = fork();
    if (pid1 == 0){
        char *arg_ptr[2];
        arg_ptr[0]= "child.c";
        arg_ptr[1]= NULL;
        execv("/home/oem/Desktop/Practica1/child.bin", arg_ptr);
    }else if (pid1 > 0){
        // // Se crea el proceso hijo 2
        pid2 = fork();
        if (pid2 == 0){
            char *arg_ptr[2];
            arg_ptr[0]= "child.c";
            arg_ptr[1]= NULL;
            execv("/home/oem/Desktop/Practica1/child.bin", arg_ptr);
        }else if (pid2 > 0){
                char command[100];
                sprintf(command, "%s %d %d %s", "sudo stap systemtap.stp ",pid1, pid2, " > syscalls.log");
                system(command);
            
        }else{
            perror("fork proceso 2");
            exit(1);
        }
        
    }else{
        perror("fork proceso 1");
        exit(1);
    }  
}