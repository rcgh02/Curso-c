#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int val;
    struct node * next;
} node_t;

void print_list(node_t * head){
    node_t * current = head;
    while (current != NULL){
        printf("%d\n", current->val);
        current = current->next;
    }
}

void push(node_t * head, int val){
    node_t * current = head;
    while (current->next != NULL)
    {   
        current = current->next;
    }
    current->next = (node_t *) malloc(sizeof(node_t));
    current->next->val = val;
    current->next->next = NULL;
}

// Agregando el primer nodo al principio de la lista
void push_f(node_t ** head, int val){
    node_t * new_node;
    new_node = (node_t *) malloc(sizeof(node_t));

    new_node->val = val;
    new_node->next = *head;
    *head = new_node;
}

// Eliminar elementos 
int pop_f(node_t ** head){
    int retval = -1; 
    node_t * next_nodo = NULL; // creamos un nodo con valor null

    if(*head== NULL){
        return -1;
    }

    next_nodo = (*head)->next; //guardadamos en 
    // next_nodo lo que hay en el sig node de head
    retval = (*head)->val; // guardamos el valor que 
    // hay en head
    free(*head); // liberamos memoria de head
    *head = next_nodo; // copiamos a head lo que hay en next_nodo
    return retval;
}

// Eliminar ultimo elemento de la lista
int remove_last(node_t * head){
    int retval = 0;
    if(head->next == NULL){
        retval = head->val;
        free(head);
        return retval;
    }

    node_t * current = head;

    while(current->next->next !=NULL){
        current = current->next;
    }

    retval = current->next->val;
    free(current->next);
    current->next = NULL;
    return retval;
}

// Eliminar elemento respecto al indice

int remove_by_index(node_t ** head, int n){ // este doble
    // puntero es para poder modifcar el valor del puntero
    int i = 0;
    int retval = -1;

    if(head == NULL || *head == NULL){
        return -1;
    }
    node_t * current = *head; // guardamos el puntero de head
    // en current
    node_t * temp_node = NULL; // este es el puntero tenporal

    if (n == 0) { // si n que es el valor del puntero es cero borra
        // el head
        return pop_f(head);
    }

    //  recorremos los valores de los punteros n
    for (i=0; i < n-1 ; i++ ){
        if(current->next == NULL){
            return -1;
        }
        current = current->next;
    }

    if (current->next == NULL){
        return -1;
    }

    temp_node = current->next; // en temp_node se guarda lo que tenemos
    // en el siguente nodo despues del actual
    retval = temp_node->val; // en retval asignamos el valor
    // de temp_nodo
    current->next = temp_node->next;// guadamos lo que hay en el 
    //  nodo temporal sigueinte en el actual proximo
    free(temp_node); // liberamos memoria 

    return retval;
}

int main() {

    node_t * head = NULL;
    node_t * pila = NULL;

    printf("Prueba LIFO (Último en entrar primero en salir.) \n");

    push_f(&pila,10);
    push_f(&pila,20);
    push_f(&pila,30);

    printf("Contenido de la pila, el ultimo que entro fue 30,20 y 10\n");
    print_list(pila);

    printf("Removiendo elementos: \n");
    //printf("Salio:%d\n",remove_by_index(&pila,0));
    // printf("Salio:%d\n",remove_by_index(&pila,0));
    //printf("Salio:%d\n",remove_by_index(&pila,0));   
    
    printf("Salio:%d\n",pop_f(&pila));
    printf("Salio:%d\n",pop_f(&pila));
    //printf("Salio:%d\n",pop_f(&pila));   
    printf("Despues de eliminar \n"); 
    print_list(pila);

    printf("Prueba FIFO (Primero en entrar primero en salir.)\n");
    node_t * cola = (node_t *) malloc(sizeof(node_t));
    if(cola == NULL){
        return 1;
   }
    cola->val = 100;
    cola->next = NULL;

    push(cola,200);
    push(cola,300);

    printf("Contenido cola, primero en entrar fue 100,200 y 300\n");
    print_list(cola);
    
    printf("Removiendo elemento: \n");
    //printf("Saliendo: %d\n",remove_by_index(&cola,0));
    //printf("Saliendo: %d\n",remove_by_index(&cola,0));

    printf("Salio:%d\n",pop_f(&cola));
    printf("Salio:%d\n",pop_f(&cola));
    //printf("Salio:%d\n",pop_f(&pila));   
    printf("Despues de eliminar \n"); 
    print_list(cola);

    return 0;
}
