#include "simulador.h"

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
    } while(*estadoDeJuego < SALIR || *estadoDeJuego > ESTADISTICAS);
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
    char comando[10];
    //tener alguna tLista llamada listaComandos, en donde se pueda verificar si un comando esta disponible o no
    int jornadaActiva = JORNADA_ACTIVA;
    int hayEventoFuturo = HAY_EVENTO_FUTURO;

    while (jornadaActiva && puerto->tiempoActual < configuracion->duracion_jornada_minutos)
    {
        //Actualizar buques y camiones para el T actual
        hayEventoFuturo = actualizarBuquesCamiones(puerto);

        printf("Comandos disponibles: ");
        //mostrarComandosDisponibles A IMPLEMENTAR
        printf("\n<%s> ", nombreOp);
        scanf("%s", comando);               // Leemos el comando escrito
        stringMayuscula(comando);           //Normalizamos todo a mayuscula, asi tambien se puede escribir en minuscula el comando

        system("cls");                      // Limpia toda la pantalla ANTES de mostrar el menú

        //Aca habria que comparar "comando" no con todos los comandos del juego, sino solo con los comandos disponibles
        //o sea --> buscarEnLista(comando, listaComandos, funcion de comparacion)   SI "comando" NO EXISTE EN "listaComandos" volver a pedir el ingreso del comando
        //tambien tener en cuenta que los comandos des y reu reciben parametros
        //tambien verificar si el tiempo de esa accion llega a entrar dentro del tiempo de la jornada
        if (strcmp(comando, "DES") == 0)
        {
            puerto->tiempoActual += configuracion->tiempo_descarga_contenedor;
            //Aumentar puntuacion
        }
        else if (strcmp(comando, "REU") == 0)
        {
            puerto->tiempoActual += configuracion->tiempo_reubicacion_contenedor;
        }
        else if (strcmp(comando, "ENT") == 0)
        {
            puerto->tiempoActual += configuracion->tiempo_carga_camion;
            //Aumentar puntuacion
        }
        else if (strcmp(comando, "VER") == 0)
        {
            mostrarEstadoPuerto(puerto); // Llama a la función
        }
        else if (strcmp(comando, "ESP") == 0)
        {
            puerto->tiempoActual++; // ESP avanza el reloj 1 minuto
        }
        else if (strcmp(comando, "SALIR") == 0) // Un comando extra para salir del bucle
        {
            jornadaActiva = FIN_DE_JORNADA;
        }
        else
        {
            printf("[ERROR] Comando no reconocido. Intente VER o ESP.\n");
        }

        //Registrar la operacion realizada


        //Actualizacion (capaz se puede hacer al principio del while)
        //if (comandosDisponibles(puerto, listaComandos))
//        {Hacer una lista de comandos disponibles?}
//        else if (verificar si quedan eventos futuros o buques con contenedores o camiones esperando)
//        {
//            avance automatico del tiempo
//        }
//        else
//        {
//            jornadaActiva = FIN_DE_JORNADA
//        }



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
}

void inicializarMuelles(tLista *listaMuelles, int cantidad)
{
    int i;
    tMuelle muelleActual;
    char codigo[TAM_COD_MUELLE];

    for (i = 1; i <= cantidad; i++)
    {
        strcpy(muelleActual.codigo, "M");
        itoa(i, codigo, 10);
        strcat(muelleActual.codigo, codigo);

        muelleActual.buqueActual = NULL;

        insertarAlFinal(listaMuelles, &muelleActual, sizeof(tMuelle));
    }
}

//Juego
void ejecutarJornada(tPuerto *puerto, const tConfiguracion *config)
{
    char lineaComando[100];
    char comando[10];
    int continuarJornada = 1;

    // Primer chequeo e impresión inicial o inicio del reloj T=0
    printf("\n=== INICIO DE LA JORNADA OPERATIVA (T = %d) ===\n", puerto->tiempoActual);

    while (continuarJornada && puerto->tiempoActual < config->duracion_jornada_minutos)
    {
        printf("\nOPERADOR> ");

        if (fgets(lineaComando, sizeof(lineaComando), stdin) != NULL)
        {
            // Extraemos únicamente el primer código de operación (ej: DES, REU, ENT, VER, ESP)
            sscanf(lineaComando, "%s", comando);

            // =========================================================
            // AQUÍ UBICÁS EL CHEQUEO DEL COMANDO "VER"
            // =========================================================
            if (strcmp(comando, "VER") == 0)
            {
                mostrarEstadoPuerto(puerto); // Muestra muelles, zonas, ventana y NO consume tiempo T
            }
            else if (strcmp(comando, "DES") == 0)
            {
                // TODO: Lógica para Descargar y Almacenar (DES  )
            }
            else if (strcmp(comando, "REU") == 0)
            {
                // TODO: Lógica para Reubicar contenedor (REU  )
            }
            else if (strcmp(comando, "ENT") == 0)
            {
                // TODO: Lógica para Entregar contenedor al camión (ENT)
            }
            else if (strcmp(comando, "ESP") == 0)
            {
                // TODO: Avanzar el reloj 1 minuto voluntariamente (ESP)
            }
            else
            {
                printf("[ERROR] Comando no reconocido. Intente VER, DES, REU, ENT o ESP.\n");
            }
        }
    }
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
