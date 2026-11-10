#ifndef SIMULADOR_H_INCLUDED
#define SIMULADOR_H_INCLUDED

#include "estructurasDeDatos.h"

//Estados de juego
#define PANTALLA_INICIAL -1
#define SALIR 0
#define JUGAR 1
#define ESTADISTICAS 2

#define OPCION_VALIDA 1
#define OPCION_NO_VALIDA 0

#define LONG_VALOR_CONFIG 4

typedef struct
{
    int     cap_max_pila,
            tiempo_descarga_contenedor,
            tiempo_reubicacion_contenedor,
            tiempo_carga_camion,
            duracion_jornada_minutos,
            cantidad_muelles,
            cantidad_zonas,
            maximo_buques,
            maximo_camion,
            maximo_contenedores_por_buque;
} tConfiguracion;

#include "puerto.h" //Este include es para que tome tConfiguracion en puerto (No mover)

//Pantallas de juego
void pantallaInicial(int *estadoDeJuego);
int ingresarNombreOperador(char *nombreDestino);
void jornadaOperativa(tPuerto *puerto, tConfiguracion *configuracion, char *nombreOp);
void mostrarPantallaEstadisticas(void);
void actualizarEstadisticasOperador(const char *nombreOp, int puntajeObtenido);

//Funciones configuracion
int obtenerValorConfiguracion(char *cadena, FILE *archivo);
int leerArchivoConfiguracion(tConfiguracion *configuracion);
void configuracionMostrar(tConfiguracion *config);
void inicializarPuerto(tPuerto *puerto, tConfiguracion *configuracion);
void inicializarMuelles(tLista *listaMuelles, int cantidad);
void crearZonas(tLista *listaZonas, int cantidadZonas, int capacidadZonas);

//Auxiliares para el juego
tNodo* localizarZona(tLista *listaZonas, const char *codZona);
int verificarZonas(const char *codzona, tLista *listaZonas);
int procesarArribosDelMinuto(FILE *archEscenario, tPuerto *puerto);


//Funciones auxiliares
void stringMayuscula(char *str);
int cmpCodZona(const void* c1, const void* c2);
int cmpOperadorNombre(const void *e1, const void *e2);
void mostrarOperador(const void *dato);

#endif // SIMULADOR_H_INCLUDED
