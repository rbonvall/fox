#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include "matrix.h"

#define DEBUG

enum {FALSE = 0, TRUE = 1};

struct grid_info {
    int      nr_world_processes;
    MPI_Comm comm;
    MPI_Comm row_comm;
    MPI_Comm col_comm;
    int      ppside;
    int      my_row, my_col, my_rank, my_world_rank;
};

void grid_setup(struct grid_info *grid);
void grid_info_print(struct grid_info *grid);
void Fox(struct grid_info *grid, int n, 
         matrix_type** local_A, matrix_type** local_B, matrix_type** local_C);
int intsqrt(int x);


int main(int argc, char *argv[]) {
    struct grid_info grid;

    MPI_Init(&argc, &argv);

    grid_setup(&grid);
    grid_info_print(&grid);

    /* lado de una submatriz */
    int n = 4;
    int local_n = n / grid.ppside;

    char A_filename[50], B_filename[50];
    sprintf(A_filename, "data_localn2_side2/A_localn2_row%d_col%d", grid.my_row, grid.my_col);
    sprintf(B_filename, "data_localn2_side2/B_localn2_row%d_col%d", grid.my_row, grid.my_col);

    matrix_type **local_A, **local_B, **local_C;
    local_A = matrix_new_from_file(local_n, local_n, A_filename);
    local_B = matrix_new_from_file(local_n, local_n, B_filename);
    local_C = matrix_new(local_n, local_n);

    Fox(&grid, local_n, local_A, local_B, local_C);

    printf("A%d%d =\n", grid.my_row, grid.my_col);
    matrix_print(stdout, local_A, local_n, local_n);
    printf("B%d%d =\n", grid.my_row, grid.my_col);
    matrix_print(stdout, local_B, local_n, local_n);
    printf("C%d%d =\n", grid.my_row, grid.my_col);
    matrix_print(stdout, local_C, local_n, local_n);

    MPI_Finalize();
    return 0;
}




/* algoritmo de Fox para multiplicar matrices cuadradas de n x n */
void Fox(struct grid_info *grid, int local_n, 
         matrix_type** local_A, matrix_type** local_B, matrix_type** local_C)
{
    int stage;
    int i, j;
    const int local_n_sq = local_n * local_n;
    const int src  = (grid->my_row + 1) % grid->ppside;
    const int dest = (grid->my_row + grid->ppside - 1) % grid->ppside;
    int bcast_root;
    MPI_Status status;
    matrix_type **temp_A = matrix_new(local_n, local_n);

#ifdef DEBUG
    char out_filename[20];
    sprintf(out_filename, "debug_out_%d_%d", grid->my_row, grid->my_col);

    FILE *debug_out = fopen(out_filename, "w");
    if (debug_out == NULL) {
        fprintf(stderr, "Could not open debugging output file\n");
        exit(-1);
    }
#endif

#ifdef DEBUG
#endif
    for (stage = 0; stage < grid->ppside; ++stage) {
#ifdef DEBUG
        fprintf(debug_out, "Entering stage %d\n", stage);
#endif
        bcast_root = (grid->my_row + stage) % grid->ppside;
        if (bcast_root == grid->my_col) {
#ifdef DEBUG
            fprintf(debug_out, "Sending this submatrix across the row:\n");
            matrix_print(debug_out, local_A, local_n, local_n);
#endif
            MPI_Bcast(local_A, local_n * local_n, MPI_FLOAT, bcast_root, grid->row_comm);
            matrix_multiply_and_add(local_A, local_B, local_C, local_n, local_n, local_n);
        }
        else {
            MPI_Bcast(temp_A,  local_n_sq, MPI_FLOAT, bcast_root, grid->row_comm);
#ifdef DEBUG
            fprintf(debug_out, "Received this matrix from somewhere on the row:\n");
            matrix_print(debug_out, temp_A, local_n, local_n);
#endif
            matrix_multiply_and_add(temp_A, local_B, local_C, local_n, local_n, local_n);
        }
#ifdef DEBUG
        fprintf(debug_out, "C now is:\n");
        matrix_print(debug_out, local_C, local_n, local_n);
#endif
        MPI_Sendrecv_replace(*local_B, local_n_sq, MPI_FLOAT, dest, 0, src, 0, grid->col_comm, &status);
#ifdef DEBUG
        fprintf(debug_out, "B now is:\n");
        matrix_print(debug_out, local_B, local_n, local_n);
#endif
    }

#ifdef DEBUG
    fclose(debug_out);
#endif
}


void grid_setup(struct grid_info *grid) {
    /* obtener datos globales */
    MPI_Comm_size(MPI_COMM_WORLD, &(grid->nr_world_processes));
    MPI_Comm_rank(MPI_COMM_WORLD, &(grid->my_world_rank));

    /* calcular cuantos procesos por lado tendra la grilla */
    grid->ppside = intsqrt(grid->nr_world_processes);

    /* crear comunicador para topologia de grilla */
    int dimensions[2]  = {grid->ppside, grid->ppside};
    int wrap_around[2] = {TRUE, TRUE};
    int reorder = TRUE;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dimensions, wrap_around, reorder, &(grid->comm));
    MPI_Comm_rank(grid->comm, &(grid->my_rank));

    /* obtener coordenadas grillisticas del proceso */
    int coordinates[2];
    MPI_Cart_coords(grid->comm, grid->my_rank, 2, coordinates);
    grid->my_row = coordinates[0];
    grid->my_col = coordinates[1];

    /* obtener comunicadores para la fila y la columna del proceso */
    int free_coords_for_rows[] = {FALSE, TRUE};
    int free_coords_for_cols[] = {TRUE, FALSE};
    MPI_Cart_sub(grid->comm, free_coords_for_rows, &(grid->row_comm));
    MPI_Cart_sub(grid->comm, free_coords_for_cols, &(grid->col_comm));
}


void grid_info_print(struct grid_info *grid) {
    printf("nr_world_processes: %d\n", grid->nr_world_processes);
    printf("ppside: %d\n", grid->ppside);
}

int intsqrt(int x) {
    /* TODO: implementacion mas eficiente */
    return (int) sqrt((double) x);
}

