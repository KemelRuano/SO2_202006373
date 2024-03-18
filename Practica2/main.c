#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "operaciones.h"
#include "users.h"

pthread_mutex_t lock;
Usuarios user[300];
int iter = 0;
int global = 0;
int globalT = 0;
char repoT[10000];

int depo = 0;
int tras = 0;
int retiro = 0;

void* CargaUsuarios(void* args){
    Control3H *Carga = (Control3H *)args;
    char line[100];
    char split;
    int local = 0;
    
    FILE *archivo;
    archivo = fopen("usuarios.csv", "r");
    
    // leer cabecera para ignorar
    fgets(line, sizeof(line),archivo);
    int posactual = global ;
    while(local < Carga->tamano){
        fgets(line, sizeof(line),archivo);
        if(Carga->hilo == 0){
              if(local >= posactual && local < (posactual + Carga->parte1)){
                    LLenarUsuarios(user,&iter,line,local);
                    global++;
              }
        }else if(Carga->hilo == 1){
            if(local >= posactual && local < (posactual + Carga->parte2)){
                    LLenarUsuarios(user,&iter,line,local);
                    global++;
              }
        }else if(Carga->hilo == 2){
            if(local >= posactual && local < (posactual + Carga->parte3)){
                    LLenarUsuarios(user,&iter,line,local);
                    global++;
              }
        }
        local++;
    }
}

void* CargaTransacciones(void* args){
    
    pthread_mutex_lock(&lock);
    Control4H *Carga = (Control4H *)args;
    char line[100];
    char split;
    int local = 0;

    FILE *archivo;
    archivo = fopen("transacciones.csv", "r");
    fgets(line, sizeof(line),archivo);
    int posactual = globalT;
    while(local < Carga->tamano){
        fgets(line, sizeof(line),archivo);
        if(Carga->hilo == 0){
              if(local >= posactual && local < (posactual + Carga->parte1)){
                    Transaccion(user,iter,line,local,repoT,&depo,&tras,&retiro);
                    globalT++;
              }
        }else if(Carga->hilo == 1){
            if(local >= posactual && local < (posactual + Carga->parte2)){
                    Transaccion(user,iter,line,local,repoT,&depo,&tras,&retiro);
                    globalT++;
              }
        }else if(Carga->hilo == 2){
            if(local >= posactual && local < (posactual + Carga->parte3)){
                    Transaccion(user,iter,line,local,repoT,&depo,&tras,&retiro);
                    globalT++;
              }
        }else if(Carga->hilo == 3){
            if(local >= posactual && local < (posactual + Carga->parte4)){
                    Transaccion(user,iter,line,local,repoT,&depo,&tras,&retiro);
                    globalT++;
              }
        }
        local++;
    }

    pthread_mutex_unlock(&lock);
}




int main() {
    while(true){
        int op = 0;
        printf("************* MENU INICIAL *************** \n");
        printf("1.  Carga Masiva Usuarios \n");
        printf("2.  Carga Masiva Operaciones \n");
        printf("3.  Operaciones Individuales \n");
        printf("4.  Reporte de Estado de Cuenta \n");
        printf("5.  Exit\n");
        printf("> Ingrese una opcion: ");
        if(scanf("%d",&op) == 1){

        
                    if(op == 1){
                            system("clear");
                            global = 0;
                            char* DateTime = DateTimeString();
                            int countLine = getLines("usuarios.csv");
                    
                            // Distribucion de hilos
                            srand(time(NULL));
                            int p1,p2,p3;
                            p1 = rand() % countLine;
                            p2 = rand() % (countLine - p1);
                            p3 = countLine - p1 - p2;

                            // //CREAR REPORTE
                            FILE *writefile;
                            writefile = fopen("Reportes/usuarios.txt", "w");
                            fprintf(writefile,"---------------------- CARGA DE USUARIOS ------------------------------\n");
                            fprintf(writefile,"Fecha:  %s\n",DateTime);
                            fprintf(writefile,"Usuarios Cargados: \n");
                            fprintf(writefile,"Hilo #1: %d\n",p1);
                            fprintf(writefile,"Hilo #2: %d\n",p2);
                            fprintf(writefile,"Hilo #3: %d\n",p3);
                            fprintf(writefile,"Total:  %d\n",countLine);
                            fprintf(writefile,"\n");
                            fprintf(writefile,"Errores: \n");
                            fclose(writefile);

                            // // CARGA EN 3 HILOS
                            pthread_t threads[3];
                            for (int i = 0; i < 3; i++){
                                Control3H *carga = malloc(sizeof(Control3H));
                                carga->hilo = i;
                                carga->parte1 = p1;
                                carga->parte2 = p2;
                                carga->parte3 = p3;
                                carga->tamano = countLine;
                                pthread_create(&threads[i], NULL, CargaUsuarios, (void *) carga);
                            }

                            for(int i = 0; i < 3; i++){pthread_join(threads[i], NULL);}   
                
                    }else if (op == 2){
                            system("clear");
                            globalT = 0;
                            int NumLine = getLines("transacciones.csv");
                            
                            // Distribucion de hilos
                            srand(time(NULL));
                            int p1,p2,p3,p4;
                            p1 = rand() % NumLine;
                            p2 = rand() % (NumLine - p1);
                            p3 =  rand() % (NumLine - p1 - p2);
                            p4 = NumLine - p1 - p2 - p3;

                            
                            
                            pthread_mutex_init(&lock, NULL); 
                            pthread_t threads[4];
                            for (int i = 0; i < 4; i++){
                                Control4H *carga = malloc(sizeof(Control4H));
                                carga->hilo = i;
                                carga->parte1 = p1;
                                carga->parte2 = p2;
                                carga->parte3 = p3;
                                carga->parte4 = p4;
                                carga->tamano = NumLine;
                                pthread_create(&threads[i], NULL, CargaTransacciones, (void *) carga);
                            }

                            for(int i = 0; i < 4; i++){pthread_join(threads[i], NULL);}   
                            
                            pthread_mutex_destroy(&lock);


                            // //CREAR REPORTE
                            FILE *writefile;
                            writefile = fopen("Reportes/transacciones.txt", "w");
                            fprintf(writefile,"---------------------- RESUMEN DE OPERACIONES ------------------------------\n");
                            fprintf(writefile,"Fecha:  %s\n",DateTimeString());
                            fprintf(writefile,"Operaciones Realizadas: \n");
                            fprintf(writefile,"Retiros: %d\n",retiro);
                            fprintf(writefile,"Depositos: %d\n",depo);
                            fprintf(writefile,"Transferencias: %d\n",tras);
                            fprintf(writefile,"Total:  %d\n",(tras+depo+retiro));
                            fprintf(writefile,"\n");
                            fprintf(writefile,"Operaciones por Hilos: \n");
                            fprintf(writefile,"Hilo #1: %d\n",p1);
                            fprintf(writefile,"Hilo #2: %d\n",p2);
                            fprintf(writefile,"Hilo #3: %d\n",p3);
                            fprintf(writefile,"Hilo #4: %d\n",p4);
                            fprintf(writefile,"Total:  %d\n",NumLine);
                            fprintf(writefile,"\n");
                            fprintf(writefile,"Errores: \n");
                            fprintf(writefile,"%s",repoT);
                            fclose(writefile);


                            depo = 0;
                            tras = 0;
                            retiro = 0;

                    }else if(op == 3){
                        system("clear");
                        MenuOp(user,iter);
                    }else if (op == 4){
                            system("clear");
                            FILE *writefile;
                            writefile = fopen("Reportes/EstadoCuenta.txt", "w");
                            fprintf(writefile,"no_cuenta , nombre , saldo\n");
                            for (int i = 0; i < iter; i++)
                            {
                                fprintf(writefile,"%d,%s,%lf\n",user[i].no_cuenta,user[i].nombre,user[i].saldo);
                            }
                            fclose(writefile);
                    }else if (op == 5){
                        break;
                    }else{
                        printf("\033[1;31m");
                        printf("!!!! opcion invalida !!!\n");
                        printf("\033[0m");
                    }
        }else{
            printf("\033[1;31m");
            printf("LA OPCION INGRESADA NO ES UN NUMERO\n");
            while (getchar() != '\n');
            printf("\033[0m");
        }
   }
    
    return 0;
    
}

