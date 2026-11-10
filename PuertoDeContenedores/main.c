#include "simulador.h"

int main()
{
    int estadoActualDeJuego = PANTALLA_INICIAL;
    tConfiguracion configuracion;
    char nombreOp[TAM_MAX_NOMBRE_OPERADOR];
    tPuerto miPuerto;
    tZona nuevaZona;

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
            memset(&miPuerto, 0, sizeof(tPuerto));
            miPuerto.tiempoActual = 0;
            miPuerto.puntuacionProvisoria = 0;

            // Limitar la cantidad de muelles al maximo soportado por el arreglo
            if (configuracion.cantidad_muelles > 10)
                miPuerto.cantMuelles = 10;
            else
                miPuerto.cantMuelles = configuracion.cantidad_muelles;

            // Inicializar cada muelle como libre/disponible
            for (int i = 0; i < miPuerto.cantMuelles; i++)
            {
                snprintf(miPuerto.muelles[i].codigo, sizeof(miPuerto.muelles[i].codigo), "M%d", i + 1);
                miPuerto.muelles[i].disponible = 1; // 1 = Libre / Disponible
            }

            // Inicializar las TDAs principales del puerto
            crearLista(&miPuerto.listaZonas);
            crearCola(&miPuerto.colaBuques);
            crearCola(&miPuerto.colaCamiones);

            // Cargar e insertar las Zonas de Almacenamiento (Z1, Z2, Z3...) en listaZonas
            for (int i = 0; i < configuracion.cantidad_zonas; i++)
            {
                memset(&nuevaZona, 0, sizeof(tZona));
                snprintf(nuevaZona.codigo, sizeof(nuevaZona.codigo), "Z%d", i + 1);
                crearPila(&nuevaZona.pila); // Inicializa la pila de contenedores de la zona

                insertarAlFinal(&miPuerto.listaZonas, &nuevaZona, sizeof(tZona));
            }

            // 4. Iniciar la jornada operativa (funcion elegida por el grupo)
            jornadaOperativa(&miPuerto, &configuracion, nombreOp);

            // 5. Al terminar la partida, actualizar / registrar las estadisticas en el binario
            actualizarEstadisticasOperador(nombreOp, miPuerto.puntuacionProvisoria);

            // 6. Liberar la memoria dinamica de las TDAs utilizadas en la simular
            vaciarLista(&miPuerto.listaZonas);
            vaciarCola(&miPuerto.colaBuques);
            vaciarCola(&miPuerto.colaCamiones);

            // Volver a la pantalla principal
            estadoActualDeJuego = PANTALLA_INICIAL;
            break;

        case ESTADISTICAS:
            mostrarPantallaEstadisticas();
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

