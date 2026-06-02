#pragma once
#include<stdlib.h>
#define HEAP_MX 5*1024*1024
char heap[HEAP_MX];
size_t top;

typedef enum _stateEnum{
	FREE = 0,
	ALLOCATED
} stateEnum;

typedef struct _chunkInfo{
	size_t size;
	stateEnum chunkState;
	struct _chunkInfo* nextChunk;
} chunkInfo;

chunkInfo* firstChunk;
chunkInfo* lastChunk;

void* sonchaMalloc(size_t size);
void sonchaFree(void* ptr);
void* sonchaRealloc(void* ptr, size_t size);

void* allocateNewChunk(size_t size){
	size_t chunkSize;
	chunkInfo* newChunkPtr;
	void* arenaStart;

	chunkSize = size+sizeof(chunkInfo);
	if((HEAP_MX-top)<chunkSize){
		printf("sonchaMalloc.h malloc: Not enough space left! Remaining space: %d bytes, Requested space: %d bytes\n", (int)(HEAP_MX-top), (int)size);
		return NULL;
	}
	printf("size: %d   sizeof(chunkInfo): %d  chunkSize: %d\n", (int)size, (int)sizeof(chunkInfo), (int)chunkSize);

	newChunkPtr = (chunkInfo*)(heap+top);
	newChunkPtr->size = size;
	newChunkPtr->chunkState = ALLOCATED;
	newChunkPtr->nextChunk = NULL;

	if(lastChunk == 0 || lastChunk == NULL || top == 0){ // If this is the first allocation of chunk
		firstChunk = newChunkPtr;
	}
	else{
		lastChunk->nextChunk = newChunkPtr;
	}
	lastChunk = newChunkPtr;
	arenaStart = heap+top+sizeof(chunkInfo);
	top += chunkSize;
	return arenaStart;
}

void* sonchaMalloc(size_t size){
	chunkInfo* currentChunk;

	printf("alloc size: %d   top: %d\n", (int)size, (int)top);

	// If this is the first allocation and there are no other chunks
	if(top == 0){
		return allocateNewChunk(size);
	}

	// Search for free chunks of same size
	currentChunk = firstChunk;
	do{
		printf("currentChunk: %p lastChunk: %p\n", currentChunk, lastChunk);
		printf("currentChunk: %p   size: %d nextChunk: %p\n", currentChunk, (int)currentChunk->size, currentChunk->nextChunk);
		if(currentChunk->size == size){
			if(currentChunk->chunkState == FREE){
				printf("match block found! chunkAddr: %p sizeof(chunkInfo): 0x%x   arenaStart: %p\n", currentChunk, (int)sizeof(chunkInfo), (void*)currentChunk + sizeof(chunkInfo));
				currentChunk->chunkState = ALLOCATED;
				return (void*)currentChunk + sizeof(chunkInfo);
			}
		}
		currentChunk = currentChunk->nextChunk;
	} while(currentChunk != 0);

	// If there were no free chunks of the same size
	return allocateNewChunk(size);
}

void sonchaFree(void* ptr){
	chunkInfo* chunkInfoPtr = ptr - sizeof(chunkInfo);
	chunkInfoPtr->chunkState = FREE;
}

void* sonchaRealloc(void* ptr, size_t size){
	void* newPtr;
	newPtr = sonchaMalloc(size);
	memcpy(newPtr, ptr, size);
	sonchaFree(ptr);
	return newPtr;
}
