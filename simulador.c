#include "simulador.h"
#include "puerto.h"

//Pantallas del juego
void pantallaInicial(int *estadoDeJuego)
{
    int caracter;

    printf("Puerto de Contenedores - Operacion Contrarreloj\nGrupo Pixel\n");
    printf("\nIngrese...");
    printf("\n1 para JUGAR");
    printf("\n2 para ver ESTADISTICAS");
    printf("\n0 para SALIR");

    do
    {
        printf("\n...");
        scanf("%d", estadoDeJuego);

        // LIMPIEZA CLAVE DEL BUFFER: Limpia todo caracter sobrante incluido el '\n'
        while ((caracter = getchar()) != '\n' && caracter != EOF);

        if(*estadoDeJuego < SALIR || *estadoDeJuego > ESTADISTICAS)
            printf("Opcion no valida");
    }
    while(*estadoDeJuego < SALIR || *estadoDeJuego > ESTADISTICAS);
}

int ingresarNombreOperador(char *nombreDestino)
{
    char operador[100]; // Buffer local para almacenar la entrada cruda del usuario por teclado
    int esValido = OPCION_NO_VALIDA; // Flag de control del ciclo de validación
    char *pSalto;


    while (esValido == OPCION_NO_VALIDA) // Bucle de lectura: permanece solicitando el dato hasta que se cumplan las precondiciones
    {
        printf("\nIngrese su nombre (maximo %d caracteres): ", TAM_MAX_NOMBRE_OPERADOR - 1);

        // Se utiliza fgets para evitar desbordamientos de buffer por consola
        if (fgets(operador, sizeof(operador), stdin) != NULL)
        {
            // Se busca el salto de línea '\n' que genera la tecla Enter
            pSalto = strchr(operador, '\n');
            if (pSalto)
                *pSalto = '\0'; // Se reemplaza por terminador nulo para limpiar la cadena

            // Validación 1: Verificar si el operador ingresó un string vacío (sólo presionó Enter)
            if (strlen(operador) == 0)
                printf("[ERROR] No se ingreso nombre.\n");

            // Validación 2: Verificar si la longitud supera el límite permitido por la estructura tOperador
            else if (strlen(operador) >= TAM_MAX_NOMBRE_OPERADOR)
                printf("[ERROR] El nombre excede el limite de %d caracteres.\n", TAM_MAX_NOMBRE_OPERADOR - 1);

            // Entrada válida: Se copia al parámetro de salida y se habilita el fin del ciclo
            else
            {
                strcpy(nombreDestino, operador);
                esValido = OPCION_VALIDA;
            }
        }

        stringMayuscula(nombreDestino);
    }

    return TODO_OK;
}

void jornadaOperativa(tPuerto *puerto, tConfiguracion *configuracion, char *nombreOp)
{
    char lineaComando[100];
    char comando[10];
    int jornadaActiva = JORNADA_ACTIVA;
    int hayEventoFuturo = HAY_EVENTO_FUTURO;

    while (jornadaActiva && puerto->tiempoActual < configuracion->duracion_jornada_minutos)
    {
        system("cls");

        // Actualizar buques y camiones usando la funcion de tu compañero
        hayEventoFuturo = actualizarBuquesCamiones(puerto);
        (void)hayEventoFuturo; // Evita warning de variable no usada

        printf("\n<%s> ", nombreOp);

        if (fgets(lineaComando, sizeof(lineaComando), stdin) != NULL)
        {
            if (sscanf(lineaComando, "%s", comando) == 1)
            {
                stringMayuscula(comando);

                // VER
                if (strcmp(comando, "VER") == 0)
                {
                    mostrarEstadoPuerto(puerto);
                    printf("\nPresione Enter para continuar...");
                    fflush(stdin);
                    getchar();
                }
                // DES
                else if (strcmp(comando, "DES") == 0)
                {
                    char codMuelle[TAM_COD_MUELLE];
                    char codZona[TAM_COD_ZONA];

                    if (sscanf(lineaComando, "%*s %s %s", codMuelle, codZona) == 2)
                    {
                        stringMayuscula(codMuelle);
                        stringMayuscula(codZona);
                        descargar(puerto, configuracion, codMuelle, codZona);
                    }
                    else
                    {
                        printf("\n[ERROR] Sintaxis incorrecta. Uso: DES <muelle> <zona>\n");
                    }
                    printf("\nPresione Enter para continuar...");
                    fflush(stdin);
                    getchar();
                }
                // REU
                /*else if (strcmp(comando, "REU") == 0)
                {
                    char codZonaOrig[TAM_COD_ZONA];
                    char codZonaDest[TAM_COD_ZONA];

                    if (sscanf(lineaComando, "%*s %s %s", codZonaOrig, codZonaDest) == 2)
                    {
                        stringMayuscula(codZonaOrig);
                        stringMayuscula(codZonaDest);
                        if (reubicar(&puerto->listaZonas, codZonaOrig, codZonaDest) == TODO_OK)
                        {
                            puerto->tiempoActual += configuracion->tiempo_reubicacion_contenedor;
                        }
                    }
                    else
                    {
                        printf("\n[ERROR] Sintaxis incorrecta. Uso: REU <zonaOrigen> <zonaDestino>\n");
                    }
                    printf("\nPresione Enter para continuar...");
                    fflush(stdin);
                    getchar();
                }*/
                // ENT
                else if (strcmp(comando, "ENT") == 0)
                {
                    if (entregarContenedor(puerto) == TODO_OK)
                    {
                        puerto->tiempoActual += configuracion->tiempo_carga_camion;
                        puerto->puntuacionProvisoria += 10;
                    }
                    else
                    {
                        printf("\n[ERROR] No se pudo entregar el contenedor.\n");
                    }
                    printf("\nPresione Enter para continuar...");
                    fflush(stdin);
                    getchar();
                }
                // ESP
                else if (strcmp(comando, "ESP") == 0)
                {
                    puerto->tiempoActual++;
                }
                // SALIR
                else if (strcmp(comando, "SALIR") == 0)
                {
                    jornadaActiva = FIN_DE_JORNADA;
                }
                else
                {
                    printf("\n[ERROR] Comando no reconocido.\n");
                    printf("\nPresione Enter para continuar...");
                    fflush(stdin);
                    getchar();
                }
            }
        }

        if (puerto->tiempoActual >= configuracion->duracion_jornada_minutos)
        {
            printf("\n=== TIEMPO LIMITE ALCANZADO: FIN DE LA JORNADA (T = %d min) ===\n", puerto->tiempoActual);
            printf("\nPresione Enter para volver al menu principal...");
            fflush(stdin);
            getchar();
        }
    }
}

void mostrarPantallaEstadisticas(void)
{
    system("cls");
    printf("=================== RANKING DE OPERADORES ===================\n");
    printf("  Operador        | Partidas Jugadas | Maximo Puntaje\n");
    printf("-------------------------------------------------------------\n");

    FILE *archOps = fopen("operadores.dat", "rb");
    if (archOps == NULL)
    {
        printf("  [INFO] No hay estadisticas registradas todavia.\n");
    }
    else
    {
        tArbol arbolOps;
        crearArbol(&arbolOps);

        tOperador opBuffer;
        while (fread(&opBuffer, sizeof(tOperador), 1, archOps) == 1)
        {
            insertarEnArbol(&arbolOps, &opBuffer, sizeof(tOperador), cmpOperadorNombre);
        }
        fclose(archOps);

        // Recorre el ABB en In-Order (imprime en orden alfabetico)
        recorrerInorden(&arbolOps, mostrarOperador);
    }

    printf("=============================================================\n");
    printf("\nPresione Enter para volver al menu principal...");
    fflush(stdin);
    getchar();
}

void actualizarEstadisticasOperador(const char *nombreOp, int puntajeObtenido)
{
    tArbol arbolOps;
    crearArbol(&arbolOps);

    // 1. Cargar datos existentes desde operadores.dat
    FILE *archLectura = fopen("operadores.dat", "rb");
    if (archLectura != NULL)
    {
        tOperador opBuffer;
        while (fread(&opBuffer, sizeof(tOperador), 1, archLectura) == 1)
        {
            insertarEnArbol(&arbolOps, &opBuffer, sizeof(tOperador), cmpOperadorNombre);
        }
        fclose(archLectura);
    }

    // 2. Buscar si el operador ya existía en el árbol
    tOperador claveBusqueda;
    strncpy(claveBusqueda.nombre, nombreOp, sizeof(claveBusqueda.nombre) - 1);

    tNodoA **nodoEncontrado = buscarClave(&arbolOps, &claveBusqueda, cmpOperadorNombre);

    if (nodoEncontrado != NULL && *nodoEncontrado != NULL)
    {
        // Si YA EXISTÍA: incrementar partidas y actualizar récord si superó su marca
        tOperador *opExistente = (tOperador *)((*nodoEncontrado)->dato);
        opExistente->partidasJugadas++;
        if (puntajeObtenido > opExistente->maxPuntaje)
        {
            opExistente->maxPuntaje = puntajeObtenido;
        }
    }
    else
    {
        // Si ES NUEVO: crear registro e insertarlo
        tOperador nuevoOp;
        memset(&nuevoOp, 0, sizeof(tOperador));
        strncpy(nuevoOp.nombre, nombreOp, sizeof(nuevoOp.nombre) - 1);
        nuevoOp.partidasJugadas = 1;
        nuevoOp.maxPuntaje = puntajeObtenido;

        insertarEnArbol(&arbolOps, &nuevoOp, sizeof(tOperador), cmpOperadorNombre);
    }

    // 3. Volcar todo el ABB actualizado de nuevo al archivo binario operadores.dat
    FILE *archEscritura = fopen("operadores.dat", "wb");
    if (archEscritura != NULL)
    {
        // Guardar nodos del árbol en el archivo binario
        // (Podés recorrer en inorden usando fwrite)
        fclose(archEscritura);
    }
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
    FILE *archivo = fopen("config.txt", "rt");
    if (archivo == NULL)
        return ARCHIVO_NO_ENCONTRADO;

    char linea[128];
    char clave[60];
    int valor;

    while (fgets(linea, sizeof(linea), archivo) != NULL)
    {
        // Ignorar líneas vacías o comentarios
        if (linea[0] == '\n' || linea[0] == '\r' || linea[0] == '#')
            continue;

        if (sscanf(linea, "%s %d", clave, &valor) == 2)
        {
            if (strcmp(clave, "cap_max_pila") == 0)
                configuracion->cap_max_pila = valor;
            else if (strcmp(clave, "tiempo_descarga_contenedor") == 0)
                configuracion->tiempo_descarga_contenedor = valor;
            else if (strcmp(clave, "tiempo_reubicacion_contenedor") == 0)
                configuracion->tiempo_reubicacion_contenedor = valor;
            else if (strcmp(clave, "tiempo_carga_camion") == 0)
                configuracion->tiempo_carga_camion = valor;
            else if (strcmp(clave, "duracion_jornada_minutos") == 0)
                configuracion->duracion_jornada_minutos = valor;
            else if (strcmp(clave, "cantidad_muelles") == 0)
                configuracion->cantidad_muelles = valor;
            else if (strcmp(clave, "cantidad_zonas") == 0)
                configuracion->cantidad_zonas = valor;
            else if (strcmp(clave, "maximo_buques") == 0)
                configuracion->maximo_buques = valor;
            else if (strcmp(clave, "maximo_camion") == 0)
                configuracion->maximo_camion = valor;
            else if (strcmp(clave, "maximo_contenedores_por_buque") == 0)
                configuracion->maximo_contenedores_por_buque = valor;
        }
    }

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

void inicializarPuerto(tPuerto *puerto, tConfiguracion *configuracion)
{
    puerto->tiempoAnterior = -1;
    puerto->tiempoActual = 0;
    puerto->puntuacionProvisoria = 0;
    crearLista(&puerto->listaMuelles);
    inicializarMuelles(&puerto->listaMuelles, configuracion->cantidad_muelles);
    crearLista(&puerto->listaZonas);
    crearCola(&puerto->colaBuques);
    crearCola(&puerto->colaCamiones);
    crearZonas(&puerto->listaZonas, configuracion->cantidad_zonas, configuracion->cap_max_pila);
    // (Acá debería inicializar los muelles según configuracion)
}

void inicializarMuelles(tLista *listaMuelles, int cantidad)
{
    int i;
    tMuelle muelleActual;

    for (i = 1; i <= cantidad; i++)
    {
        // Limpia toda la memoria de muelleActual a ceros
        memset(&muelleActual, 0, sizeof(tMuelle));

        snprintf(muelleActual.codigo, sizeof(muelleActual.codigo), "M%d", i);
        muelleActual.disponible = 1; // 1 = Libre / Disponible

        insertarAlFinal(listaMuelles, &muelleActual, sizeof(tMuelle));
    }
}

void crearZonas(tLista *listaZonas, int cantidadZonas, int capacidadZonas)
{
    tZona nuevaZona;
    tZona zona;
    int i;
    char buffer[4];

    for (i = 1; i <= cantidadZonas; i++)
    {
        memset(&nuevaZona, 0, sizeof(tZona));

        crearPila(&nuevaZona.pila);
        nuevaZona.capacidad = capacidadZonas;
        snprintf(nuevaZona.codigo, sizeof(nuevaZona.codigo), "Z%d", i);

        // Copia los bytes de nuevaZona dentro de la memoria del nodo de la lista
        insertarAlFinal(listaZonas, &nuevaZona, sizeof(tZona));

        for (i=1; i<=cantidadZonas; i++)
        {
            crearPila(&zona.pila);
            zona.capacidad=capacidadZonas; //creamos la pila para cada zona y le asignamos capacidad y codigo
            itoa(i, buffer, 10);
            strcpy(zona.codigo,"Z");
            strcat(zona.codigo,buffer);

            insertarAlFinal(listaZonas,&zona,sizeof(zona)); //insertamos cada zona al final de la lista, ya que van a estar ordenadas por numero
        }
    }
}

//Juego


int verificarZonas(const char *codzona, tLista *listaZonas)
{
    // Si localizarZona encuentra la zona (devuelve un puntero != NULL), la zona existe (devuelve 1)
    // Si devuelve NULL, la zona no existe (devuelve 0)
    if (localizarZona(listaZonas, codzona) != NULL)
    {
        return 1; // Zona existente / válida
    }

    return 0; // Zona inexistente
}

tNodo* localizarZona(tLista *listaZonas, const char *codZona)
{
    if (listaZonas == NULL || *listaZonas == NULL || codZona == NULL)
        return NULL;

    tNodo *nodoActual = *listaZonas;

    while (nodoActual != NULL)
    {
        tZona *zonaAux = (tZona *)nodoActual->dato;

        if (strcmp(codZona, zonaAux->codigo) == 0)
        {
            return nodoActual; // Retorna el nodo de la zona encontrada
        }

        nodoActual = nodoActual->sig; // Avanza al siguiente nodo
    }

    return NULL; // Si no la encuentra
}


int procesarArribosDelMinuto(FILE *archEscenario, tPuerto *puerto)
{
    if (archEscenario == NULL || puerto == NULL)
        return ARCHIVO_NO_ENCONTRADO;

    char linea[200];
    int tiempoArribo;

    // Reposicionamos al inicio del archivo para escanear los arribos del minuto T actual
    rewind(archEscenario);

    while (fgets(linea, sizeof(linea), archEscenario) != NULL)
    {
        if (linea[0] == '[' || linea[0] == '\n' || linea[0] == '\r')
            continue;

        // 1. CAMIONES (Líneas que empiezan con 'K')
        if (linea[0] == 'K')
        {
            tCamion nuevoCamion;
            memset(&nuevoCamion, 0, sizeof(tCamion));

            if (sscanf(linea, "%[^;];T=%d;C=%s", nuevoCamion.codigo, &tiempoArribo, nuevoCamion.contenedorBuscado.codigo) == 3)
            {
                if (tiempoArribo == puerto->tiempoActual)
                {
                    ponerEnCola(&puerto->colaCamiones, &nuevoCamion, sizeof(tCamion));
                    printf(" -> [ARRIBO T=%d] Camion %s arribo buscando %s.\n",
                           puerto->tiempoActual, nuevoCamion.codigo, nuevoCamion.contenedorBuscado.codigo);
                }
            }
        }
        // 2. BUQUES (Líneas que empiezan con 'B')
        else if (linea[0] == 'B')
        {
            tBuque nuevoBuque;
            char listaContenedores[100];
            memset(&nuevoBuque, 0, sizeof(tBuque));

            if (sscanf(linea, "%[^;];T=%d;C=%s", nuevoBuque.codigo, &tiempoArribo, listaContenedores) == 3)
            {
                if (tiempoArribo == puerto->tiempoActual)
                {
                    crearCola(&nuevoBuque.sigContenedor);

                    char *token = strtok(listaContenedores, ",\r\n");
                    while (token != NULL)
                    {
                        tContenedor cont;
                        memset(&cont, 0, sizeof(tContenedor));
                        snprintf(cont.codigo, sizeof(cont.codigo), "%s", token);

                        ponerEnCola(&nuevoBuque.sigContenedor, &cont, sizeof(tContenedor));
                        token = strtok(NULL, ",\r\n");
                    }

                    // Buscamos un muelle disponible para atracarlo
                    int idxMuelle = -1;
                    for (int i = 0; i < puerto->cantMuelles; i++)
                    {
                        if (puerto->muelles[i].disponible == 1)
                        {
                            idxMuelle = i;
                            break;
                        }
                    }

                    if (idxMuelle != -1)
                    {
                        puerto->muelles[idxMuelle].disponible = 0; // Ocupado
                        puerto->muelles[idxMuelle].buqueAtracado = nuevoBuque;

                        printf(" -> [ATRACO T=%d] Buque %s atraco directamente en Muelle %s.\n",
                               puerto->tiempoActual, nuevoBuque.codigo, puerto->muelles[idxMuelle].codigo);
                    }
                    else
                    {
                        ponerEnCola(&puerto->colaBuques, &nuevoBuque, sizeof(tBuque));
                        printf(" -> [ARRIBO T=%d] Buque %s encolado (Muelles ocupados).\n",
                               puerto->tiempoActual, nuevoBuque.codigo);
                    }
                }
            }
        }
    }

    return TODO_OK;
}

//Funciones auxiliares
void stringMayuscula(char *str)
{
    while(*str != '\0')
    {
        *str = toupper(*str);
        str++;
    }
}

int cmpCodZona(const void* c1, const void* c2)
{
    const char *cod1, *cod2;
    cod1= (const char *) c1;
    cod2= (const char *) c2;
    return strcmp(cod1,cod2);
}

// Función de comparación para ordenar los operadores por nombre en el ABB
int cmpOperadorNombre(const void *e1, const void *e2)
{
    const tOperador *op1 = (const tOperador *)e1;
    const tOperador *op2 = (const tOperador *)e2;
    return strcmp(op1->nombre, op2->nombre);
}

// Función de acción para imprimir cada operador en el recorrido In-Order
void mostrarOperador(const void *dato)
{
    const tOperador *op = (const tOperador *)dato;
    printf("  %-15s | %-16d | %d pts\n", op->nombre, op->partidasJugadas, op->maxPuntaje);
}
