#include "puerto.h"
#include "simulador.h"

///COMANDOS


//int comandosDisponibles(const tPuerto *puerto, tLista *listaComandos)
//{
//    criterios para saber si un comando esta disponible:
//        SALIR, VER y ESP: SIEMPRE
//        REU --> si existe al menos una zona de almacenamiento con al menos 1 espacio libre
//        DES --> si existe buque en algun muelle y espacio en la zona de almacenamiento
//        ENT --> si el contenedor que pide el primer camion de la cola esta en el tope de alguna zona de almacenamiento
//}

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

//DES
int descargar(tPuerto *puerto, const tConfiguracion *config, const char *codMuelle, const char *codZona)
{
    // 1. Buscar el muelle ingresado
    int idxMuelle = -1;
    for (int i = 0; i < puerto->cantMuelles; i++)
    {
        if (strcmp(puerto->muelles[i].codigo, codMuelle) == 0)
        {
            idxMuelle = i;
            break;
        }
    }

    if (idxMuelle == -1 || puerto->muelles[idxMuelle].disponible == 1)
    {
        printf("\n[ERROR] El muelle %s esta libre o no existe.\n", codMuelle);
        return OPCION_NO_VALIDA;
    }

    // 2. Localizar la zona destino
    tNodo *nodoZona = localizarZona(&puerto->listaZonas, codZona);
    if (nodoZona == NULL)
    {
        printf("\n[ERROR] La zona %s no existe.\n", codZona);
        return OPCION_NO_VALIDA;
    }

    tZona *zonaDestino = (tZona *)nodoZona->dato;

    // 3. Validar capacidad de la pila de la zona
    if (pilaLlena(&zonaDestino->pila, sizeof(tContenedor)))
    {
        printf("\n[ERROR] La Zona %s esta llena.\n", codZona);
        return OPCION_NO_VALIDA;
    }

    // 4. Extraer el contenedor de la cola del buque
    tContenedor contenedorExtraido;
    memset(&contenedorExtraido, 0, sizeof(tContenedor));

    if (sacarDeCola(&puerto->muelles[idxMuelle].buqueAtracado.sigContenedor, &contenedorExtraido, sizeof(tContenedor)) != TODO_OK)
    {
        printf("\n[ERROR] El buque en %s no tiene mas contenedores para descargar.\n", codMuelle);
        return OPCION_NO_VALIDA;
    }

    // 5. Apilar en la zona destino
    ponerEnPila(&zonaDestino->pila, &contenedorExtraido, sizeof(tContenedor));

    // 6. Avanzar reloj T
    puerto->tiempoActual += config->tiempo_descarga_contenedor;

    printf("\n -> [DESCARGA OK] Contenedor %s descargado de %s a Zona %s (+%d min -> T=%d min).\n",
           contenedorExtraido.codigo, codMuelle, codZona,
           config->tiempo_descarga_contenedor, puerto->tiempoActual);

    return TODO_OK;
}

//REU
/*int reubicar(tLista *listaZonas, const char codZona)
{
    tNodo* zonaInsertar, *zonaRetirar;
    char container[4];
    zonaRetirar=localizarZona(listaZonas,codZona+4);//recibe "Zx Zy", pero compara solamente los promeros 2 caracteres
    zonaInsertar=localizarZona(listaZonas,codZona+7); //recibe "Zy" y lo compara con las zonas de la lista para posicionar a zonaRetirar
    if (((*tZona)zonaInsertar->dato->capacidad>0)&& pilaVacia(&(*tZona)zonaRetirar->dato->pila)==0)//verifica si hay espacio en una pila y si hay elementos en la otra
    {
        sacarDePila(&(*tZona)zonaRetirar->dato->pila,container,sizeof(container));
        ponerEnPila(&(*tZona)zonaInsertar->dato->pila,container,sizeof(container));
        return 1;
    }
    return 0;
}
*/

void mostrarMuelle(const void *muelleDato)
{
    const tMuelle *muelle = muelleDato;

    printf("  - Muelle %s: ", muelle->codigo);
    if (muelle->disponible)
        printf("Disponible\n");
    else
        printf("No disponible\n");
}

//actualizarBuques(puerto, archivo)
//{
//    leer archivo y comparar con el tiempo actual a ver si llega algun buque
//      if (hay buque)
    //      if (no hay muelle disponible)
//          {
//            insertar buque en cola de espera del puerto
//          }
//          else
//          {
//            asignar a muelle
//          }
//}
//
////actualizarCamiones(puerto, archivo)
//{
//    leer archivo y comprar con el tiempo actual a ver si llega algun camion
//        if (hay camion)
//            insertar en cola de espera
//}
