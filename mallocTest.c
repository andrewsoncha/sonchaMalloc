#include<stdio.h>
#include<string.h>
#include"sonchaMalloc.h"

void* getChunkFromArena(void* arenaAddr){
	return arenaAddr - sizeof(chunkInfo);
}

void printChunkInfo(chunkInfo* info){
	printf("info addr: %p\n", info);
	if(info == NULL){
		printf("info address is NULL!\n");
		return;
	}
	printf("chunk size: %d\n", (int)info->size);
	printf("chunk State: %s\n", (info->chunkState==FREE)?"FREE":"ALLOCATED");
	printf("chunk nextChunkPtr: %p\n", info->nextChunk);
}

int main(){
	char* allocated_str = sonchaMalloc(10);
	char* allocated_str2 = sonchaMalloc(20);
	if(allocated_str==NULL || allocated_str2==NULL){
		return -1;
	}
	strcpy(allocated_str, "Hello:D");
	strcpy(allocated_str2, "Did this work? Is this long?");
	printf("allocated_str: %s address: %p\n", allocated_str, allocated_str);
	printf("allocated_str2: %s address: %p\n", allocated_str2, allocated_str2);

	chunkInfo* alloc1Info = getChunkFromArena(allocated_str);
	printChunkInfo(alloc1Info);
	printf("\n\n");

	chunkInfo* alloc2Info = getChunkFromArena(allocated_str2);
	printChunkInfo(alloc2Info);
	printf("\n\n");

	printf("freeing allocated_str!\n");
	sonchaFree(allocated_str);
	printf("allocated_str free complete!\n");
	printChunkInfo(alloc1Info);

	char* allocated_new_str = sonchaMalloc(10);
	strcpy(allocated_new_str, "SamePlace");
	printf("allocated_new_str: %s address: %p \n", allocated_new_str, allocated_new_str);
	chunkInfo* allocNewInfo = getChunkFromArena(allocated_new_str);
	printChunkInfo(allocNewInfo);

	printf("\n\nfreeing allocated_str2!\n");
	sonchaFree(allocated_str2);
	printf("allocated_str2 free complete!\n");

	char* realloced_str = sonchaRealloc(allocated_new_str, 35);
	printf("realloced_str: %s \n", realloced_str);
	chunkInfo* reallocInfo = getChunkFromArena(realloced_str);
	printChunkInfo(reallocInfo);
	return 0;
}
