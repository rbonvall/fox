#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include "matrix.h"


struct grid_info {
    int      nr_world_processes;
    MPI_Comm grid_comm;
    MPI_Comm row_comm;
    MPI_Comm col_comm;
    int      ppside;
    int      my_row, my_col, my_rank;
};

void grid_setup(struct grid_info *grid);
void Fox(struct grid_info *grid, int n, 
         matrix_type** local_A, matrix_type** local_B, matrix_type** local_C);
int intsqrt(int x);


int main(int argc, char *argv[]) {
    struct grid_info grid;

    MPI_Init(&argc, &argv);
    grid_setup(&grid);


    MPI_Finalize();
    return 0;
}





void Fox(struct grid_info *grid, int n, 
         matrix_type** local_A, matrix_type** local_B, matrix_type** local_C)
{
    float **temp_A;
    int stage;
    int bcast_root;
    MPI_Status status;

    int n_bar = n / grid->ppside;

    int i, j;
    for (i = 0; i < n; ++i)
        for (j = 0; j < n; ++j)
            local_C[i][j] = 0;

    int src  = (grid->my_row + 1) % grid->ppside;
    int dest = (grid->my_row + grid->ppside - 1) % grid->ppside;

    temp_A = matrix_new(n_bar, n_bar);

    for (stage = 0; stage < grid->ppside; ++stage) {
        bcast_root = (grid->my_row + stage) % grid->ppside;
        if (bcast_root == grid->my_col) {
            MPI_Bcast(local_A, 1, local_matrix_mpi_t, bcast_root, grid->row_comm);
            matrix_multiply(local_A, local_B, local_C, n, n, n);
        }
        else {
            MPI_Bcast(temp_A, 1, local_matrix_mpi_t, bcast_root, grid->row_comm);
            matrix_multiply(temp_A, local_B, local_C, n, n, n);
        }
        MPI_Sendrecv_replace(local_B, 1, local_matrix_mpi_t, dest, 0, src, 0, grid->col_comm, &status);
    }
}


void grid_setup(struct grid_info *grid) {
    /* obtener datos globales */
    int world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &(grid->nr_world_processes));
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    /* calcular cuantos procesos por lado tendra la grilla */
    grid->ppside = intsqrt(grid->nr_world_processes);

    /* crear comunicador para topologia de grilla */
    int dimensions[2]  = {grid->ppside, grid->ppside};
    int wrap_around[2] = {1, 1};
    int reorder = 1;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dimensions, wrap_around, reorder, &(grid->comm));
    MPI_Comm_rank(grid->comm, &(grid->my_rank));

    /* obtener coordenadas grillisticas del proceso */
    int coordinates[2];
    MPI_Cart_coords(grid->comm, grid->my_rank, 2, coordinates);
    grid->my_row = coordinates[0];
    grid->my_col = coordinates[1];

    /* obtener comunicadores para la fila y la columna del proceso */
    int free_coords_for_rows[] = {0, 1}
    int free_coords_for_cols[] = {1, 0}
    MPI_Cart_sub(grid->comm, free_coords_for_rows, &(grid->row_comm));
    MPI_Cart_sub(grid->comm, free_coords_for_cols, &(grid->col_comm));
}


int intsqrt(int x) {
    /* TODO: implementacion mas eficiente */
    return (int) sqrt((double) x);
}

