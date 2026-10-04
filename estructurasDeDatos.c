#include "estructurasDeDatos.h"

//Funciones pila
void crearPila(tPila *pl)
{
    pl->tope = TAM_PILA;
}

void vaciarPila(tPila *pl)
{
    pl->tope = TAM_PILA;
}

int pilaVacia(const tPila *pl)
{
    return (pl->tope == TAM_PILA);
}

int pilaLlena(const tPila *pl, unsigned cantBytes)
{
    return (pl->tope < cantBytes + sizeof(cantBytes));
}

int ponerEnPila(tPila *pl, const void *dato, unsigned cantBytes)
{
    if (pl->tope < cantBytes + sizeof(cantBytes))        //Verifico que todavia quede espacio en la pila
        return ERROR_PILA_LLENA;

    pl->tope -= cantBytes;                         //Guardo el tamaño del dato
    memcpy(pl->pila + pl->tope, dato, cantBytes);
    pl->tope -= sizeof(unsigned);            //Guardo el dato
    memcpy(pl->pila + pl->tope, &cantBytes, sizeof(unsigned));

    return TODO_OK;
}

int sacarDePila(tPila *pl, void *buffer, unsigned cantBytes)
{
    if (pl->tope == TAM_PILA)        //Verifico que la pila no este vacia
        return ERROR_PILA_VACIA;

    unsigned tamDato;

    memcpy(&tamDato, pl->pila + pl->tope, sizeof(unsigned));      //Recupero el tamaño del dato
    pl->tope += sizeof(unsigned);

    memcpy(buffer, pl->pila + pl->tope, minimo(tamDato, cantBytes));             //Recupero el dato
    pl->tope += tamDato;

    return TODO_OK;
}

int verTope(const tPila *pl, void *buffer, unsigned cantBytes)
{
    if (pl->tope == TAM_PILA)        //Verifico que la pila no este vacia
        return ERROR_PILA_VACIA;

    unsigned tamDato;

    memcpy(&tamDato, pl->pila + pl->tope, sizeof(unsigned));      //Recupero el tamaño del dato

    memcpy(buffer, pl->pila + pl->tope + sizeof(unsigned), minimo(tamDato, cantBytes));             //Recupero el dato

    return TODO_OK;
}


//Funciones cola
void crearCola(tCola *pc)
{
    pc->pri = NULL;
    pc->ult = NULL;     //no es necesario
}

int colaLlena(const tCola *pc, unsigned cantBytes) //NO tiene mucho sentido, hagamos que devuelva que nunca esta llena
{
    return TODO_OK;
}

int ponerEnCola(tCola *pc, const void *dato, unsigned cantBytes)
{
//  Verifico que haya memoria --> reservo memoria para nodo + dato
    tNodo *nodoActual;
    nodoActual = malloc(sizeof(tNodo));
    if (nodoActual == NULL)
        return ERROR_SIN_MEM;

    nodoActual->dato = malloc(cantBytes);
    if (nodoActual->dato == NULL)
    {
        free(nodoActual);
        return ERROR_SIN_MEM;
    }


//  Pude reservar memoria. Copio dato (parametro) en nodo.dato
    memcpy(nodoActual->dato, dato,cantBytes);
//  Copio cantBytes en nodo.dato
    nodoActual->tam = cantBytes;
//  El nodo actual que se agrega siempre va a ser el ultimo de la cola, osea, no hay siguiente
    nodoActual->sig = NULL;
//  Si es el primer elemento (pri == NULL) ---> pri <- dir del nodo actual
    if (pc->pri == NULL)
        pc->pri = nodoActual;
    else    //Si no es el primero tengo que ir al ultimo y cambiarle el sig (ult.sig = direccion nodo actual)
        pc->ult->sig = nodoActual;
//  Actualizo ult
    pc->ult = nodoActual;

    return TODO_OK;
}

int verPrimero(const tCola *pc, void *buffer, unsigned cantBytes)
{
//  Verifico si la cola esta vacia
    if (pc->pri == NULL)
        return ERROR_COLA_VACIA;

    tNodo *nodoActual = pc->pri;
//  Copio el dato --> min(cantBytes, pri.tam)
    memcpy(buffer, nodoActual->dato, minimo(cantBytes, nodoActual->tam));

    return TODO_OK;
}

int colaVacia(const tCola *pc)
{
    return (pc->pri == NULL);
}

int sacarDeCola(tCola *pc, void *buffer, unsigned cantBytes)
{
//  Verifico si la cola esta vacia
    if (pc->pri == NULL)
        return ERROR_COLA_VACIA;

    tNodo *nodoActual = pc->pri;
//  Copio el dato --> min(cantBytes, pri.tam)
    memcpy(buffer, nodoActual->dato, minimo(cantBytes, nodoActual->tam));
//  Actualizo pri (pri = pri.sig o elim.sig)
    pc->pri = nodoActual->sig;
//  Libero al dato
    free(nodoActual->dato);
//  Libero al nodo
    free(nodoActual);

    return TODO_OK;
}

void vaciarCola(tCola *pc)
{
    tNodo *nodoActual = pc->pri;

    while (nodoActual != NULL)  //Mientras nodoActual siga señalando a algo...
    {
        pc->pri = nodoActual->sig;  //Guardo el siguiente elemento en pri
        free(nodoActual->dato);     //Libero el dato
        free(nodoActual);           //Libero el nodo
        nodoActual = pc->pri;       //Ahora nodoActual apunta al nuevo nodo
    }
}


//Funciones lista
void crearLista(tLista *pl)
{
    *pl = NULL;
}

void vaciarLista(tLista *pl)
{
    tNodo* elim;

    while(*pl != NULL)
    {
        elim = *pl; // Guardamos la referencia al nodo actual
        free(elim->dato); // Liberamos el dato reservado dinámicamente
        free(elim); // Liberamos el nodo
        pl = &(*pl)->sig; // Avanzamos el puntero de la lista al siguiente
    }
}

int listaLlena(tLista *pl)
{
    return TODO_OK;         //Siempre retorno que hay lugar
}

int listaVacia(tLista *pl)
{
    return (*pl == NULL);
}

int insertarAlFinal(tLista *pl, void *dato, unsigned tam)
{
//  Avanzo hasta que no haya nada, cuando avanzo, avanzo hacia LA DIRECCION DEL SIG DEL NODO ACTUAL (pl = &(*pl)->sig )
    while(*pl != NULL)
        pl = &(*pl)->sig;


//  Si no hay nada---> Reservo memoria para el nodo
    tNodo *nuevoNodo = (tNodo *)malloc(sizeof(tNodo));
    if (nuevoNodo == NULL)      //Si no hay memoria retorno ERROR_SIN_MEM
        return ERROR_SIN_MEM;

    nuevoNodo->dato = malloc(tam);
    if (nuevoNodo->dato == NULL)
    {
        free(nuevoNodo);
        return ERROR_SIN_MEM;
    }

    *pl = nuevoNodo;
//  Copio el dato en nodo.dato
    memcpy(nuevoNodo->dato, dato, tam);
//  Copio el tam en nodo.tam
    nuevoNodo->tam = tam;
//  Seteo nodo.sig en NULL
    nuevoNodo->sig = NULL;


    return TODO_OK;
}

int insertarSinDuplicados(tLista *pl, void *dato, unsigned tam, int (*cmp)(const void *elem1, const void *elem2), void (*accion)(void *elemEnLista, void *datoAccion))
{
    //  Avanzo hasta que no haya nada, cuando avanzo, avanzo hacia LA DIRECCION DEL SIG DEL NODO ACTUAL (pl = &(*pl)->sig )
    while(*pl != NULL)
    {
        if (cmp(dato, (*pl)->dato) == 0)
        {
            if (accion != NULL)
                accion((*pl)->dato, dato);
            return ERROR_ELEM_DUP;
        }
        pl = &(*pl)->sig;
    }

//  Si no hay nada---> Reservo memoria para el nodo
    tNodo *nuevoNodo = (tNodo *)malloc(sizeof(tNodo));
    if (nuevoNodo == NULL)      //Si no hay memoria retorno ERROR_SIN_MEM
        return ERROR_SIN_MEM;

    nuevoNodo->dato = malloc(tam);
    if (nuevoNodo->dato == NULL)
    {
        free(nuevoNodo);
        return ERROR_SIN_MEM;
    }

    *pl = nuevoNodo;
//  Copio el dato en nodo.dato
    memcpy(nuevoNodo->dato, dato, tam);
//  Copio el tam en nodo.tam
    nuevoNodo->tam = tam;
//  Seteo nodo.sig en NULL
    nuevoNodo->sig = NULL;


    return TODO_OK;
}

int insertarEnOrden(tLista *pl, void *dato, unsigned tam, int (*cmp)(const void *elem1, const void *elem2), int aceptaDuplicados, void (*accion)(void *elemEnLista, void *datoAccion))
{
    //Me muevo hasta la ultima posicion
    while(*pl != NULL && cmp(dato, (*pl)->dato) >= 0)
    {
        if (aceptaDuplicados == NO_ACEPTA_DUP && cmp(dato, (*pl)->dato) == 0)
        {
            if (accion != NULL)
                accion((*pl)->dato, dato);
            return ERROR_ELEM_DUP;
        }

        pl = &(*pl)->sig;
    }

//  Si no hay nada---> Reservo memoria para el nodo
    tNodo *nuevoNodo = (tNodo *)malloc(sizeof(tNodo));
    if (nuevoNodo == NULL)      //Si no hay memoria retorno ERROR_SIN_MEM
        return ERROR_SIN_MEM;

    nuevoNodo->dato = malloc(tam);
    if (nuevoNodo->dato == NULL)
    {
        free(nuevoNodo);
        return ERROR_SIN_MEM;
    }


//  Copio el dato en nodo.dato
    memcpy(nuevoNodo->dato, dato, tam);
//  Copio el tam en nodo.tam
    nuevoNodo->tam = tam;
//  Seteo nodo.sig en NULL
    nuevoNodo->sig = *pl;


    *pl = nuevoNodo;

    return TODO_OK;
}

int mostrarLista(const tLista *pl, void (*muestra)(const void *dato))
{
    const tNodo *nodoActual = *pl;

    if (nodoActual == NULL)
        return ERROR_LISTA_VACIA;
    else
        do
        {
            muestra(nodoActual->dato);

            nodoActual = nodoActual->sig;
        } while(nodoActual != NULL);

    return TODO_OK;
}

//int buscarEnLista(const void *dato, const void *lista, funcion de comparacion)
//{
//
//}
