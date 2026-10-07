#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANIO 10

typedef struct Nodo
{
    char nombre[50];
    int id;
    struct Nodo* siguiente;
} Nodo;

typedef struct 
{
    Nodo* arreglo[TAMANIO];
} TablaHash;

// --- Funciones hash ---//

unsigned int funcion_hash(const char *clave) {
    unsigned int hash = 0;
    while (*clave) {
        hash = (hash * 31) + *clave;
        clave++;
    }
    return hash % TAMANIO;
}

int funcionHash(char* nombre){
    int suma = 0;
    for (int i = 0; nombre[i] != '\0'; i++){
        suma += nombre[i];
    }
    return suma % TAMANIO;
}

unsigned long funcionHashDJB2(char* nombre){
    unsigned long hash = 5381;
    int c;

    while ((c = *nombre++))
    {
        hash = ((hash<<5)+ hash) + c;
    }
    return hash % TAMANIO;
}

void inicializar(TablaHash* tabla){
    for (int i = 0; i < TAMANIO; i++)
    {
        tabla->arreglo[i] = NULL;
    }
}

Nodo* buscar(TablaHash*tabla, char* nombre, int id_buscado){
    //unsigned long indice = funcionHashDJB2(nombre);
    unsigned int indice = funcion_hash(nombre);
    Nodo* actual = tabla->arreglo[indice];

    while (actual != NULL)
    {
        if (strcmp(actual->nombre, nombre) == 0 && actual->id == id_buscado)
        {
            return actual;
        }
        actual = actual->siguiente;
    }
    return NULL;
}

void insertar(TablaHash* tabla, char* nombre, int id){

    if (buscar(tabla, nombre, id) != NULL)
    {
        printf("[ERROR] No se pudo insertar: El registro '%s' con ID %d YA EXISTE en la tabla.\n", nombre, id);
        return; // Terminamos la función aquí, impidiendo el duplicado
    }
    
    //unsigned long indice = funcionHashDJB2(nombre);
    unsigned int indice = funcion_hash(nombre);

    Nodo* nuevoNodo = (Nodo*)malloc(sizeof(Nodo));
    strcpy(nuevoNodo->nombre, nombre);
    nuevoNodo->id = id;

    nuevoNodo->siguiente = tabla->arreglo[indice];
    tabla->arreglo[indice] = nuevoNodo;

    printf("Insertado: %s con ID: %d en el indice: %d\n", nombre, id, indice);
}


// void insertar(TablaHash* tabla, char* nombre, int id){
//     int indice = funcionHash(nombre);

//     Nodo* nuevoNodo = (Nodo*)malloc(sizeof(Nodo));
//     strcpy(nuevoNodo->nombre, nombre);
//     nuevoNodo->id = id;

//     nuevoNodo->siguiente = tabla->arreglo[indice];
//     tabla->arreglo[indice] = nuevoNodo;

//     printf("Insertado: %s con ID: %d en el indice: %d\n", nombre, id, indice);
// }



// Nodo* buscar(TablaHash*tabla, char* nombre, int id_buscado){
//     int indice = funcionHash(nombre);
//     Nodo* actual = tabla->arreglo[indice];

//     while (actual != NULL)
//     {
//         if (strcmp(actual->nombre, nombre) == 0 && actual->id == id_buscado)
//         {
//             return actual;
//         }
//         actual = actual->siguiente;
//     }
//     return NULL;
// }

int main(){
    TablaHash miTabla;
    inicializar(&miTabla);

    printf("----- Insertando datos ----- \n");


    insertar(&miTabla, "yuki", 14);
    insertar(&miTabla, "yuki", 15);
    insertar(&miTabla, "yuki", 16);
    insertar(&miTabla, "yuki", 16);
    insertar(&miTabla, "Fernanda",2);

    insertar(&miTabla, "Hmc", 1);

    printf("\n ---- Buscando ---- \n");
    int idBuscar = 16;
    Nodo* resultado = buscar(&miTabla, "yuki", idBuscar);

    if (resultado != NULL){
        printf("¡Encontrado con éxito! Nombre: %s, ID: %d\n", resultado->nombre, resultado->id);
    } else {
        printf("No se encontró a yuki con el ID %d\n", idBuscar);
    }
    
    return 0;
}