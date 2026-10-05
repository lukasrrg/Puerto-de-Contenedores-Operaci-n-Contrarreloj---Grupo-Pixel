#include "simulador.h"

int main()
{
    int estadoActualDeJuego = PANTALLA_INICIAL;
    tConfiguracion configuracion;
    char nombreOp[TAM_MAX_NOMBRE_OPERADOR];

    tPuerto miPuerto;

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
            inicializarPuerto(&miPuerto, &configuracion);

            printf("\n=== INICIO DE LA JORNADA (T = 0) ===\n");

            // 4. Bucle de la jornada operativa
            jornadaOperativa(&miPuerto, &configuracion, nombreOp);
            //Mostrar puntuacion final antes de ir a la pantalla inicial A IMPLEMENTAR

            // 5. Al terminar la simulación, volvemos a la pantalla inicial
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

