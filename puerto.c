#include "puerto.h"

///COMANDOS

//VER
void mostrarEstadoPuerto(const tPuerto *puerto)
{
    tNodo *nodoZonaActual;
    tZona *zona;
    tNodo *nodoCamion;
    tCamion *camion;

    printf("\n=================== ESTADO DEL PUERTO ===================\n");
    printf(" Tiempo Actual (T): %d min\n", puerto->tiempoActual);
    printf(" Puntuacion Provisoria: %d pts\n", puerto->puntuacionProvisoria);
    printf("---------------------------------------------------------\n");

    // 1. Mostrar Muelles
    printf(" MUELLES\n");
    //Mostrar lista de muelles
    mostrarLista(&puerto->listaMuelles, mostrarMuelle);
    printf("  Buques esperando en cola: %s\n", colaVacia(&puerto->colaBuques) ? "Ninguno" : "Si");

    // 2. Mostrar Zonas de Almacenamiento
    printf("\n ZONAS DE ALMACENAMIENTO\n");
    nodoZonaActual = puerto->listaZonas;

    if (nodoZonaActual == NULL)
    {
        printf("  No hay zonas configuradas.\n");
    }
    else
    {
        while (nodoZonaActual != NULL)
        {
            zona = (tZona *)nodoZonaActual->dato;

            // Como tZona no tiene una Pila en el .h, solo imprimimos su código.
            printf("  - Zona %s\n", zona->codigo);

            nodoZonaActual = nodoZonaActual->sig;
        }
    }

    // 3. Ventana de Planificacion (Hasta 3 camiones sin alterar la Cola)
    printf("\n VENTANA DE PLANIFICACION - CAMIONES EN ESPERA\n");
    if (colaVacia(&puerto->colaCamiones))
    {
        printf("No hay camiones esperando.\n");
    }
    else
    {
        nodoCamion = puerto->colaCamiones.pri;
        int contador = 1;

        while (nodoCamion != NULL && contador <= 3)
        {
            camion = (tCamion *)nodoCamion->dato;

            // Accedemos correctamente al string dentro del struct anidado
            printf("  %d) Camion %s -> Solicita Contenedor %s %s\n",
                   contador,
                   camion->codigo,
                   camion->contenedorBuscado.codigo,
                   (contador == 1) ? "<- FRENTE A ATENDER" : "");

            nodoCamion = nodoCamion->sig;
            contador++;
        }
    }
    printf("=========================================================\n\n");
}

void mostrarMuelle(const void *muelleDato)
{
    const tMuelle *muelle = muelleDato;

    printf("  - Muelle %s: ", muelle->codigo);
    if (muelle->disponible)
        printf("Disponible\n");
    else
        printf("No disponible\n");
}
