#ifndef PUERTO_H_INCLUDED
#define PUERTO_H_INCLUDED

#include "estructurasDeDatos.h"


#define TAM_COD_CONTENEDOR 5
#define TAM_COD_MUELLE 10
#define TAM_COD_BUQUE 5
#define TAM_COD_CAMION 5
#define TAM_COD_ZONA 15
#define TAM_MAX_NOMBRE_OPERADOR 6

#define TODO_OK 0

#define MUELLE_DISPONIBLE 1
#define MUELLE_NO_DISPONIBLE 0

#define FIN_DE_JORNADA 0
#define JORNADA_ACTIVA 1

typedef struct
{
    char codigo[TAM_COD_CONTENEDOR];
} tContenedor;                              //Si no tiene ningun otro campo quizas es mas facil hacer directamente typedef char codigo[TAM_COD_CONTENEDOR] tContenedor

typedef struct
{
    char codigo[TAM_COD_BUQUE];
    tCola sigContenedor;
    //tPila o tCola sigContenedor;      Decidir si se usa con pila o con cola
} tBuque;

typedef struct
{
    char codigo[TAM_COD_MUELLE];
    int disponible; //Actua como booleano, 0 = no disponible, 1 = disponible
    tBuque buqueAtracado;  // <--- Agregado
} tMuelle;

typedef struct
{
    char codigo[TAM_COD_CAMION];
    tContenedor contenedorBuscado;      //Depende de como lo implementemos quizas conviene guardar solo el codigo y no la estructura entera
} tCamion;

typedef struct
{
    tPila pila;
    int capacidad;
    char codigo [TAM_COD_ZONA];
} tZona;                            //cada zona tiene su pila, código de zona y capacidad

typedef struct
{
    char nombre[TAM_MAX_NOMBRE_OPERADOR];
    int partidasJugadas;
    int maxPuntaje;
} tOperador;


typedef struct
{
    int tiempoActual;
    int puntuacionProvisoria;
    tLista listaMuelles;
    tMuelle muelles[10];     // <--- Arreglo de muelles
    int cantMuelles;         // <--- Cantidad de muelles
    tCola colaBuques;
    tLista listaZonas;      // Lista dinámica de Zonas (donde cada zona tiene una tPila)
    tCola colaCamiones;
} tPuerto;                   //Variables de la partida en una sola estructura

#include "simulador.h" //Este include es para que lo tome a tPuerto en simulador. No mover

//COMANDOS
//int comandosDisponibles(const tPuerto *puerto, tLista *listaComandos) Devuelve la cantidad de comandos posibles y guarda en la lista los comandos detectados como disponibles
void mostrarEstadoPuerto(const tPuerto *puerto); //VER
int descargar(tPuerto *puerto, const tConfiguracion *config, const char *codMuelle, const char *codZona);
int reubicar(tLista *listaZonas, const char codZona); //REU

void mostrarMuelle(const void *muelle);

#endif // PUERTO_H_INCLUDED
