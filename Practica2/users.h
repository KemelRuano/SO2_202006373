#ifndef USERS_H
#define USERS_H
#include <stdbool.h>

int getLines(char namefile[]);
char* DateTimeString();
bool esNumero(const char *cadena);
typedef struct {
    int no_cuenta;
    char nombre[25];
    double saldo;
}Usuarios;

typedef struct {
    int hilo;
    int parte1;
    int parte2;
    int parte3;
    int tamano;
}Control3H;

typedef struct {
    int hilo;
    int parte1;
    int parte2;
    int parte3;
    int parte4;
    int tamano;
}Control4H;

void LLenarUsuarios(Usuarios *user, int *n , char *buffer , int line);
bool existuser(Usuarios *user , int *n ,  int cuenta);
void Deposito(Usuarios *user, int n , int cuenta , double monto);
bool Retiro(Usuarios *user, int size , int cuenta , double monto);
void Transaccion(Usuarios *user, int n , char *buffer , int line , char *reporte  ,int *depo,int *tras,int *retiro);
#endif