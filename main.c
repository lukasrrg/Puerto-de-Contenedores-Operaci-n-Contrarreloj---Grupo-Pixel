#include "simulador.h"

int main()
{
    int estadoActualDeJuego = PANTALLA_INICIAL;
    tConfiguracion configuracion;
    char nombreOp[TAM_MAX_NOMBRE_OPERADOR];

    char comando[10];
    int jornadaActiva;

    while (estadoActualDeJuego != SALIR)
    {
        system("cls"); // Limpia toda la pantalla ANTES de mostrar el menú

        switch(estadoActualDeJuego)
        {
        case PANTALLA_INICIAL:
            pantallaInicial(&estadoActualDeJuego);
            break;

        case JUGAR:

            system("cls"); // Limpia toda la pantalla ANTES

            // 1. Pedimos el nombre del operador
            ingresarNombreOperador(nombreOp);
            printf("\n Operador cargado con exito: %s\n\n", nombreOp);

            // 2. Leemos la configuración del archivo config.txt
            if (leerArchivoConfiguracion(&configuracion) == TODO_OK)
            {
                printf("--- CONFIGURACION CARGADA ---\n");
                configuracionMostrar(&configuracion);
            }
            // 3. Inicializamos el estado del puerto
            tPuerto miPuerto;
            miPuerto.tiempoActual = 0;
            miPuerto.puntuacionProvisoria = 0;
            miPuerto.cantMuelles = configuracion.cantidad_muelles;
            crearLista(&miPuerto.listaZonas);
            crearCola(&miPuerto.colaBuques);
            crearCola(&miPuerto.colaCamiones);
            // (Acá debería inicializar los muelles según configuracion)


            printf("\n=== INICIO DE LA JORNADA (T = 0) ===\n");


            jornadaActiva = 1;

            // 4. Bucle de la jornada operativa
            while (jornadaActiva && miPuerto.tiempoActual < configuracion.duracion_jornada_minutos)
            {
                printf("OPERADOR ");
                scanf("%s", comando); // Leemos el comando escrito
                system("cls"); // Limpia toda la pantalla ANTES de mostrar el menú

                if (strcmp(comando, "VER") == 0)
                {
                    mostrarEstadoPuerto(&miPuerto); // Llama a la función
                }
                else if (strcmp(comando, "ESP") == 0)
                {
                    miPuerto.tiempoActual++; // ESP avanza el reloj 1 minuto
                }
                else if (strcmp(comando, "SALIR") == 0) // Un comando extra para salir del bucle
                {
                    jornadaActiva = 0;
                }
                else
                {
                    printf("[ERROR] Comando no reconocido. Intente VER o ESP.\n");
                }
            }

            // 5. Al terminar la simulación, volvemos a la pantalla inicial
            estadoActualDeJuego = PANTALLA_INICIAL;
            break;

        case ESTADISTICAS:
            printf("\n--- ESTADISTICAS Y RANKING ---\n");
            // Volvemos a la pantalla inicial tras mostrar estadísticas
            estadoActualDeJuego = PANTALLA_INICIAL;
            break;
        }
    }

    return TODO_OK;
}

