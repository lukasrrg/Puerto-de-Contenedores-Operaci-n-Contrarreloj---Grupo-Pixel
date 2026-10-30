#include "simulador.h"

int main()
{
    int estadoActualDeJuego = PANTALLA_INICIAL;
    tConfiguracion configuracion;
    char nombreOp[TAM_MAX_NOMBRE_OPERADOR];

    //tPuerto miPuerto;

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

            // 1. Pedir nombre del operador
            fflush(stdin);
            ingresarNombreOperador(nombreOp);

            // 2. Leer configuracion desde config.txt
            if (leerArchivoConfiguracion(&configuracion) == TODO_OK)
            {
                configuracionMostrar(&configuracion);
            }

            // 3. Inicializacion limpia a cero del puerto
            tPuerto miPuerto;
            memset(&miPuerto, 0, sizeof(tPuerto));
            miPuerto.tiempoActual = 0;
            miPuerto.puntuacionProvisoria = 0;

            // Inicializar muelles (limitar a maximo 10)
            if (configuracion.cantidad_muelles > 10)
                miPuerto.cantMuelles = 10;
            else
                miPuerto.cantMuelles = configuracion.cantidad_muelles;

            for (int i = 0; i < miPuerto.cantMuelles; i++)
            {
                snprintf(miPuerto.muelles[i].codigo, sizeof(miPuerto.muelles[i].codigo), "M%d", i + 1);
                miPuerto.muelles[i].disponible = 1; // 1 = Libre / Disponible
            }

            // Inicializar TDAs principales
            crearLista(&miPuerto.listaZonas);
            crearCola(&miPuerto.colaBuques);
            crearCola(&miPuerto.colaCamiones);

            // === CARGAR LAS ZONAS DE ALMACENAMIENTO (Z1, Z2, Z3...) ===
            for (int i = 0; i < configuracion.cantidad_zonas; i++)
            {
                tZona nuevaZona;
                memset(&nuevaZona, 0, sizeof(tZona));
                snprintf(nuevaZona.codigo, sizeof(nuevaZona.codigo), "Z%d", i + 1);
                crearPila(&nuevaZona.pila); // Inicializa la pila de contenedores de la zona

                insertarAlFinal(&miPuerto.listaZonas, &nuevaZona, sizeof(tZona));
            }

            // Abrir archivo de escenario y ejecutar simulación
            FILE *archEscenario = fopen("puerto.txt", "rt");

            ejecutarJornada(&miPuerto, &configuracion, archEscenario);

            if (archEscenario != NULL)
                fclose(archEscenario);

            estadoActualDeJuego = PANTALLA_INICIAL;
            break;

        case ESTADISTICAS:
            printf("\n--- ESTADISTICAS Y RANKING ---\n");
            //mostrarEstadisticas() A IMPLEMENTAR
            system("pause");
            // Volvemos a la pantalla inicial tras mostrar estadísticas
            estadoActualDeJuego = PANTALLA_INICIAL;
            break;
        }
    }


    //vaciarTodo            A IMPLEMENTAR (todo para lo que se haya reservado memoria dinamica y no se haya liberado, agregarlo aca)
    return TODO_OK;
}

