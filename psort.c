struct record {
  int key;
  char* remainder;
};

int main(int argc, char* argv[]){
  // too little arguments
  if (argc < 2 || argc > 3){
    exit(1);
  }

  /*
  The input file will consist of records; within each record is a key. The key is the first four bytes of the record. The records are fixed-size, and are each 100 bytes (which includes the key).

A successful sort will read all the records into memory from the input file, sort them, and then write them out to the output file.

You also have to force writes to disk by calling fsync() on the output file before finishing.

You can assume that this is a one-pass sort, i.e., the data can fit into memory. You do not have to implement a multi-pass sort.
  */

  return -1;
}