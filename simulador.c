#include "simulador.h"

//Pantallas del juego
void pantallaInicial(int *estadoDeJuego)
{
    printf("Puerto de Contenedores - Operacion Contrarreloj\nGrupo Pixel\n");
    printf("\nIngrese...");
    printf("\n1 para JUGAR");
    printf("\n2 para ver ESTADISTICAS");
    printf("\n0 para SALIR");

    do
    {
        printf("\n...");
        scanf("%d", estadoDeJuego);
        if(*estadoDeJuego < SALIR || *estadoDeJuego > ESTADISTICAS)
            printf("Opcion no valida");
    } while(*estadoDeJuego < SALIR || *estadoDeJuego > ESTADISTICAS);
}

//Funciones configuracion
int obtenerValorConfiguracion(char *cadena, FILE *archivo)
{
    int valor;

    fseek(archivo, strlen(cadena), SEEK_CUR);   //Avanzo en la cantidad de caracteres de la cadena
    fscanf(archivo, "%d", &valor);              //Guardo el int
    fseek(archivo, 1, SEEK_CUR);                //Avanzo para saltarme el \n

    return valor;
}

int leerArchivoConfiguracion(tConfiguracion *configuracion)
{
    FILE *archivo = fopen("config.txt", "r");
    if(archivo == NULL)
        return ARCHIVO_NO_ENCONTRADO;

    configuracion->cap_max_pila = obtenerValorConfiguracion("cap_max_pila ", archivo);
    configuracion->tiempo_descarga_contenedor = obtenerValorConfiguracion("tiempo_descarga_contenedor ", archivo);
    configuracion->tiempo_reubicacion_contenedor = obtenerValorConfiguracion("tiempo_reubicacion_contenedor ", archivo);
    configuracion->tiempo_carga_camion = obtenerValorConfiguracion("tiempo_carga_camion ", archivo);
    configuracion->duracion_jornada_minutos = obtenerValorConfiguracion("duracion_jornada_minutos ", archivo);
    configuracion->cantidad_muelles = obtenerValorConfiguracion("cantidad_muelles ", archivo);
    configuracion->cantidad_zonas = obtenerValorConfiguracion("cantidad_zonas ", archivo);
    configuracion->maximo_buques = obtenerValorConfiguracion("maximo_buques ", archivo);
    configuracion->maximo_camion = obtenerValorConfiguracion("maximo_camion ", archivo);
    configuracion->maximo_contenedores_por_buque = obtenerValorConfiguracion("maximo_contenedores_por_buque ", archivo);

    fclose(archivo);

    return TODO_OK;
}

void configuracionMostrar(tConfiguracion *config)
{
    printf("cap_max_pila %d\n", config->cap_max_pila);
    printf("tiempo_descarga_contenedor %d\n", config->tiempo_descarga_contenedor);
    printf("tiempo_reubicacion_contenedor %d\n", config->tiempo_reubicacion_contenedor);
    printf("tiempo_carga_camion %d\n", config->tiempo_carga_camion);
    printf("duracion_jornada_minutos %d\n", config->duracion_jornada_minutos);
    printf("cantidad_muelles %d\n", config->cantidad_muelles);
    printf("cantidad_zonas %d\n", config->cantidad_zonas);
    printf("maximo_buques %d\n", config->maximo_buques);
    printf("maximo_camion %d\n", config->maximo_camion);
    printf("maximo_contenedores_por_buque %d\n", config->maximo_contenedores_por_buque);
}
