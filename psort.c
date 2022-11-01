#include <sys/sysinfo.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct record {
  int key;
  char remainder[96];
} this_record;

// function that swaps elements
void swap(struct record *a, struct record *b){
  struct record tmp = *a;
  *a = *b;
  *b = tmp;
}

int partition(struct record array[],int low, int high){
  int pivot = array[high].key;

  int j = low - 1;

  for(int i = low; i < high; i++){
    if (array[i].key <= pivot){
      j++;
      swap(&array[i],&array[j]);
    }
  }

  swap(&array[j+1],&array[high]);
  return j + 1;
}

//function that performs quicksort
void quicksort(struct record array[],int low, int high){
  if (low < high) {
    int m = partition(array,low,high);
    quicksort(array,low,m-1);
    quicksort(array,m+1,high);
  }
}

int main(int argc, char* argv[]){
    // too little arguments
    if (argc != 3){
        return -1;
    }

    FILE *fp;
    char *inputFile;
    char *outputFile;
    char line[256];
    this_record *array;
    array = malloc(100*sizeof(this_record));

    inputFile = argv[1];
    outputFile = argv[2];

    int numLines = 0;
    fp=fopen(inputFile,"r");
    if(fp==NULL) {
        printf("File \"%s\" does not exist!\n", inputFile);
    }

    printf("Size: %ld", sizeof(this_record));
    while(fgets(line, 256, fp)){
        this_record new_record;
        char *ptr = line;
        int key = *(int *)line;
        new_record.key = key;
        memcpy(new_record.remainder, ptr+4, 96);
        array[numLines] = new_record;
        numLines+=1;
    }
    fclose(fp);

    /*
    The input file will consist of records; within each record is a key. The key is the first four bytes of the record. The records are fixed-size, and are each 100 bytes (which includes the key).
    A successful sort will read all the records into memory from the input file, sort them, and then write them out to the output file.
    You also have to force writes to disk by calling fsync() on the output file before finishing.
    You can assume that this is a one-pass sort, i.e., the data can fit into memory. You do not have to implement a multi-pass sort.
    */

    // *(int*)(input + i*100 + j-4)
    // int key = *(int *)memaddr;

    /*
    int values[1000];
    int num_of_process = get_nprocs();
    int num_of_threads = numLines/num_of_process;
    for(int i = 0; i < num_of_threads; i++){
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, sort, values);
        pthread_join(thread_id, NULL);
    }
    */

  return -1;
}

/*
// sorting comparison
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void sort(int* values){
    qsort(values, 5, sizeof(int), cmpfunc);
    printf("New Thread");
}
*/
