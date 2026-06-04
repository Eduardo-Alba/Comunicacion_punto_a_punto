/* Actividad 4: Comunicación Punto a Punto con MPI


#include <stdio.h>      /* printf, fprintf */
#include <stdlib.h>     /* EXIT_SUCCESS, EXIT_FAILURE */
#include <mpi.h>        /* API de Message Passing Interface */

#define VALOR_A_ENVIAR  100   /* Valor entero que el proceso 0 enviará */
#define TAG_MENSAJE     0     /* Etiqueta utilizada para identificar el mensaje */

int main(int argc, char *argv[])
{
    int rank;          /* Identificador (rank) del proceso actual */
    int size;          /* Número total de procesos en el comunicador */
    int dato;          /* Variable donde se almacena el valor enviado/recibido */
    MPI_Status estado; /* Estructura que guarda metadatos del mensaje recibido */

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* Validación: el programa requiere al menos 2 procesos */
    if (size < 2) {
        if (rank == 0) {
            fprintf(stderr,
                    "Error: este programa requiere al menos 2 procesos.\n"
                    "Ejecutelo con: mpirun -np 2 ./comunicacion_punto_a_punto\n");
        }
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    /* ACTIVIDAD 4: Comunicación punto a punto */
    if (rank == 0) {
        dato = VALOR_A_ENVIAR;
        printf("[Proceso %d] Preparando envio del valor %d al proceso 1...\n",
               rank, dato);

        MPI_Send(&dato, 1, MPI_INT, 1, TAG_MENSAJE, MPI_COMM_WORLD);

        printf("[Proceso %d] Envio completado exitosamente.\n", rank);
    }
    else if (rank == 1) {
        printf("[Proceso %d] Esperando recibir un mensaje del proceso 0...\n",
               rank);

        MPI_Recv(&dato, 1, MPI_INT, 0, TAG_MENSAJE, MPI_COMM_WORLD, &estado);

        printf("[Proceso %d] Valor recibido: %d (enviado por el proceso %d, "
               "etiqueta %d)\n",
               rank, dato, estado.MPI_SOURCE, estado.MPI_TAG);
    }
    else {
        printf("[Proceso %d] No participa en la comunicacion.\n", rank);
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}
