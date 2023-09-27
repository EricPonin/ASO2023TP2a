#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // Incluye la librería booleana
#define NUMBER_OF_THREADS 2
#define CANTIDAD_INICIAL_HAMBURGUESAS 20

int cantidad_restante_hamburguesas = CANTIDAD_INICIAL_HAMBURGUESAS;
int turno = 0;
bool hamburguesas_agotadas = false; // Variable para indicar que no quedan hamburguesas

void *comer_hamburguesa(void *tid) {
    int thread_id = (int)tid;
    while (!hamburguesas_agotadas) { // Verificar si las hamburguesas están agotadas
        // Esperar hasta que sea el turno de este hilo
        while (turno != thread_id)
            ; // Espera ocupada (busy waiting)

        // Verificar si todavía hay hamburguesas disponibles
        if (cantidad_restante_hamburguesas > 0) {
            printf("Hola! soy el hilo(comensal) %d, me voy a comer una hamburguesa! Quedan %d hamburguesas\n", thread_id, cantidad_restante_hamburguesas);
            cantidad_restante_hamburguesas--; // Comer una hamburguesa
        } else {
            // Indicar que se terminaron las hamburguesas
            hamburguesas_agotadas = true;
            printf("SE TERMINARON LAS HAMBURGUESAS para el hilo %d\n", thread_id);
        }

        // Cambiar el turno al siguiente hilo
        turno = (turno + 1) % NUMBER_OF_THREADS;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t threads[NUMBER_OF_THREADS];
    int status, i, ret;
    for (i = 0; i < NUMBER_OF_THREADS; i++) {
        printf("Hola!, soy el hilo principal. Estoy creando el hilo %d\n", i);
        status = pthread_create(&threads[i], NULL, comer_hamburguesa, (void *)i);
        if (status != 0) {
            printf("Algo salió mal al crear el hilo. Código de error: %d\n", status);
            exit(-1);
        }
    }

    for (i = 0; i < NUMBER_OF_THREADS; i++) {
        void *retval;
        ret = pthread_join(threads[i], &retval); // Esperar a la terminación de los hilos
    }
    pthread_exit(NULL); // Terminar el programa principal
}
