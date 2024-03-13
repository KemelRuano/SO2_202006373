#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
struct Usuarios {
    int no_cuenta;
    char nombre[25];
    double saldo;
   
};
struct Usuarios user[200];
int iter = 0;
bool existuser(int cuenta){
    for (int i = 0; i < iter; i++)
    {
        if (user[i].no_cuenta == cuenta){
            return false;
        }
    }
    return true;
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
// LLenado de estrucutra Usuario
void llenado(char *buffer , int line){
    FILE *writefile;
    int no_cuenta = atoi(strtok(buffer,","));
    
    char usuario[25];
    strcpy(usuario , strtok(NULL,","));
    
    double saldo;
    if(existuser(no_cuenta)){
        char *token;
        token = strtok(NULL,",");
        if(esNumero(token)){
            sscanf(token, "%lf",&saldo);
            if(saldo >= 0){
                user[iter].no_cuenta = no_cuenta;
                strcpy(user[iter].nombre, usuario);
                user[iter].saldo = saldo;
                iter++;
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


typedef struct {
    int hilo;
    int parte1;
    int parte2;
    int parte3;
    int tamano;
}CargaUsers;
int global = 0;

void* CargaUsuarios(void* args){
    CargaUsers *Carga = (CargaUsers *)args;
    char line[100];
    char split;
    int local = 0;
    
    FILE *archivo;
    archivo = fopen("prueba_usuarios.csv", "r");
    
    // leer cabecera para ignorar
    fgets(line, sizeof(line),archivo);
    int posactual = global ;
    while(local < Carga->tamano){
        fgets(line, sizeof(line),archivo);
        if(Carga->hilo == 0){
              if(local >= posactual && local < (posactual + Carga->parte1)){
                    llenado(line ,local);
                    global++;
              }
        }else if(Carga->hilo == 1){
            if(local >= posactual && local < (posactual + Carga->parte2)){
                    llenado(line , local);
                    global++;
              }
        }else if(Carga->hilo == 2){
            if(local >= posactual && local < (posactual + Carga->parte3)){
                    llenado(line , local);
                    global++;
              }
        }
        local++;
    }
   
    printf("HILO: %d \n",Carga->hilo);
}

int getLines(){
    FILE *archivo;
    archivo = fopen("prueba_usuarios.csv", "r");
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

int main() {
    while(true){
        int op;
        printf("1.  Carga Masiva Usuarios \n");
        printf("2.  Operaciones Individuales \n");
        printf("3.  Carga Masiva Operaciones \n");
        printf("4.  Reportes \n");
        printf("5.  Exit\n");
        printf("Ingrese una opcion: ");
        scanf("%d",&op);
        if(op == 1){
                // TIEMPO
                time_t tiempoActual;
                struct tm *infoTiempo;
                char buffer[80];
                time(&tiempoActual);
                infoTiempo = localtime(&tiempoActual);
                strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", infoTiempo);
                
                //Distribucion de hilos
                srand(time(NULL));
                int p1,p2,p3;
                p1 = rand() % getLines();
                p2 = rand() % (getLines() - p1);
                p3 = getLines() - p1 - p2;

                //CREAR REPORTE
                FILE *writefile;
                writefile = fopen("Reportes/usuarios.txt", "w");
                fprintf(writefile,"---------------------- CARGA DE USUARIOS ------------------------------\n");
                fprintf(writefile,"Fecha:  %s\n",buffer);
                fprintf(writefile,"Usuarios Cargados: \n");
                fprintf(writefile,"Hilo #1: %d\n",p1);
                fprintf(writefile,"Hilo #2: %d\n",p2);
                fprintf(writefile,"Hilo #3: %d\n",p3);
                fprintf(writefile,"Total:  %d\n",getLines());
                fprintf(writefile,"\n");
                fprintf(writefile,"Errores: \n");
                fclose(writefile);

                // CARGA EN 3 HILOS
                pthread_t threads[3];
                for (int i = 0; i < 3; i++){
                    CargaUsers *carga = malloc(sizeof(CargaUsers));
                    carga->hilo = i;
                    carga->parte1 = p1;
                    carga->parte2 = p2;
                    carga->parte3 = p3;
                    carga->tamano = getLines();
                    pthread_create(&threads[i], NULL, CargaUsuarios, (void *) carga);
                }

                for(int i = 0; i < 3; i++){pthread_join(threads[i], NULL);}   
            
        }else if (op == 2){
            for (int i = 0; i < iter; i++)
            {
               printf("cuenta: %d  nombre: %s  saldo: %f \n",user[i].no_cuenta,user[i].nombre,user[i].saldo);
            }
            
        }else if(op == 3){
            printf("MasivaOp\n");
        }else if (op == 4){
            printf("reportes\n");
        }else if(op == 5){
            break;
        }else{
            printf("!!!! opcion invalida !!!\n");
        }
    }
    
    return 0;
    
}
