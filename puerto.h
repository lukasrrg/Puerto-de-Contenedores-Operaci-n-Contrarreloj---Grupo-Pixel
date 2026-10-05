#ifndef PUERTO_H_INCLUDED
#define PUERTO_H_INCLUDED

#include "estructurasDeDatos.h"

#define TAM_COD_CONTENEDOR 5
#define TAM_COD_MUELLE 3
#define TAM_COD_BUQUE 5
#define TAM_COD_CAMION 5
#define TAM_COD_ZONA 3
#define TAM_MAX_NOMBRE_OPERADOR 6
#define MAX_LARGO_LINEA 200

#define TODO_OK 0

#define HAY_EVENTO_FUTURO 1
#define NO_HAY_EVENTO_FUTURO 0
//#define MUELLE_DISPONIBLE 1
//#define MUELLE_NO_DISPONIBLE 0

#define FIN_DE_JORNADA 0
#define JORNADA_ACTIVA 1

#define ARCHIVO_NO_ENCONTRADO 789



typedef struct
{
    char codigo[TAM_COD_CONTENEDOR];
} tContenedor;                              //Si no tiene ningun otro campo quizas es mas facil hacer directamente typedef char codigo[TAM_COD_CONTENEDOR] tContenedor

typedef struct
{
    char codigo[TAM_COD_BUQUE];
    tCola sigContenedor;
} tBuque;

typedef struct
{
    char codigo[TAM_COD_MUELLE];
    tBuque *buqueActual;            //Si vale NULL es porque esta disponible
//    int disponible; //Actua como booleano, 0 = no disponible, 1 = disponible      (SI DEFINIMOS EL CAMPOR buqueActual CREO QUE NO HACE FALTA)
} tMuelle;

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

typedef struct
{
    int tiempoAnterior;
    int tiempoActual;
    int puntuacionProvisoria;
    tLista listaMuelles;
    tCola colaBuques;
    tLista listaZonas;      // Lista dinámica de Zonas (donde cada zona tiene una tPila)
    tCola colaCamiones;
} tPuerto;                   //Variables de la partida en una sola estructura

typedef struct
{
    char tipo;              //Vale B si es un buque o K si es un camion
    int tiempo;
    tCola contenedores;
} tEventoFuturo;

//COMANDOS
//int comandosDisponibles(const tPuerto *puerto, tLista *listaComandos) Devuelve la cantidad de comandos posibles y guarda en la lista los comandos detectados como disponibles
void mostrarEstadoPuerto(const tPuerto *puerto); //VER

//Funciones de buques, camiones, contenedor, etc
void mostrarMuelle(const void *muelle);
void mostrarBuque(const void *buqueDato);
int actualizarBuquesCamiones(tPuerto *puerto);                      //Devuelve HAY_EVENTO_FUTURO o NO_HAY_EVENTO_FUTURO
int procesarLineaPuerto(char *linea, tPuerto *puerto);      //Devuelve HAY_EVENTO_FUTURO o NO_HAY_EVENTO_FUTURO
int muelleDisponible(const void *muelleDato, const void *nulo);
void devolverMuelle(void *dato, void *destino);
void atracarBuque(tMuelle *muelle, tBuque *buque);

#endif // PUERTO_H_INCLUDED
