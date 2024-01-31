
#ifndef H_BLOCK_ALLOCATOR
#define H_BLOCK_ALLOCATOR

struct Block;
struct Chunk;

class BlockAllocator {
    public:
        BlockAllocator();
        ~BlockAllocator();

        void* allocate(int size);
        void free(void* addr, int size);
    private:
        Chunk* chunks;
        int chunkCount;

       

};

#endif // !H_BLOCK_ALLOCATOR
