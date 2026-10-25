#ifndef SIMULADOR_H_INCLUDED
#define SIMULADOR_H_INCLUDED

#include "puerto.h"

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


//Pantallas de juego
void pantallaInicial(int *estadoDeJuego);
int ingresarNombreOperador(char *nombreDestino);
void jornadaOperativa(tPuerto *puerto, tConfiguracion *configuracion, char *nombreOp);

//Funciones configuracion
int obtenerValorConfiguracion(char *cadena, FILE *archivo);
int leerArchivoConfiguracion(tConfiguracion *configuracion);
void configuracionMostrar(tConfiguracion *config);
void inicializarPuerto(tPuerto *puerto, tConfiguracion *configuracion);
void inicializarMuelles(tLista *listaMuelles, int cantidad);

//Juego

//Funciones auxiliares
void stringMayuscula(char *str);

#endif // SIMULADOR_H_INCLUDED
