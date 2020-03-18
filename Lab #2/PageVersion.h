#pragma once 

#include <Windows.h>
#include <iostream>

struct LeftBD{
	size_t nextFreeBlock;
};

struct Description{
	size_t next;
    LeftBD* firstFree;
	size_t bsize;
	char state; //0-free, 1-left, 2-right
};

class Allocator {
public:	
	Allocator(const size_t ms, const size_t ps);

    void* mem_realloc(void *addr, size_t size);

    void mem_free(void *addr);

    void mem_dump();

	void* mem_alloc(size_t size);

	
private:
    Description defineCategory(size_t s);
    size_t defineBlockSize(size_t s);
    void initPages();
    size_t* getFreeLBlock(size_t bs);
    size_t getIndex(size_t s);
    size_t createLBlockPage(size_t bs);
    size_t getFreePage();
    size_t* getAbsolutePageAddr(size_t index);
    void setAllFree(Description pd);
    size_t* getFreeMBlock(size_t ps);
    bool freeLBlockIsLast(Description pd);
    size_t findPageByAddress(size_t* addr);
    size_t findBlockByAddress(size_t* addr, size_t bs);
    void copyData(size_t* from, size_t* to, size_t length);
	size_t* begin;
	size_t* pagesBegin;
	size_t size;
	size_t pages;
	size_t pageSize;
    Description* pageDescriptors;
	size_t* lBlocks;
	size_t lBlocksLength;
	size_t firstFreePage;
};
