#include "puerto.h"

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




//Funciones de buques, camiones, contenedor, etc
void mostrarMuelle(const void *muelleDato)
{
    const tMuelle *muelle = (tMuelle*)muelleDato;

    printf("  - Muelle %s: ", muelle->codigo);
    if (muelle->buqueActual == NULL)
        printf("Disponible\n");
    else
    {
        mostrarBuque(muelle->buqueActual);
    }
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
    tMuelle *muelle = (tMuelle*)muelleDato;

    return !(muelle->buqueActual == NULL);
}

void devolverMuelle(void *dato, void *destino)
{
    *(tMuelle **)destino = dato;
}

void atracarBuque(tMuelle *muelle, tBuque *buque)
{
    muelle->buqueActual = buque;
}
