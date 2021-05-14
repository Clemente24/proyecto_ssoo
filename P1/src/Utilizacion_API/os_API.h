
typedef struct mbt
{
    char entradas[128][8];
  
} Mbt;

// Functions

Mbt* init_mbt();
int is_partition_valid(Mbt* mbt, int index);

void os_delete_partition(Mbt* mbt, int id);