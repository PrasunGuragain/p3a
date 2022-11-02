#include <sys/sysinfo.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

struct record {
  int key;
  char remainder[96];
};

typedef struct args{
    struct record *array;
    int low;
    int high;
} t_arg;

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

void *sort(void *ptr){
    t_arg *curr_arg = ptr;
    struct record *array = curr_arg->array;
    int low = curr_arg->low;
    int high = curr_arg->high;
    quicksort(array, low, high);
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
    struct record *array;
    array = malloc(100*sizeof(struct record));

    inputFile = argv[1];
    outputFile = argv[2];

    int numLines = 0;
    fp=fopen(inputFile,"r");
    if(fp==NULL) {
        printf("File \"%s\" does not exist!\n", inputFile);
    }

    /*
    char buffer[256];
    read(fp, buffer, 100);
    */

    while(fgets(line, 256, fp)){
        struct record new_record;
        char *ptr = line;
        int key = *(int *)line;
        new_record.key = key;
        memcpy(new_record.remainder, ptr+4, 96);
        array[numLines] = new_record;
        numLines+=1;
    }
    fclose(fp);

    int num_proc = get_nprocs();
    int sections = numLines/num_proc;
    int i = 0;
    while(i < numLines){
        // current range: i to i + section
        pthread_t thread_id;
        t_arg curr_arg = {.array = array, .low = i, .high = i + sections};
        pthread_create(&thread_id, NULL, &sort, &curr_arg);
        pthread_join(thread_id, NULL);
        i += sections;
        //quicksort(array, i, i + sections);
    }

    int min_value = INT_MAX;
    struct record min_record;
    int min_counter;

    int pointer_array[num_proc];
    int current = 0;
    for(int k = 0; k < num_proc; k++){
        pointer_array[k] = current;
        current += sections;
    }

    struct record *sorted_array;
    sorted_array = malloc(100*sizeof(struct record));

    int j = 0;
    while(j < numLines){
        int l = 0;
        while(l < num_proc){
            if(array[pointer_array[j]].key < min_value){
                min_value = array[pointer_array[j]].key;
                min_record = array[pointer_array[j]];
                min_counter = j;
            }
            l++;
        }
        sorted_array[j] = min_record;
        pointer_array[j] += sizeof(struct record);
        j++;
    }

    for(int m = 0; m < numLines; m++){
        printf("%d\n", sorted_array[m].key);
    }

    //quicksort(array,0,numLines-1);

    /*
    The input file will consist of records; within each record is a key. The key is the first four bytes of the record. The records are fixed-size, and are each 100 bytes (which includes the key).
    A successful sort will read all the records into memory from the input file, sort them, and then write them out to the output file.
    You also have to force writes to disk by calling fsync() on the output file before finishing.
    You can assume that this is a one-pass sort, i.e., the data can fit into memory. You do not have to implement a multi-pass sort.
    */

  return -1;
}
