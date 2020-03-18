#include <unistd.h> // sbrk
#include "stdio.h" // printf


int has_initialized = 0;
void *managed_memory_start;
void *last_valid_address;

void malloc_init()
{
    last_valid_address = sbrk(0);

    managed_memory_start = last_valid_address;

    has_initialized = 1;
}

struct mem_control_block {
    int is_available;
    int size;
};

void free(void *firstbyte) {
    struct mem_control_block *mcb;

    mcb = firstbyte - sizeof(struct mem_control_block);

    mcb->is_available = 1;

    return;
}

void *malloc(long numbytes) {

    void *current_location;

    struct mem_control_block *current_location_mcb;

    void *memory_location;

    if(! has_initialized)  {
        malloc_init();
    }

    numbytes = numbytes + sizeof(struct mem_control_block);

    memory_location = 0;

    current_location = managed_memory_start;

    while(current_location != last_valid_address)
    {
        current_location_mcb =
                (struct mem_control_block *)current_location;

        if(current_location_mcb->is_available)
        {
            if(current_location_mcb->size >= numbytes)
            {

                current_location_mcb->is_available = 0;

                memory_location = current_location;

                break;
            }
        }

        current_location = current_location +
                           current_location_mcb->size;
    }


    if(! memory_location)
    {
        sbrk(numbytes);

        memory_location = last_valid_address;

        last_valid_address = last_valid_address + numbytes;

        current_location_mcb = memory_location;
        current_location_mcb->is_available = 0;
        current_location_mcb->size = numbytes;
    }


    memory_location = memory_location + sizeof(struct mem_control_block);

    return memory_location;
}


void *remalloc(void *adr, long numbytes) {
    if (! has_initialized){
        malloc_init();
    }

    if (!adr){
        return malloc(numbytes);
    }

    void *current_location = adr;

    struct mem_control_block *current_location_mcb;

    current_location_mcb = (struct mem_control_block *)current_location;


    if (current_location_mcb->size >= numbytes){
        current_location_mcb->size = numbytes;
        current_location = current_location +
                           current_location_mcb->size;
    }

//    if (current_location_mcb->size < numbytes)
//    {
//        int *new_tmp = malloc(numbytes);
//        for (int i=0; i < current_location_mcb->size; i++){
//            new_tmp[i] =  ((int *)current_location)[0];
//        }
//        free(current_location);
//        for (int i=0; i < current_location_mcb->size; i++){
//            printf("+%d", new_tmp[i]);
//        }
//        new_tmp = (void*)new_tmp;
//
//        return new_tmp;
//    }


    return current_location;

}


int main() {
    int number = 10;
    printf("Allocating memory for a digit:\n");
    int *a = malloc(8);
    a = &number;
    printf("A value = %d\n", *a);
    printf("A index in memory = %d\n", a);

    printf("Allocating memory for an array size of 4:\n");
    int *arr = malloc(4 * sizeof(int));
    if(arr) {
        for(int n=0; n<4; n++) // Заполнение массива
            arr[n] = n*n;
        printf("\n\nArray:\n");
        for(int n=0; n<4; n++) // Вывод его содержимого
            printf("--arr[%d] is %d\n",n,arr[n]);
    }

    printf("\n\nReallocating memory for an array size of 4 to size of 3:\n");
    arr = remalloc(arr, 3 * sizeof(int));
    if (arr) {
        printf("\n\nArray:\n");
        for(int n=0; n<3; n++) // Вывод его содержимого
            printf("--arr[%d] is %d\n",n,arr[n]);
    }

    printf("\n\nReallocating memory for an array size of 4 to size of 10:");
    arr = remalloc(arr, 10 * sizeof(int));
    if (arr) {
        printf("\n\nArray:\n");
        for(int n=0; n<10; n++) // Вывод его содержимого
            printf("--arr[%d] is %d\n",n,arr[n]);
    }
    free(arr);
    return 0;
}
    /*
    printf("Hello, World!\n");
    long *a = custom_malloc(8);
    //*a = 1048238484234424211;

    int *b = custom_malloc(4);
    printf("A value = %ld\n", *a);
    printf("A address is %ld\n", a);
    printf("B address is %ld\n", b);
    custom_free(a);

    a = custom_malloc(16);
    printf("New A address is %ld\n", a);

    int *arr = custom_malloc(4*sizeof(int));
    if(arr) {
        for(int n=0; n<4; n++) // Заполнение массива
            arr[n] = n*n;
        printf("\n\nArray:\n");
        for(int n=0; n<4; n++) // Вывод его содержимого
            printf("--arr[%d] is %d\n",n,arr[n]);
    }
    int new_size = 3;
    // Error time
    arr = custom_realocate(arr, new_size * sizeof(int));
    if(arr) {
        //for(int n=10; n<new_size; n++) // Заполнение массива
          //  arr[n] = n*n;
        printf("\n\nArray:\n");
        for(int n=0; n<new_size; n++) // Вывод его содержимого
            printf("--arr[%d] is %d\n",n,arr[n]);
    }
    return 0;
}
*/