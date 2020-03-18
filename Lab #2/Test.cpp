#include "PageVersion.h"

using namespace std;

int main(){
    int n  = 100;
    size_t size = 4194304;
    size_t ps = 4096;

    Allocator al(size, ps);
    size_t** addrArray = new size_t*[n];
    for(int i = 0; i < n; i++){
        addrArray[i] = (size_t*)al.mem_alloc(rand());
        if(addrArray[i] == NULL){
            cout << "Error occurred" << endl;
        }
    }
    al.mem_dump();
    for(int i = 0; i < n/3; i++){
        al.mem_realloc(addrArray[i], rand());
    }

    al.mem_dump();
    for(int i = n/2; i < n; i++){
        al.mem_free(addrArray[i]);
    }
    al.mem_dump();
    return 0;
}
