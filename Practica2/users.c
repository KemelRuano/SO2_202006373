#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "users.h"

int getLines(char namefile[]){
    FILE *archivo;
    archivo = fopen(namefile, "r");
    int line = 0;
    char caracter;
    while((caracter = fgetc(archivo)) != EOF){
            if(caracter == '\n'){
                line++;
            }
    }
    fclose(archivo);
    return line;

}


char* DateTimeString(){
    static char buffer[80];
    time_t tiempoActual;
    struct tm *infoTiempo;
    time(&tiempoActual);
    infoTiempo = localtime(&tiempoActual);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", infoTiempo);
    return buffer;
}

bool esNumero(const char *cadena) {
    double numero;
    char resto[2]; 
    if (sscanf(cadena, "%lf%1s", &numero, resto) == 1) {
        return true;
    } else {
        return false;
    }
}

bool existuser(Usuarios *user , int *n ,  int cuenta){
    for (int i = 0; i < (*n); i++)
    {
        if (user[i].no_cuenta == cuenta){
            return false;
        }
    }
    return true;
}

void LLenarUsuarios(Usuarios *user, int *n , char *buffer , int line){

    FILE *writefile;
    int no_cuenta = atoi(strtok(buffer,","));
    
    char usuario[25];
    strcpy(usuario , strtok(NULL,","));
    
    double saldo;
    if(existuser(user ,n,no_cuenta)){
        char *token;
        token = strtok(NULL,",");
        if(esNumero(token)){
            sscanf(token, "%lf",&saldo);
            if(saldo >= 0){
                user[(*n)].no_cuenta = no_cuenta;
                strcpy(user[(*n)].nombre, usuario);
                user[(*n)].saldo = saldo;
                (*n)++;
            }else{
                writefile = fopen("Reportes/usuarios.txt", "a");
                fprintf(writefile,"Linea #%d el saldo es menor a 0 \n",(line+2));
                fclose(writefile);
            }
        }else{
                writefile = fopen("Reportes/usuarios.txt", "a");
                fprintf(writefile,"Linea #%d el saldo no es un numero \n",(line+2));
                fclose(writefile);
        }
    }else{
        writefile = fopen("Reportes/usuarios.txt", "a");
        fprintf(writefile,"Linea #%d Ya existe la cuenta: %d \n",(line+2),no_cuenta);
        fclose(writefile);
    }

}

void Deposito(Usuarios *user, int size , int cuenta , double monto){
    for (int i = 0; i < size; i++){
        if(user[i].no_cuenta == cuenta){
            double SaldoActual = user[i].saldo;
            SaldoActual = SaldoActual + monto;
            user[i].saldo = SaldoActual;
        }
    }

}

bool Retiro(Usuarios *user, int size , int cuenta , double monto){
    for (int i = 0; i < size; i++){
        if(user[i].no_cuenta == cuenta){
            double SaldoActual = user[i].saldo;
            SaldoActual = SaldoActual - monto;
            if(SaldoActual < 0){
                return false;
            }else{
                user[i].saldo = SaldoActual;
            }
        }
    }
    return true;
}

void Transaccion(Usuarios *user, int n , char *buffer , int line,char *reporte , int *depo , int *tras, int *ret){

    FILE *writefile;
    int tipo = atoi(strtok(buffer,","));
    int cuenta1 = atoi(strtok(NULL,","));
    int cuenta2 = atoi(strtok(NULL,","));
    double monto = strtod(strtok(NULL,","),NULL);

    if(tipo == 1){

        if(!existuser(user,&n,cuenta1)){
            if(monto >= 0){
                    Deposito(user,n,cuenta1,monto);
                     (*depo)++;
            }else{
                char string[200] = "";
                sprintf(string,"Linea #%d El monto es menor a 0: %lf \n",(line+2),monto);
                strcat(reporte,string);
            }
        }else{
            char string[200] = "";
            sprintf(string,"Linea #%d La cuenta no existe: %d \n",(line+2),cuenta1);
            strcat(reporte,string);
        }   

    }else if(tipo == 2){

        if(!existuser(user,&n,cuenta1)){
            if(monto >= 0){
                    int ejec = Retiro(user,n,cuenta1,monto);

                    if(!ejec){
                        char string[200] = "";
                        sprintf(string, "Linea #%d La cuenta: %d no tiene suficiente saldo para el RETIRO\n",(line+2),cuenta1);
                        strcat(reporte,string);
                    }else{
                        (*ret)++;
                    }
        
            }else{
                char string[200] = "";
                sprintf(string,"Linea #%d El monto es menor a 0: %lf \n",(line+2),monto);
                strcat(reporte,string);
            }
          
        }else{
            char string[200] = "";
            sprintf(string,"Linea #%d La cuenta no existe: %d \n",(line+2),cuenta1);
            strcat(reporte,string);
        }

    }else if(tipo == 3){

        if(!existuser(user,&n,cuenta1)){
                if(!existuser(user,&n,cuenta2)){
                        if(monto >= 0){

                                int ejec = Retiro(user,n,cuenta1,monto);
                                if(ejec){
                                    Deposito(user,n,cuenta2,monto);
                                   (*tras)++;
                                }else{
                                    char string[200] = "";
                                    sprintf(string,"Linea #%d La cuenta: %d no tiene suficiente saldo para el RETIRO\n",(line+2),cuenta1);
                                    strcat(reporte,string);
                                }
                        }else{
                            char string[200] = "";
                            sprintf(string,"Linea #%d El monto es menor a 0: %lf \n",(line+2),monto);
                            strcat(reporte,string);
                        }
                }else{
                    char string[200] = "";
                    sprintf(string,"Linea #%d La cuenta no existe: %d \n",(line+2),cuenta2);
                    strcat(reporte,string);
                }
        }else{
            char string[200] = "";
            sprintf(string,"Linea #%d La cuenta no existe: %d \n",(line+2),cuenta1);
            strcat(reporte,string);
        }

    }else{
        char string[200] = "";
        sprintf(string,"Linea #%d El identificador no existe\n",(line+2));
        strcat(reporte,string);
    }


}