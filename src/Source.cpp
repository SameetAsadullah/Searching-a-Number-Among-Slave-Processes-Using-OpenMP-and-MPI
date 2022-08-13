#include <iostream>
#include "mpi.h"
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <unistd.h>
using namespace std;

int main(int argc, char **argv)
{
    int *arr, rank, root = 0, nprocs, namelen;
    char processorName[10];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Get_processor_name(processorName, &namelen);

    if (nprocs != 1) {  // Program should run on more than 1 processes
        int size = 20;  // Size of array to be equally divided between slaves
        int size_of_arrays = size/(nprocs - 1);     // calculating size of divided arrays
        int number_to_search = 31;   // number to search between the data given to slaves

        if (rank == root) {     // master process
            cout << "Name: Sameet Asadullah\nRoll Number: 18i-0479\n\n";

            cout << processorName << ": The number to search is " << number_to_search << "\n";
            cout << "Process 0 has input data: "; 
            arr = new int[size];
            int temp = 0;
            for (int i = 1; i < size * 2; i += 2) {    // initializing the main array
                arr[temp] = i;
                cout << arr[temp] << " ";
                temp++;
            }
            cout << "\n";

            for (int i = 1, j = 0; i < nprocs; ++i) {   // dividing the main array equally and sending it to all the slaves
                int *arr_to_send = new int[size_of_arrays];
                for (int k = 0; k < size_of_arrays; ++k, ++j) {
                    arr_to_send[k] = arr[j];
                }
                MPI_Send(arr_to_send, size_of_arrays, MPI_INT, i, 1234, MPI_COMM_WORLD);
                delete[] arr_to_send;
            }

            if (nprocs != 2) {  // if there's only one slave then it'll def find the number. So, no need to check.
                MPI_Status status;
                char data[27];
                MPI_Recv(data, 27, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

                if (strcmp(data, "I have found the number :-)") == 0) {
		            sleep(1);
                    cout << processorName << ": Process " << status.MPI_SOURCE << " has found the number!\n";
                    cout << processorName << ": Informing all processes to abort!\n";
                    for (int i = 1; i < nprocs; ++i) {  // telling slaves to abort search
                        char abort_message[6] = "ABORT";
                        MPI_Send(abort_message, 6, MPI_CHAR, i, 1236, MPI_COMM_WORLD);
                    }
                }
            }
        } else {    // slave processes
            arr = new int[size_of_arrays];
            MPI_Recv(arr, size_of_arrays, MPI_INT, 0, 1234, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            cout << "Process " << rank << " has local data: ";
            for (int i = 0; i < size_of_arrays; ++i) {
                cout << arr[i] << " ";
            }
            cout << "\n";
            sleep(2);

            char abort_message[6];
            #pragma omp parallel num_threads(2) // making two threads, one for searching and one for receiving abort message from master
            {
                if (omp_get_thread_num() == 0) {    // thread to search number
                    for (int i = 0; i < size_of_arrays; ++i) {
                        if (strcmp(abort_message, "ABORT") == 0) {  // if master said to abort the search
                            cout << "Process " << rank << ": Aborting search!\n";
                            break;
                        }

                        if (arr[i] == number_to_search) {   // if number found then letting master know and ending the search
                            char message[] = "I have found the number :-)";
                            MPI_Send(message, 27, MPI_CHAR, 0, 1235, MPI_COMM_WORLD);
                            cout << "Process " << rank << ": " << message << "\n";
                            break;
                        }

                        if (rank != 4) {
                            sleep(2);
                        }
                    }
                } else {    // thread to receive abort message
                    if (nprocs != 2) {
                        MPI_Recv(abort_message, 6, MPI_CHAR, 0, 1236, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    }
                }
            }
            delete[] arr;
        }
    } else {
        cout << "No. of processes must be more than 1." << endl;
    }
    MPI_Finalize();
}