#include "operaciones.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void MenuOp(Usuarios *user , int size){
    int cuenta;
    double monto;
    while(true){
        int op;
        printf("1.  Deposito \n");
        printf("2.  Retiro \n");
        printf("3.  Transacciones \n");
        printf("4.  Consultar Cuenta \n");
        printf("5.  Regresar \n");
        printf("Ingrese una opcion: ");
        scanf("%d",&op);
        
        if(op == 1){
            system("clear");
            cuenta = 0;
            monto = 0;
            printf("********** DEPOSITO **********\n");
            printf("> Ingrese No. Cuenta : ");
            if (scanf("%d",&cuenta) == 1){
                if(!existuser(user,&size,cuenta)){
                     printf("> Monto: ");
                     if(scanf("%lf",&monto) == 1 && monto >= 0){
                         Deposito(user,size,cuenta,monto);
                         printf(" DEPOSITO EXITOSO \n");
                     }else{ printf("\033[1;31m"); printf("EL MONTO NO ES UN NUMERO O ES MENOR A 0\n");  printf("\033[0m"); while (getchar() != '\n');}
                }else{ printf("\033[1;31m"); printf("|| NO EXISTE LA CUENTA || \n");  printf("\033[0m"); }
            }else{ printf("\033[1;31m"); printf("LA CUENTA NO ES UN NUMERO\n");  printf("\033[0m"); while (getchar() != '\n');}
            printf("\n");

        }else if (op == 2){
             system("clear");
            cuenta = 0;
            monto = 0;
            printf("********** RETIRO **********\n");
            printf("> Ingrese No. Cuenta : ");
            if (scanf("%d",&cuenta) == 1){
                if(!existuser(user,&size,cuenta)){
                     printf("> Monto: ");
                     if(scanf("%lf",&monto) == 1 && monto >= 0){
                         int exitoso = Retiro(user,size,cuenta,monto);
                         if(exitoso){
                                printf(" RETIRO EXITOSO \n");
                         }else{
                                printf("\033[1;31m"); 
                                printf(" LA CUENTA NO CUENTA CON SUFICIENTE SALDO PARA EL RETIRO \n");
                                printf("\033[0m");
                         }
                     }else{ printf("\033[1;31m");  printf("EL MONTO NO ES UN NUMERO O ES MENOR A 0\n");  printf("\033[0m"); while (getchar() != '\n');}
                }else{ printf("\033[1;31m");  printf("|| NO EXISTE LA CUENTA || \n");  printf("\033[0m");}
            }else{ printf("\033[1;31m");  printf("LA CUENTA NO ES UN NUMERO\n");  printf("\033[0m"); while (getchar() != '\n');}
            printf("\n");

        }else if (op == 3){
            system("clear");
            cuenta = 0;
            monto = 0;
            int cuenta2 = 0;
            printf("********** TRANSACCION **********\n");
            printf("> Cuenta de Retiro : ");
            if (scanf("%d",&cuenta) == 1){
                 if(!existuser(user,&size,cuenta)){
                    printf("> Cuenta de Destino : ");
                    if (scanf("%d",&cuenta2) == 1){
                         if(!existuser(user,&size,cuenta2)){
                            printf("> Monto a Depositar: ");
                            if(scanf("%lf",&monto) == 1 && monto >= 0){

                                int ejec = Retiro(user,size,cuenta,monto);
                                if(ejec){
                                    Deposito(user,size,cuenta2,monto);
                                    printf("TRANSACCION EXITOSA \n");
                                }else{
                                     printf("\033[1;31m"); 
                                     printf(" LA CUENTA : %d NO CUENTA CON SALDO SUFICIENTE \n",cuenta);
                                     printf("\033[0m");
                                }


                            }else{ printf("\033[1;31m"); printf("EL MONTO NO ES UN NUMERO O ES MENOR A 0\n");  printf("\033[0m"); while (getchar() != '\n');}
                         }else{printf("\033[1;31m"); printf("|| NO EXISTE LA CUENTA || \n"); printf("\033[0m"); }
                    }else{ printf("\033[1;31m"); printf("LA CUENTA NO ES UN NUMERO\n");  printf("\033[0m"); while (getchar() != '\n');}
                 }else{ printf("\033[1;31m"); printf("|| NO EXISTE LA CUENTA || \n"); printf("\033[0m"); }
            }else{ printf("\033[1;31m"); printf("LA CUENTA NO ES UN NUMERO\n");  printf("\033[0m"); while (getchar() != '\n');}
            printf("\n");

        }else if (op == 4){
             system("clear");
            cuenta = 0;
            printf("********** ESTADO DE CUENTA **********\n");
            printf("> Ingrese No. Cuenta : ");
            if (scanf("%d",&cuenta) == 1){
                if(!existuser(user,&size,cuenta)){
                    for (int i = 0; i < size; i++){
                        if(user[i].no_cuenta == cuenta){
                             printf("\n");
                             printf("Cuenta: %d \n",user[i].no_cuenta );
                             printf("Nombre: %s \n",user[i].nombre );
                             printf("Saldo: %lf \n",user[i].saldo );
                             printf("\n");
                        }
                    }
                }else{
                    printf("\033[1;31m");
                    printf("|| NO EXISTE LA CUENTA || \n");
                    printf("\033[0m");
                }
             }else{ printf("\033[1;31m"); printf("LA CUENTA NO ES UN NUMERO\n"); printf("\033[0m"); while (getchar() != '\n');}

        }else if(op == 5){
            system("clear");
            break;
        }else{
            printf("\033[1;31m");
            printf("!!!! opcion invalida !!!\n");
            printf("\033[0m");
        }
    };
}       