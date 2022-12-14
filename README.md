<h1 align="center">Searching a Number Among Slave Processes Using OpenMP and MPI</h1>

### Description
Program written in `C++ language` to search a number from an array after distributing it equally to all `Slave Processes` and using `OpenMP` and `MPI` for `Parallel Searching`. You can view the detailed project description [Here](https://github.com/SameetAsadullah/Searching-a-Number-Among-Slave-Processes-Using-OpenMP-and-MPI/blob/main/Project%20Statement.pdf).

### Manual
1) Use following command to `Compile the Code`:
    ```
    mpic++ -fopenmp Source.cpp -o Source
    ```

2) Use following command to `Execute the Code`. 3 indicates total number of processes to create:
    ```
    mpiexec -n 3 -f machinefile ./Source
    ```
    
### Working Screenshots
<div align="center">
  <img src = "https://github.com/SameetAsadullah/Searching-a-Number-Among-Slave-Processes-Using-OpenMP-and-MPI/blob/main/extras/working-ss-1.png" alt = "" width="700px"/>
</div>
<br/>
<div align="center">
  <img src = "https://github.com/SameetAsadullah/Searching-a-Number-Among-Slave-Processes-Using-OpenMP-and-MPI/blob/main/extras/working-ss-1.png" alt = "" width="700px"/>
</div>
