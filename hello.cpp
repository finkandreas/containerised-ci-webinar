#include <mpi.h>
#include <iostream>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    auto comm = MPI_COMM_WORLD;
    int comm_rank, comm_size;
    MPI_Comm_size(comm, &comm_size);
    MPI_Comm_rank(comm, &comm_rank);

    std::cout << "Hello from " << comm_rank << "/" << comm_size << std::endl;
}
