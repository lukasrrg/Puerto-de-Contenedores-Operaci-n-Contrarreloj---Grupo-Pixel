#include "puerto.h"
#include "simulador.h"

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
    for (int i = 0; i < puerto->cantMuelles; i++)
    {
        printf("  - Muelle %s: ", puerto->muelles[i].codigo);
        if (puerto->muelles[i].disponible == 1)
        {
            printf("Disponible\n");
        }
        else
        {
            printf("Ocupado por Buque %s\n", puerto->muelles[i].buqueAtracado.codigo);
        }
    }
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
            tContenedor topeCont;
            memset(&topeCont, 0, sizeof(tContenedor));

            printf("  - Zona %s: ", zona->codigo);

            // Verificamos si la pila de la zona tiene contenedores o esta vacia
            if (pilaVacia(&zona->pila))
            {
                printf("[Vacia]\n");
            }
            else if (verTope(&zona->pila, &topeCont, sizeof(tContenedor)) == TODO_OK)
            {
                printf("Tope -> [%s]\n", topeCont.codigo);
            }
            else
            {
                printf("[Vacia]\n");
            }

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

// ENT
int entregarContenedor(tPuerto *puerto)
{
    tCamion camion;
    tNodo *nodoZonaActual = puerto->listaZonas;
    tZona *zonaActual;
    tContenedor contenedorActual;

    // 1. Validar si hay camiones en la cola de espera
    if (colaVacia(&puerto->colaCamiones))
    {
        return CONTENEDOR_NO_ENCONTRADO;
    }

    // 2. Inspeccionar el frente de la cola de camiones sin desencolarlo
    verPrimero(&puerto->colaCamiones, &camion, sizeof(tCamion));

    // 3. Recorrer la lista dinamica de zonas para buscar el contenedor solicitado
    while (nodoZonaActual != NULL)
    {
        zonaActual = (tZona *)nodoZonaActual->dato;

        // Validar que la pila de la zona actual no este vacia
        if (!pilaVacia(&zonaActual->pila))
        {
            // Consultar el tope de la pila de contenedores de la zona
            if (verTope(&zonaActual->pila, &contenedorActual, sizeof(tContenedor)) == TODO_OK)
            {
                // Verificar si el contenedor en el tope coincide con el buscado por el camion
                if (strcmp(contenedorActual.codigo, camion.contenedorBuscado.codigo) == 0)
                {
                    // Si coincide: Desapilar de la zona y desencolar el camion
                    sacarDePila(&zonaActual->pila, &contenedorActual, sizeof(tContenedor));
                    sacarDeCola(&puerto->colaCamiones, &camion, sizeof(tCamion));

                    // Sumar bonificacion y contabilizar operacion si aplica en tu struct
                    puerto->puntuacionProvisoria += 10;
                    puerto->operacionesRealizadas++;

                    return TODO_OK;
                }
            }
        }
        nodoZonaActual = nodoZonaActual->sig;
    }

    return CONTENEDOR_NO_ENCONTRADO;
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

    // 4. Extraer el contenedor de la cola del buque atracado
    tContenedor contenedorExtraido;
    memset(&contenedorExtraido, 0, sizeof(tContenedor));

    if (sacarDeCola(&puerto->muelles[idxMuelle].buqueAtracado.sigContenedor, &contenedorExtraido, sizeof(tContenedor)) != TODO_OK)
    {
        printf("\n[ERROR] El buque en %s no tiene mas contenedores para descargar.\n", codMuelle);
        return OPCION_NO_VALIDA;
    }

    // 5. Apilar en la zona destino
    ponerEnPila(&zonaDestino->pila, &contenedorExtraido, sizeof(tContenedor));

    // 6. Avanzar reloj T y sumar operacion
    puerto->tiempoActual += config->tiempo_descarga_contenedor;
    puerto->operacionesRealizadas++;

    printf("\n -> [DESCARGA OK] Contenedor %s descargado de %s a Zona %s (+%d min -> T=%d min).\n",
           contenedorExtraido.codigo, codMuelle, codZona,
           config->tiempo_descarga_contenedor, puerto->tiempoActual);

    // =========================================================================
    // OPCIÓN B: CHEQUEO DE ZARPE AUTOMÁTICO Y ATRACO DEL SIGUIENTE BUQUE EN COLA
    // =========================================================================
    if (colaVacia(&puerto->muelles[idxMuelle].buqueAtracado.sigContenedor))
    {
        printf(" -> [ZARPE] El Buque %s completo su descarga y zarpo del Muelle %s.\n",
               puerto->muelles[idxMuelle].buqueAtracado.codigo, codMuelle);

        puerto->muelles[idxMuelle].disponible = 1; // Muelle Libre

        // Si habia barcos esperando en la cola general, atraca inmediatamente el siguiente
        if (!colaVacia(&puerto->colaBuques))
        {
            tBuque buqueSiguiente;
            memset(&buqueSiguiente, 0, sizeof(tBuque));

            if (sacarDeCola(&puerto->colaBuques, &buqueSiguiente, sizeof(tBuque)) == TODO_OK)
            {
                puerto->muelles[idxMuelle].disponible = 0; // Ocupado
                puerto->muelles[idxMuelle].buqueAtracado = buqueSiguiente;

                printf(" -> [ATRACO AUTOMATICO] El Buque %s salio de la cola y atraco en Muelle %s.\n",
                       buqueSiguiente.codigo, codMuelle);
            }
        }
    }

    return TODO_OK;
}

//REU
int reubicar(tLista *listaZonas, const char *codZonaOrigen, const char *codZonaDestino)
{
    // 1. Localizar los nodos de ambas zonas en la lista dinamica
    tNodo *nodoRetirar = localizarZona(listaZonas, codZonaOrigen);
    tNodo *nodoInsertar = localizarZona(listaZonas, codZonaDestino);

    // 2. Validar existencia de ambos nodos ANTES de desreferenciarlos (evita Segmentation Fault)
    if (nodoRetirar == NULL || nodoInsertar == NULL)
    {
        printf("\n[ERROR] Una o ambas zonas (%s, %s) no existen.\n", codZonaOrigen, codZonaDestino);
        return OPCION_NO_VALIDA;
    }

    // 3. Castear de forma segura el puntero void* dato a (tZona *)
    tZona *zRetirar = (tZona *)nodoRetirar->dato;
    tZona *zInsertar = (tZona *)nodoInsertar->dato;

    // 4. Validar que la zona de origen no este vacia
    if (pilaVacia(&zRetirar->pila))
    {
        printf("\n[ERROR] La zona de origen %s esta vacia. No hay contenedores para reubicar.\n", codZonaOrigen);
        return OPCION_NO_VALIDA;
    }

    // 5. Validar que la zona de destino no este llena
    if (pilaLlena(&zInsertar->pila, sizeof(tContenedor)))
    {
        printf("\n[ERROR] La zona de destino %s esta llena.\n", codZonaDestino);
        return OPCION_NO_VALIDA;
    }

    // 6. Transferencia segura usando el TDA Pila
    tContenedor container;
    memset(&container, 0, sizeof(tContenedor));

    if (sacarDePila(&zRetirar->pila, &container, sizeof(tContenedor)) == TODO_OK)
    {
        ponerEnPila(&zInsertar->pila, &container, sizeof(tContenedor));

        // Actualizar capacidades relativas si las utilizas en tu struct tZona
        zRetirar->capacidad++;
        zInsertar->capacidad--;

        return TODO_OK;
    }

    return OPCION_NO_VALIDA;
}

//Funciones de buques, camiones, contenedor, etc
void mostrarMuelle(const void *muelleDato)
{
    const tMuelle *muelle = (const tMuelle*)muelleDato;

    printf("  - Muelle %s: ", muelle->codigo);
    if (muelle->disponible == 1)
        printf("Disponible\n");
    else
        printf("Ocupado por Buque %s\n", muelle->buqueAtracado.codigo);
}

void mostrarBuque(const void *buqueDato)
{
    const tBuque *buque = (tBuque*)buqueDato;

    tContenedor contenedor;
    verPrimero(&buque->sigContenedor, &contenedor, sizeof(tContenedor));
    printf("Buque %s: %s\n", buque->codigo, contenedor.codigo);

}

int actualizarBuquesCamiones(tPuerto *puerto)
{
    int eventoFuturo = NO_HAY_EVENTO_FUTURO;

    FILE *archivo = fopen("puerto.txt", "r");
    if (archivo == NULL)
        return ARCHIVO_NO_ENCONTRADO;

    char linea[MAX_LARGO_LINEA];

    while(fgets(linea, sizeof(linea), archivo) != NULL)
    {
        linea[strcspn(linea, "\n")] = '\0';     // Sacamos el '\n'

        if(linea[0] == 'B' || linea[0] == 'K')  //Se trabaja directamente sobre las lineas que empiezan por B o K
        {
            procesarLineaPuerto(linea, puerto);
        }
    }

    fclose(archivo);

    return eventoFuturo;
}

int procesarLineaPuerto(char *linea, tPuerto *puerto)
{
    char *token;
    char *codigo;
    char *codigoContenedor;
    char *valorT;
    char *contenedores;
    int tiempo;
    int eventoFuturo = NO_HAY_EVENTO_FUTURO;

    tMuelle *muelle = NULL;
    tBuque *buque;
    tCamion *camion;
    tContenedor *contenedor = (tContenedor*)malloc(sizeof(tContenedor));
    if (contenedor == NULL)
        return ERROR_SIN_MEM;

    token = strtok(linea, ";");     //Busco el primer campo B00X o K00X
    if(token == NULL)
        return eventoFuturo;

    codigo = token;                 //Guardo el codigo

    token = strtok(NULL, ";");      //Busco el segundo campo: T=X
    if(token == NULL)
        return eventoFuturo;

    valorT = token + 2;             //Salteamos "T="
    tiempo = atoi(valorT);

    token = strtok(NULL, ";");      //Busco el tercer campo: C=C10X,C10X,C10X...
    if(token == NULL)
        return eventoFuturo;

    contenedores = token + 2;       //Salteamos "C="

    if(tiempo > puerto->tiempoAnterior)
    {
        if(tiempo <= puerto->tiempoActual)
        {
            if(codigo[0] == 'B')
            {
                buque = (tBuque*)malloc(sizeof(tBuque));
                if (buque == NULL)
                    return ERROR_SIN_MEM;

                strcpy(buque->codigo, codigo);
                crearCola(&buque->sigContenedor);

                codigoContenedor = strtok(contenedores, ",");   //Saco el codigo del contenedor

                while(codigoContenedor != NULL)
                {
                    strcpy(contenedor->codigo, codigoContenedor);        //Guardo en la estructura tContenedor

                    ponerEnCola(&buque->sigContenedor, contenedor, sizeof(tContenedor));

                    codigoContenedor = strtok(NULL, ",");
                }

                if(buscarEnLista(&puerto->listaMuelles, NULL, &muelle, muelleDisponible, devolverMuelle))    //Busco si hay muelle disponible
                {
                    atracarBuque(muelle, buque);        //Si hay, atraco
                }
                else
                    ponerEnCola(&puerto->colaBuques, buque, sizeof(tBuque));    //Si no hay, a la cola de espera
            }
            else if(codigo[0] == 'K')
            {
                camion = (tCamion*)malloc(sizeof(tCamion));

                strcpy(camion->codigo, codigo);

                codigoContenedor = strtok(contenedores, ",");

                if(codigoContenedor != NULL)
                {
                    strcpy(camion->contenedorBuscado.codigo, codigoContenedor);
                }

                ponerEnCola(&puerto->colaCamiones, camion, sizeof(tCamion));   //Guardo el camion en la cola
            }
        }
        else
            eventoFuturo = HAY_EVENTO_FUTURO;
    }

    return eventoFuturo;
}

int muelleDisponible(const void *muelleDato, const void *nulo)
{
    const tMuelle *muelle = (const tMuelle*)muelleDato;

    return (muelle->disponible == 1);
}

void devolverMuelle(void *dato, void *destino)
{
    *(tMuelle **)destino = dato;
}

void atracarBuque(tMuelle *muelle, tBuque *buque)
{
    muelle->disponible = 0;
    muelle->buqueAtracado = *buque;
}

void liberarMemoriaPuerto(tPuerto *puerto)
{
    // 1. Liberar pilas de contenedores de cada zona y la lista de zonas
    tNodo *nodoZona = puerto->listaZonas;
    while (nodoZona != NULL)
    {
        tZona *z = (tZona *)nodoZona->dato;
        vaciarPila(&z->pila); // Vacía los contenedores apilados
        nodoZona = nodoZona->sig;
    }
    vaciarLista(&puerto->listaZonas);

    // 2. Liberar contenedores pendientes dentro de los buques en cola
    while (!colaVacia(&puerto->colaBuques))
    {
        tBuque b;
        sacarDeCola(&puerto->colaBuques, &b, sizeof(tBuque));
        vaciarCola(&b.sigContenedor); // Vacía la cola interna de contenedores del buque
    }
    vaciarCola(&puerto->colaBuques);

    // 3. Liberar cola de camiones
    vaciarCola(&puerto->colaCamiones);

    printf("\n[INFO] Memoria del puerto liberada correctamente en su totalidad.\n");
}
