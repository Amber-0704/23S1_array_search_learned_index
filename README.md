# Array search learned index

This project is a C-based implementation of a learned index search algorithm designed for fast lookups in sorted datasets. It combines linear regression prediction with segmented indexing and binary search refinement to achieve higher efficiency compared to traditional search methods.

## Features

- Read and sort input data using quicksort
- Build one or more linear regression models to predict key positions
- Segment the dataset to reduce prediction errors
- Use binary search within bounded ranges for precise lookup
- Support multiple search strategies for experimentation
- Flexible input handling via standard input redirection

## File Structure

```
.
├── program.c            # Main source code
├── test0.txt            # Sample input 0
├── test0-output.txt     # Sample expected output 0
├── test1.txt            # Sample input 1
├── test1-output.txt     # Sample expected output 1
```

## Compilation

Compile the program using `gcc`:

```bash
gcc program.c -o learned_index_search -lm
```

## Usage

Run the program by redirecting an input file:

```bash
./learned_index_search < test0.txt
```

Compare the output to the expected results in `test0-output.txt` or `test1-output.txt`.

## Algorithm Overview

1. Sort the dataset using quicksort.  
2. Build linear regression functions to map keys to predicted positions.  
3. Segment data when prediction error exceeds a set bound.  
4. For each query, use the corresponding model to predict the position.  
5. Refine the prediction using binary search within the bounded range.

## Notes

- Written in standard C, requires the math library (`-lm`) during compilation.
- Input files must follow the required format for correct execution.
