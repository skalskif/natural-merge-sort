# Natural merge sort
Natural merge sorting algorithm implementation

## General info
The project was made for academic purposes, specifically for "Database Structures" subject. It is a console app sorting serial files using a natural merge algorithm. See [Technical details](#technical-details) section for more information.

## Technical details
All parameters and formats given below are hardcoded for the project requirements.

### Method
The scheme used for implemented algorithm is 3+1, which means that we use 4 tapes - three for records distribution and one for merging.

### Input file format
The input file is a binary file with records serialized one by one, without no bytes separating them. Each record has two integer values - one representing the first element of geometric sequence, and the other being its common ratio. The program has the feature of randomly generating input data.

### Sorting criterion
The records are sorted in ascending order by the sum of the first three sequence elements.

### Block operations
Data is read/written in blocks of 40 bytes, each containing five records.

### Results display
Records are displayed in format: 
\<first sequence element\> \<sequence common ratio\> \<sum of first three elements\>
The sum is not saved in the file. File content is displayed before and after sorting. Program also shows how many phases and read/write operations it took. There is also an option to display data after every sorting phase.

## Build
Build for CMake v.3.26+

```
mkdir build
cd build
cmake ..
cmake --build .
```

