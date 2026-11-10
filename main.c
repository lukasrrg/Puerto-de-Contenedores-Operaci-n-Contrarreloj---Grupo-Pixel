#include "simulador.h"

int main()
{
    int estadoActualDeJuego = PANTALLA_INICIAL;
    tConfiguracion configuracion;
    char nombreOp[TAM_MAX_NOMBRE_OPERADOR];
    tPuerto miPuerto;
    tZona nuevaZona;
    tArbol arbolEstadisticas;

    crearArbol(&arbolEstadisticas);

    while (estadoActualDeJuego != SALIR)
    {
        system("cls"); // Limpia toda la pantalla ANTES de mostrar el menú

        switch(estadoActualDeJuego)
        {
        case PANTALLA_INICIAL:
            pantallaInicial(&estadoActualDeJuego);
            break;

        case JUGAR:
            system("cls");

            // 1. Pedir nombre del operador activo
            fflush(stdin);
            ingresarNombreOperador(nombreOp);
            printf("\n -> Operador activo registrado: %s\n\n", nombreOp);

            // 2. Leer configuracion desde config.txt
            if (leerArchivoConfiguracion(&configuracion) == TODO_OK)
            {
                printf("--- CONFIGURACION CARGADA ---\n");
                configuracionMostrar(&configuracion);
            }

            // 3. Inicializacion limpia a cero de la estructura tPuerto
            tPuerto miPuerto;
            memset(&miPuerto, 0, sizeof(tPuerto));
            miPuerto.tiempoActual = 0;
            miPuerto.puntuacionProvisoria = 0;

            // Controlar que la cantidad de muelles no supere el tamano del arreglo
            if (configuracion.cantidad_muelles > 10)
                miPuerto.cantMuelles = 10;
            else
                miPuerto.cantMuelles = configuracion.cantidad_muelles;

            // Inicializar cada muelle como disponible (1 = Libre)
            for (int i = 0; i < miPuerto.cantMuelles; i++)
            {
                snprintf(miPuerto.muelles[i].codigo, sizeof(miPuerto.muelles[i].codigo), "M%d", i + 1);
                miPuerto.muelles[i].disponible = 1;
            }

            // Inicializar las estructuras dinamicas del puerto
            crearLista(&miPuerto.listaZonas);
            crearCola(&miPuerto.colaBuques);
            crearCola(&miPuerto.colaCamiones);

            // Cargar e insertar las Zonas de Almacenamiento (Z1, Z2, Z3...) en listaZonas
            for (int i = 0; i < configuracion.cantidad_zonas; i++)
            {
                tZona nuevaZona;
                memset(&nuevaZona, 0, sizeof(tZona));
                snprintf(nuevaZona.codigo, sizeof(nuevaZona.codigo), "Z%d", i + 1);
                crearPila(&nuevaZona.pila); // Inicializa la pila interna de la zona

                insertarAlFinal(&miPuerto.listaZonas, &nuevaZona, sizeof(tZona));
            }

            // 4. Iniciar la jornada operativa de la simulación
            jornadaOperativa(&miPuerto, &configuracion, nombreOp, &arbolEstadisticas);

            // 5. Liberar la memoria dinamica utilizada durante la partida
            vaciarLista(&miPuerto.listaZonas);
            vaciarCola(&miPuerto.colaBuques);
            vaciarCola(&miPuerto.colaCamiones);

            // 6. Volver a la pantalla inicial del menu principal
            estadoActualDeJuego = PANTALLA_INICIAL;
            break;

        case ESTADISTICAS:
            system("cls");
            printf("=================== HISTORIAL Y RANKING DE OPERADORES ===================\n");

            FILE *archTxt = fopen("operadores.txt", "rt");
            if (archTxt == NULL)
            {
                printf("  [INFO] No hay estadisticas registradas todavia en 'operadores.txt'.\n");
            }
            else
            {
                char linea[150];
                while (fgets(linea, sizeof(linea), archTxt) != NULL)
                {
                    printf("  %s", linea);
                }
                fclose(archTxt);
            }

            printf("=========================================================================\n");
            printf("\nPresione Enter para volver al menu principal...");
            fflush(stdin);
            getchar();

            estadoActualDeJuego = PANTALLA_INICIAL;
            break;

        case SALIR:
            printf("\n¡Gracias por utilizar el simulador de puerto!\n");
            break;
        }
    }


    //vaciarTodo            A IMPLEMENTAR (todo para lo que se haya reservado memoria dinamica y no se haya liberado, agregarlo aca)
    return TODO_OK;
}

