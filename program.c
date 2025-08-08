/* A simplified learned index implementation:
 *
 * Skeleton code written by Jianzhong Qi, April 2023
 * Edited by: [Muhan Chu 1346862]
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STAGE_NUM_ONE 1                       /* stage numbers */ 
#define STAGE_NUM_TWO 2
#define STAGE_NUM_THREE 3
#define STAGE_NUM_FOUR 4
#define STAGE_HEADER "Stage %d\n==========\n" /* stage header format string */

#define DATASET_SIZE 100                      /* number of input integers */
#define DATA_OUTPUT_SIZE 10                   /* output size for stage 1 */

#define BS_NOT_FOUND (-1)                     /* used by binary search */
#define BS_FOUND 0

typedef int data_t;                           /* data type */

/****************************************************************/

/* function prototypes */
void print_stage_header(int stage_num);
int cmp(data_t *x1, data_t *x2);
void swap_data(data_t *x1, data_t *x2);
void partition(data_t dataset[], int n, data_t *pivot,
        int *first_eq, int *first_gt);
void quick_sort(data_t dataset[], int n);
int binary_search(data_t dataset[], int lo, int hi, 
    data_t *key, int *locn);

void stage_one(data_t dataset[]);
void stage_two(data_t dataset[]);
int stage_three(data_t dataset[],data_t array_a[], 
              data_t array_b[], data_t array_num[], int* max_error);
void stage_four(data_t datset[], data_t array_a[], 
       data_t array_b[],data_t array_num[], 
       data_t count, int max_error);

/* add your own function prototypes here */
data_t max(data_t num1,data_t num2);
data_t min(data_t num1,data_t num2);
/****************************************************************/

/* main function controls all the action */
int
main(int argc, char *argv[]) {
    /* to hold all input data */
    data_t dataset[DATASET_SIZE];
    data_t array_a[DATASET_SIZE];
    data_t array_b[DATASET_SIZE];
    data_t array_num[DATASET_SIZE];
    data_t max_error;
    /* to hold the mapping functions */


    /* stage 1: read and sort the input */
    stage_one(dataset); 
    
    /* stage 2: compute the first mapping function */
    stage_two(dataset);

    /* stage 3: compute more mapping functions */ 
    data_t count = stage_three(dataset, array_a, array_b, 
                     array_num, &max_error);
    
    /* stage 4: perform exact-match queries */
    stage_four(dataset, array_a, array_b, array_num, count, max_error);
    
    /* all done; take some rest */
    return 0;
}

/****************************************************************/

/* add your code below; you can also modify the function return type 
   and parameter list 
*/



/* stage 1: read and sort the input */
void stage_one(data_t dataset[]) {
    /* Read 100 numbers and store them in the dataset array */
    data_t num_read = 0;
    /* Keep reading the first 100 numbers */
    while (num_read < DATASET_SIZE) {
        data_t num;
        scanf("%d",&num);
        dataset[num_read] = num;
        num_read += 1;
    }
    /* Sort the 100 numbers */
    quick_sort(dataset,num_read);
    /* print stage header */
    print_stage_header(STAGE_NUM_ONE);
    /* print the first 10 numbers */
    printf("First 10 numbers: ");
    data_t i;
    /* print the first 10 sorted numbers */
    for(i = 0; i < DATA_OUTPUT_SIZE; i++) {
        /* Different locations require 
           different amounts of space */
        if(i == DATA_OUTPUT_SIZE -1) {
            printf("%d\n", dataset[i]);
        } else {
            printf("%d ", dataset[i]);
        }

    }

    printf("\n");
}



/* stage 2: compute the first mapping function */
void stage_two(data_t dataset[]) {
    /* Calculate a, b */
    data_t a = (dataset[1] * 0 - dataset[0] * 1) / (1 - 0);
    data_t b = (dataset[1] - dataset[0]) / (1 - 0);
    data_t i, prediction, pred_error, index, max_error = -1;
    /* Calculate the prediction and prediction error for each number */
    for(i = 0; i < DATASET_SIZE; i++) {

        /* Determines whether the previous and next numbers are equal */
        if(dataset[0] == dataset[1]) {
            prediction = 0;
        } else {
            prediction = fabs(ceil((dataset[i] + a) / (1.0 * b)));
        }
        pred_error = abs(prediction - i);

        /* Find the maximum error, and record the position */
        if(pred_error > max_error) {
            max_error = pred_error;
            index = i;
        } 
    }
    /* print stage header */
    print_stage_header(STAGE_NUM_TWO);
    printf("Maximum prediction error: %d\n", max_error);
    printf("For key: %d\n", dataset[index]);
    printf("At position: %d\n", index);
    printf("\n");
}



/* stage 3: compute more mapping functions */ 
int 
stage_three(data_t dataset[], data_t array_a[], data_t array_b[], 
            data_t array_num[], int* max_error) {
    scanf("%d", max_error);
    data_t a, b, i, prediction, pred_error;
    /* Initialize a, b */
    if(dataset[1] == dataset[0]) {
        a = 0;
        b = 0;
    } else {
        a = ((dataset[1] * 0 -  dataset[0] * 1) / (1 - 0));
        b = (dataset[1]-dataset[0]) / (1 - 0);
    }

    data_t count = 0; 
    /* Used to record the location that is running */
    data_t current_index;
    /* Loop over every number in the dataset */  
    for(i = 2; i < DATASET_SIZE; i++) {

        /* Calculating prediction error */
        if(b==0) {
            prediction = current_index;
        } else {
            prediction = fabs(ceil((dataset[i] + a)/(1.0 * b)));
        }

        pred_error = abs(prediction - i);
        /* Check that the current index runs to the 
           last bit of the dataset */
        if(i == DATASET_SIZE-1 && pred_error <= *max_error) {
            array_a[count] = a;
            array_b[count] = b;
            array_num[count] = dataset[i];
            count += 1;
        }

        /* Determine whether the number is covered，and if so, 
            record the current a, b and the previous number covered*/
        if(pred_error > *max_error) {
            current_index = i;
            array_a[count] = a;
            array_b[count] = b;
            array_num[count] = dataset[i-1];
            /* Update the values of a and b according to different situations */
            if(dataset[i] == dataset[i+1]) {
                a = current_index;
                b = 0;
            } else {    
                a = ((dataset[i + 1] * i - (i + 1) * dataset[i]) / (i+1-i));
                b = (dataset[i + 1]-dataset[i]) / (i + 1 - i);
            }

            /* recount the number */
            count += 1;
        }  

    }
    /* print stage header */
    print_stage_header(STAGE_NUM_THREE);
    printf("Target maximum prediction error: %d\n",*max_error);
    /* print the final value */
    for(i = 0; i < count; i++){
        printf("Function %2d: a = %4d, b = %3d, max element =%4d\n", i, 
                array_a[i], array_b[i], array_num[i]);
    }
    printf("\n");
    return count;
}




/* stage 4: perform exact-match queries */
void stage_four(data_t dataset[], data_t array_a[], data_t array_b[], 
                data_t array_num[], int count, int max_error) {
    print_stage_header(STAGE_NUM_FOUR);
    data_t key; /* The number we need to search */
    /* The last line of digits is read until all digits are read */
    while(scanf("%d", &key) == 1) {
        printf("Searching for %d:\n", key);
        /* Check whether key is in the range */
        if(key < dataset[0] || key >dataset[DATASET_SIZE-1]) {
            printf("Step 1: not found!\n");
        } else {
            printf("Step 1: search key in data domain.\n");
            int locn;
            printf("Step 2:");
            /* Do binary search on the max_element array */
            binary_search(array_num, 0, count, &key, &locn);
            printf("\n");
            /* Find the corresponding a and b */
            data_t a = array_a[locn];
            data_t b = array_b[locn];
            double pred;

            if (b == 0){
                pred = a;
            } else {
                pred = ceil((key + a)/(1.0 * b));
            }
            /* Define the new range in dataset */
            data_t lo = max(0, pred - max_error);
            data_t hi = min(DATASET_SIZE - 1,
                              pred + max_error) + 1;
            printf("Step 3:");
            /* Do binary search on the dataset array */
            binary_search(dataset, lo, hi, &key, &locn);
            /* Check the number */
            if(key == dataset[locn]){
                printf(" @ dataset[%d]!\n", locn);
            } else {
                printf(" not found!\n");
            }

        }

    }

    printf("\n");
}


/* Find the larger of the two numbers */
data_t max(data_t num1,data_t num2) {
    /* çompare the number */
    if(num1 > num2) {
        return num1;
    }
    
    return num2;
}

/*Find the larger of the two numbers*/
data_t min(data_t num1,data_t num2) {
    /* Compare the number */
    if(num1 > num2) {
        return num2;
    }

    return num1;
}

/****************************************************************/
/* functions provided, adapt them as appropriate */

/* print stage header given stage number */
void 
print_stage_header(int stage_num) {
    printf(STAGE_HEADER, stage_num);
}

/* data swap function used by quick sort, adpated from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/sortscaffold.c
*/
void
swap_data(data_t *x1, data_t *x2) {
    data_t t;
    t = *x1;
    *x1 = *x2;
    *x2 = t;
}

/* partition function used by quick sort, from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/quicksort.c 
*/
void
partition(data_t dataset[], int n, data_t *pivot,
        int *first_eq, int *first_gt) {
    int next=0, fe=0, fg=n, outcome;
    
    while (next<fg) {

        if ((outcome = cmp(dataset+next, pivot)) < 0) {
            swap_data(dataset+fe, dataset+next);
            fe += 1;
            next += 1;
        } else if (outcome > 0) {
            fg -= 1;
            swap_data(dataset+next, dataset+fg);
        } else {
            next += 1;
        }
    }
    
    *first_eq = fe;
    *first_gt = fg;
    return;
}

/* quick sort function, adapted from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/quicksort.c 
*/
void
quick_sort(data_t dataset[], int n) {
    data_t pivot;
    int first_eq, first_gt;

    if (n<=1) {
        return;
    }
    /* array section is non-trivial */
    pivot = dataset[n/2]; // take the middle element as the pivot
    partition(dataset, n, &pivot, &first_eq, &first_gt);
    quick_sort(dataset, first_eq);
    quick_sort(dataset + first_gt, n - first_gt);
}

/* comparison function used by binary search and quick sort, from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/binarysearch.c 
*/
int
cmp(data_t *x1, data_t *x2) {
    return (*x1-*x2);
}

/* binary search between dataset[lo] and dataset[hi-1], adapted from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/binarysearch.c 
*/
int
binary_search(data_t dataset[], int lo, int hi, 
        data_t *key, int *locn) {
    int mid, outcome;
    /* if key is in dataset, it is between dataset[lo] and dataset[hi-1] */
    if (lo >= hi) {
        *locn =lo;
        return BS_NOT_FOUND;
    }

    mid = (lo+hi)/2;
    printf(" %d", dataset[mid]);
    
    if ((outcome = cmp(key, dataset+mid)) < 0) {
        return binary_search(dataset, lo, mid, key, locn);
    } else if (outcome > 0) {
        return binary_search(dataset, mid+1, hi, key, locn);
    } else {
        *locn = mid;
        return BS_FOUND;
    }
    
}