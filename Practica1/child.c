#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
int main(int argc){

    printf("-------------archivo abierto --------------");
    int fd = open("practica1.txt",  O_CREAT|O_WRONLY|O_TRUNC, 0755);
    if(fd == -1){
        perror("practica1.txt !!ERROR!!");
        close(fd);
    }

    while(1){
        srand(time(NULL));
        int callsystem = (rand() % 3) + 1;
        if(callsystem == 1){
              char bufferWrite[9];
              bufferWrite[8] = '\0';
              char ListCharacter[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
              int longitud = sizeof(ListCharacter) - 1;
              for (int i = 0; i < 8; i++){
                    int randomcaracter = rand() % longitud;
                    bufferWrite[i] = ListCharacter[randomcaracter]; 
                    
              }
              int byteswrite = write(fd , bufferWrite , strlen(bufferWrite));
        }else if ( callsystem == 2){

            char bufferRead[9];
            bufferRead[8] = '\0';
            int bytesread = read(fd , bufferRead , 8);       
        }else{
            lseek(fd, 0, SEEK_SET);
        }
        sleep(callsystem);
    }

}