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

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    malloc2dfloat(&array, 10, 10);//Asigna memoria a la matriz que va a ser enviada
    if (rank == 0){
      cout << "Hola del proce: " << rank << endl;
      for(i=0;i<10;i++){
        for(j=0;j<10;j++){
            array[i][j]=i+j;
            cout << array[i][j];
        }
        cout << endl;
      }
    }

    MPI_Bcast(&(array[0][0]), 10*10, MPI_FLOAT, 0, MPI_COMM_WORLD);
    cout << endl << endl;

    if (rank == 1){
      cout << "Hola del proce: " << rank << endl;
      for(i=0;i<10;i++){
        for(j=0;j<10;j++){
              cout << array[i][j];
        }
        cout << endl;
      }
    }

    MPI_Finalize();
}
