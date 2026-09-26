#ifndef PUERTO_H_INCLUDED
#define PUERTO_H_INCLUDED

#include "estructurasDeDatos.h"

#define TAM_COD_CONTENEDOR 5
#define TAM_COD_MUELLE 3
#define TAM_COD_BUQUE 5
#define TAM_COD_CAMION 5
#define TAM_COD_ZONA 3
#define TAM_MAX_NOMBRE_OPERADOR 6

#define TODO_OK 0

typedef struct
{
    char codigo[TAM_COD_CONTENEDOR];
} tContenedor;                              //Si no tiene ningun otro campo quizas es mas facil hacer directamente typedef char codigo[TAM_COD_CONTENEDOR] tContenedor

typedef struct
{
    char codigo[TAM_COD_MUELLE];
    int disponible; //Actua como booleano, 0 = no disponible, 1 = disponible
} tMuelle;

typedef struct
{
    char codigo[TAM_COD_BUQUE];
    //tPila o tCola sigContenedor;      Decidir si se usa con pila o con cola
} tBuque;

typedef struct
{
    char codigo[TAM_COD_CAMION];
    tContenedor contenedorBuscado;      //Depende de como lo implementemos quizas conviene guardar solo el codigo y no la estructura entera
} tCamion;

typedef struct
{
    char codigo[TAM_COD_ZONA];
} tZona;                            //Si no tiene ningun otro campo quizas es mas facil hacer directamente typedef char codigo[TAM_COD_ZONA] tZona
                                    //La Zona no deberia tener tambien una Pila de Contenedores?
typedef struct
{
    char nombre[TAM_MAX_NOMBRE_OPERADOR];
    int partidasJugadas;
    int maxPuntaje;
} tOperador;








#endif // PUERTO_H_INCLUDED
