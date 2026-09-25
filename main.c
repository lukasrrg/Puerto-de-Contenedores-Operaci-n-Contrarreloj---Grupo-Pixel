#include "simulador.h"

int main()
{
    int estadoActualDeJuego = PANTALLA_INICIAL;
    tConfiguracion configuracion;

    //Este switch deberia estar dentro de un while, no lo agrego porque al no estar implementado JUGAR ni ESTADISTICAS, se colgaria
    switch(estadoActualDeJuego)
    {
    case PANTALLA_INICIAL:
        pantallaInicial(&estadoActualDeJuego);
        break;
    case JUGAR:
        //ingresarNombreOperador();
        leerArchivoConfiguracion(&configuracion);
        break;
    case ESTADISTICAS:
        break;
    }

    return TODO_OK;
}

