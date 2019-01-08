#include <iostream>
#include <stdlib.h>
#include <mpi.h>

using namespace std;

int malloc2dfloat(float ***array, int n, int m) {

    /* allocate the n*m contiguous items */
    float *p = (float *)malloc(n*m*sizeof(float));
    if (!p) return -1;

    /* allocate the row pointers into the memory */
    (*array) = (float **)malloc(n*sizeof(float*));
    if (!(*array)) {
       free(p);
       return -1;
    }

    /* set up the pointers into the contiguous memory */
    for (int i=0; i<n; i++)
       (*array)[i] = &(p[i*m]);

    return 0;
}

int free2dfloat(float ***array) {
    /* free the memory - the first element of the array is at the start */
    free(&((*array)[0][0]));

    /* free the pointers into the memory */
    free(*array);

    return 0;
}





int main(int argc, char **argv)
{
    float **array;
    int rank,size,i,j;
    float numero = 0;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    malloc2dfloat(&array, 9, 9);//Asigna memoria a la matriz que va a ser enviada
    if (rank == 0){
      cout << "Hola del proce: " << rank << " y el numero es: " << numero << endl;
      for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            array[i][j]=0;
            cout << array[i][j];
        }
        cout << endl;
      }
    }

    MPI_Barrier(MPI_COMM_WORLD);// sincroniza los procesadores
    MPI_Bcast(&numero, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&(array[0][0]), 9*9, MPI_FLOAT, 0, MPI_COMM_WORLD);
    cout << endl << endl;

    if (rank == 1){
      numero = 1;
      cout << "Hola del proce: " << rank << " y el numero es: " << numero << endl;
      for(i=0;i<9;i++){
        cout << "Valor numero enviado: ";
        for(j=0;j<9;j++){
              array[i][j] = numero;
              cout << array[i][j];
        }
        cout << endl;
      }
    }


    MPI_Finalize();
}
