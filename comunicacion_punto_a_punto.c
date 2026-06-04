/*
 * =====================================================================
 *  Actividad de Programación #4
 *  Comunicación Punto a Punto con MPI
 * ---------------------------------------------------------------------
 *  Autor:        Eduardo
 *  Universidad:  Universidad Iberoamericana (UNIBE)
 *  Asignatura:   Programación Paralela
 *  Fecha:        Mayo - Junio 2026
 * ---------------------------------------------------------------------
 *  Descripción:
 *      Programa que demuestra la comunicación punto a punto entre dos
 *      procesos MPI. El proceso 0 envía un valor entero al proceso 1
 *      mediante MPI_Send, y el proceso 1 lo recibe con MPI_Recv y lo
 *      imprime junto con el identificador del proceso emisor.
 *
 *  Compilación:
 *      mpicc -Wall -o comunicacion_punto_a_punto comunicacion_punto_a_punto.c
 *
 *  Ejecución (mínimo 2 procesos):
 *      mpirun -np 2 ./comunicacion_punto_a_punto
 * =====================================================================
 */

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

    /* 1. Inicialización del entorno MPI ------------------------------ */
    MPI_Init(&argc, &argv);

    /* 2. Obtención del rank del proceso actual ----------------------- */
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* 3. Obtención del número total de procesos ---------------------- */
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

    /* 4. Comunicación punto a punto ---------------------------------- */
    if (rank == 0) {
        /* El proceso 0 actúa como EMISOR */
        dato = VALOR_A_ENVIAR;

        printf("[Proceso %d] Preparando envio del valor %d al proceso 1...\n",
               rank, dato);

        MPI_Send(&dato,          /* Dirección del buffer a enviar           */
                 1,              /* Cantidad de elementos                   */
                 MPI_INT,        /* Tipo de dato MPI                        */
                 1,              /* Rank del proceso destino                */
                 TAG_MENSAJE,    /* Etiqueta del mensaje                    */
                 MPI_COMM_WORLD  /* Comunicador                             */
        );

        printf("[Proceso %d] Envio completado exitosamente.\n", rank);
    }
    else if (rank == 1) {
        /* El proceso 1 actúa como RECEPTOR */
        printf("[Proceso %d] Esperando recibir un mensaje del proceso 0...\n",
               rank);

        MPI_Recv(&dato,          /* Dirección del buffer de recepción       */
                 1,              /* Cantidad máxima de elementos a recibir  */
                 MPI_INT,        /* Tipo de dato MPI                        */
                 0,              /* Rank del proceso origen                 */
                 TAG_MENSAJE,    /* Etiqueta del mensaje esperado           */
                 MPI_COMM_WORLD, /* Comunicador                             */
                 &estado         /* Estructura MPI_Status con metadatos     */
        );

        printf("[Proceso %d] Valor recibido: %d (enviado por el proceso %d, "
               "etiqueta %d)\n",
               rank, dato, estado.MPI_SOURCE, estado.MPI_TAG);
    }
    else {
        /* Procesos adicionales (rank >= 2) no participan */
        printf("[Proceso %d] No participa en la comunicacion.\n", rank);
    }

    /* 5. Finalización del entorno MPI -------------------------------- */
    MPI_Finalize();

    return EXIT_SUCCESS;
}
