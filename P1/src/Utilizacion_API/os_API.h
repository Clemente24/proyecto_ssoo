
typedef struct mbt
{
    char entradas[128][8];
  
} Mbt;

typedef struct disk
{
    char* name;
    Mbt* mbt;
    FILE* file_pointer;
  
} Disk;

// Functions

Disk* init_disk();

Mbt* init_mbt();
int is_partition_valid(Mbt* mbt, int index);

int os_delete_partition(Mbt* mbt,int id);