
#ifndef H_BLOCK_ALLOCATOR
#define H_BLOCK_ALLOCATOR

struct Block;
struct Chunk;

/**
 * The BlockAllocator is a memory management scheme that chunks memory into blocks in order
 * to store physics entities in connected blocks of memory. This increases speed, because less time 
 * is spent accessing memory addresses that are farther away from eachother.
 */
class BlockAllocator {
    public:
        BlockAllocator();
        ~BlockAllocator();

        /**
         * Allocates an amount of memory inside of the block allocator. This is similar to the built in
         * malloc() function in c++, but it allocates inside of a chunk. 
         *
         * @param size The size of the block to be allocated (in bytes)
         * @return A pointer to the allocated memory.
         */
        void* allocate(int size);

        /**
         * Frees a block of memory from the allocator, allowing it to be used again by a new 
         * insertion.
         *
         * @param addr The memory pointer that is to be freed.
         * @param size The amount of bytes to be freed.
         */
        void free(void* addr, int size);
    private:
        Chunk* chunks;
        int chunkCount;
};

#endif // !H_BLOCK_ALLOCATOR
