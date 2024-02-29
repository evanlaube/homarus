
#include "blockallocator.h"
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <iostream>

struct Block {
    Block* next;
};

struct Chunk {
    Chunk* next;
    Block* blocks;
    int blockCount;
};

// TODO: Allow blocks of different sizes. For now just use 128 for all
static int blockSize = 128;

// One chunk can store ~128 blocks
static int chunkBlockCount = 128;
static int chunkSize = chunkBlockCount * blockSize;

// Starting allocation for 32 chunks in memory = 524k of RAM
static int chunkSpace = 32;

BlockAllocator::BlockAllocator() {
    chunkCount = 0;
    chunks = (Chunk*) malloc(chunkSpace * sizeof(Chunk));
    memset(chunks, 0, chunkSpace * sizeof(Chunk)); 
}

BlockAllocator::~BlockAllocator() {

    for(int i = 0; i < chunkCount; i++) {
        Block* b = chunks[i].blocks;

        if(b == nullptr)
            continue;
       
        // TODO: Free all the blocks
        
        // TODO: Why does it only compile with ::free vs free
        ::free(chunks);
    }

}

void* BlockAllocator::allocate(int size) {
    std::cout << "Allocating with size of " << size << std::endl;
    if(size == 0) {
        return nullptr;
    }

    Chunk* c = chunks;

    while(c->blockCount >= 128) {
        c += 1;
    }

    // TODO: Expand memoty if chunks have filled up

    if(c->blockCount == 0) {
        // Allocate new chunk
        c->blocks = (Block*) malloc(chunkSize);
        for(int i = 0; i < chunkBlockCount-1; i++) {
            Block* b = (Block*) ((int8_t*)c->blocks + blockSize * i);
            Block* next = (Block*) ((int8_t*)c->blocks + blockSize * (i+1));
            std::cout << "Chunk: " << c << " | Block : " << b << " | Next: " << next << " | Block-Next: " << (int8_t*)next-(int8_t*)b << std::endl;
            b->next = next;
        }
        c->blocks[chunkBlockCount-1].next = nullptr;
    }

    Block* b = c->blocks;
    c->blocks = b->next;
    c->blockCount++;
    b->next = nullptr; // Is this necessary?
    return b;
}
