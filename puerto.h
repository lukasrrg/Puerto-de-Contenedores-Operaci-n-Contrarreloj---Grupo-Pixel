#ifndef PUERTO_H_INCLUDED
#define PUERTO_H_INCLUDED

#include "estructurasDeDatos.h"

#define TAM_COD_CONTENEDOR 5
#define TAM_COD_MUELLE 15
#define TAM_COD_BUQUE 5
#define TAM_COD_CAMION 5
#define TAM_COD_ZONA 15
#define TAM_MAX_NOMBRE_OPERADOR 6
#define MAX_LARGO_LINEA 200

#define TODO_OK 0

#define HAY_EVENTO_FUTURO 1
#define NO_HAY_EVENTO_FUTURO 0
//#define MUELLE_DISPONIBLE 1
//#define MUELLE_NO_DISPONIBLE 0
#define CONTENEDOR_NO_ENCONTRADO 51

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
    int tiempoAnterior;
    int tiempoActual;
    int puntuacionProvisoria;
    tLista listaMuelles;
    tMuelle muelles[10];     // <--- Arreglo de muelles
    int cantMuelles;         // <--- Cantidad de muelles
    tCola colaBuques;
    tLista listaZonas;      // Lista dinÃ¡mica de Zonas (donde cada zona tiene una tPila)
    tCola colaCamiones;
    int operacionesRealizadas;
} tPuerto;                   //Variables de la partida en una sola estructura

typedef struct
{
    char tipo;              //Vale B si es un buque o K si es un camion
    int tiempo;
    tCola contenedores;
} tEventoFuturo;

#include "simulador.h" //Este include es para que lo tome a tPuerto en simulador. No mover

//COMANDOS
//int comandosDisponibles(const tPuerto *puerto, tLista *listaComandos) Devuelve la cantidad de comandos posibles y guarda en la lista los comandos detectados como disponibles
void mostrarEstadoPuerto(const tPuerto *puerto); //VER
int entregarContenedor(tPuerto *puerto); //ENT
int descargar(tPuerto *puerto, const tConfiguracion *config, const char *codMuelle, const char *codZona);//DES
int reubicar(tLista *listaZonas, const char *codZonaOrigen, const char *codZonaDestino); //REU

//Funciones de buques, camiones, contenedor, etc
void mostrarMuelle(const void *muelle);
void mostrarBuque(const void *buqueDato);
int actualizarBuquesCamiones(tPuerto *puerto);                      //Devuelve HAY_EVENTO_FUTURO o NO_HAY_EVENTO_FUTURO
int procesarLineaPuerto(char *linea, tPuerto *puerto);      //Devuelve HAY_EVENTO_FUTURO o NO_HAY_EVENTO_FUTURO
int muelleDisponible(const void *muelleDato, const void *nulo);
void devolverMuelle(void *dato, void *destino);
void atracarBuque(tMuelle *muelle, tBuque *buque);
void liberarMemoriaPuerto(tPuerto *puerto);

#endif // PUERTO_H_INCLUDED
