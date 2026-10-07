#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANIO 100

// Nodo para la lista enlazada (manejo de colisiones)
typedef struct HashNode {
    char *clave;
    int valor;
    struct HashNode *siguiente;
} HashNode;

// Estructura de la tabla hash
typedef struct {
    HashNode *tabla[TAMANIO];
} HashTable;

unsigned int funcion_hash(const char *clave) {
    unsigned int hash = 0;
    while (*clave) {
        hash = (hash * 31) + *clave;
        clave++;
    }
    return hash % TAMANIO;
}

// Agregando otra funcion hash
unsigned long funcionHashDJB2(const char* clave){
    unsigned long hash = 5381;
    int c;

    while ((c = *clave++))
    {
        hash = ((hash<<5)+ hash) + c;
    }
    return hash % TAMANIO;
}

// Insertar un elemento
void hash_insertar(HashTable *ht, const char *clave, int valor) {
    unsigned int indice = funcion_hash(clave);
    //unsigned long indice = funcionHashDJB2(clave);
    HashNode *actual = ht->tabla[indice];

    // Verificar si la clave ya existe para actualizarla
    while (actual != NULL) {
        if (strcmp(actual->clave, clave) == 0 && actual->valor == valor) {
            printf("[ERROR] El registro '%s' con ID %d ya existe en la sublista.\n", clave, valor);
            return;
        }
        actual = actual->siguiente;
    }

    // Crear nuevo nodo si no existe
    HashNode *nuevo = (HashNode *)malloc(sizeof(HashNode));
    nuevo->clave = strdup(clave);
    nuevo->valor = valor;
    nuevo->siguiente = ht->tabla[indice];
    ht->tabla[indice] = nuevo;

    //printf("Insertado con éxito: %s con ID: %d en el indice: %lu\n", clave, valor, indice);
    printf("Insertado con éxito: %s con ID: %d en el indice: %u\n", clave, valor, indice);
}

// Buscar un elemento con su indice
HashNode* hash_buscar(HashTable *ht, const char *clave, int valor_buscado, int *encontrado) {
    unsigned int indice = funcion_hash(clave);
    //unsigned long indice = funcionHashDJB2(clave);
    
    HashNode *actual = ht->tabla[indice];

    while (actual != NULL) {
        if (strcmp(actual->clave, clave) == 0 && actual->valor == valor_buscado) {
            *encontrado = 1;
            return actual;
        }
        actual = actual->siguiente;
    }

    *encontrado = 0;
    return NULL;
}

// Buscar solo por nombre

HashNode* hash_buscar_nombre(HashTable *ht, const char *clave, int *encontrado) {
    unsigned int indice = funcion_hash(clave);
    HashNode *actual = ht->tabla[indice];

    while (actual != NULL) {
        // Buscamos SOLO por nombre (clave)
        if (strcmp(actual->clave, clave) == 0) {
            *encontrado = 1;
            return actual; // Devuelve el primer nodo que coincida
        }
        actual = actual->siguiente;
    }

    *encontrado = 0;
    return NULL;
}

void hash_buscar_todos(HashTable *ht, const char *clave) {
    unsigned int indice = funcion_hash(clave);
    HashNode *actual = ht->tabla[indice];
    int conteo = 0;

    printf("Buscando todos los registros para el nombre '%s' con el indice %u:\n", clave, indice);
    
    while (actual != NULL) {
        if (strcmp(actual->clave, clave) == 0) {
            printf("  -> Se encontró: %s con ID: %d\n", actual->clave, actual->valor);
            conteo++;
        }
        actual = actual->siguiente;
    }

    if (conteo == 0) {
        printf("  No se encontraron registros con el nombre '%s'.\n", clave);
    }
}

// Borrar una clave específica de la tabla
int hash_borrar(HashTable *ht, const char *clave) {
    unsigned int indice = funcion_hash(clave);
    //unsigned long indice = funcionHashDJB2(clave);
    
    HashNode *actual = ht->tabla[indice];
    HashNode *anterior = NULL;

    while (actual != NULL) {
        if (strcmp(actual->clave, clave) == 0) {
            // Si el nodo a borrar es el primero de la lista
            if (anterior == NULL) {
                ht->tabla[indice] = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }

            // Liberar memoria del nodo
            free(actual->clave); // Liberar la copia de la cadena creada con strdup
            free(actual);
            return 1; // Borrado con éxito
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    return 0; // Clave no encontrada
}

// Liberar toda la memoria de la tabla hash
void hash_destruir(HashTable *ht) {
    for (int i = 0; i < TAMANIO; i++) {
        HashNode *actual = ht->tabla[i];
        while (actual != NULL) {
            HashNode *temporal = actual;
            actual = actual->siguiente;
            
            free(temporal->clave); // Liberar la clave
            free(temporal);       // Liberar el nodo
        }
        ht->tabla[i] = NULL;
    }
}

// limpiar tabla

void hash_inicializar(HashTable *ht){
    for (int i = 0; i < TAMANIO; i++)
    {
        ht->tabla[i] = NULL;
    }
    
}

int main(){

HashTable HT;
hash_inicializar(&HT);

printf("--------------------------------------------------------------------\n");

//hash_insertar(HashTable *ht, const char *clave, int valor)
hash_insertar(&HT,"Ernesto",10);
hash_insertar(&HT,"Braulio",15);
hash_insertar(&HT,"Catalina",56);
hash_insertar(&HT,"Alondra",18);
hash_insertar(&HT,"Freddy",59);
hash_insertar(&HT,"Ernesto",18);
hash_insertar(&HT,"Fer",18);

//hash_buscar(HashTable *ht, const char *clave, int *encontrado)

int encontrado;
int id_buscado = 180;
HashNode* resultado =  hash_buscar(&HT, "Ernesto", id_buscado, &encontrado);

printf("--------------------------------------------------------------------\n");
if (encontrado && resultado != NULL)
{
    printf("Encontrado el Nombre : %s Con ID: %d\n", resultado->clave, resultado->valor);
}else{
    printf("No se encontro el registro con ID %d\n", id_buscado);
}


printf("--------------------------------------------------------------------\n");
// Buscamos todas las instancias de Ernesto (debería mostrar el ID 18 y el ID 10)
hash_buscar_todos(&HT, "Ernesto");

printf("--------------------------------------------------------------------\n");
// Buscamos todas las instancias de Fernanda 
hash_buscar_todos(&HT, "Fernanda");

printf("--------------------------------------------------------------------\n");
// Buscamos todas las instancias de catalina
hash_buscar_todos(&HT, "Catalina");

printf("--------------------------------------------------------------------\n");
// Liberar memoria antes de salir (¡Súper importante!)
hash_destruir(&HT);

//printf("\n %d %d \n",hash_buscar(&HT,"Ernesto",&encontrado),hash_buscar(&HT,"Ernesto",&encontrado));

return 0;

}