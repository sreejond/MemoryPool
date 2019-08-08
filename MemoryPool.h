#ifndef _MEMORYPOOL_H_
#define _MEMORYPOOL_H_

#include <vector>
#include <forward_list>
#include <mutex>

#include "MemoryBlock.h"

/**
 * \class MemoryPool
 *
 * \brief This class ceate block of memories (MemoryBlock) for given type.
 * It maintains a availability list of single memory unit for given element type from these block of memory.
 * On allocation it provide a single memory unit and remove it from the availability list.
 * When user freed that single memory unit it will be available for allocation again.
 * It can increase the memory size on runtime when needed.
 * By default memory block size is 100; i.e. 100 single memory unit for given element type.
 */
template <class ElemType>
class MemoryPool
{
public:
	/**
      * \brief constructor that instantiate a memory block of default size
      */
	MemoryPool();

	/** \brief constructor that instantiate a memory block of given size
      * \param size size of a block of memory
      */
	MemoryPool(const int size);

	/**
      * \brief destructor
      */
	~MemoryPool();

	/** \brief provide a single memory unit of the given element type
      * \return on successful allocation it returns a single unit of memory for ElemType, otherwise return nullptr
      */
	ElemType* alloc();

	/**
      * \brief reserve the given valid pointer to a single memory unit for later use
      */
	void free(ElemType* item);

private:
	/** \brief initialize the MemoryPool with a single MemoryBlock of given size
      *
      * \param size size of a block of memory
      */
	void init (const int size);

	/**
      * \brief populate the availability list for allocation from the MemoryBlock
      */
	void addMemoryBlockIntoFreeList();
	
	// default size of the memory block
	static const int defaultSize = 100;
	// point to the head of the MemoryBlock
	MemoryBlock<ElemType> *memoryBlock;
	// point to the current MemoryBlock that is in used
	MemoryBlock<ElemType> *currentBlock;
	// singly link list contains available single memory unit
	std::forward_list<ElemType*> freeList;
	// mutex for thread synchronization
	std::mutex lock;
	// block size
	int blockSize;
};

template <class ElemType>
MemoryPool<ElemType>::MemoryPool()
: memoryBlock(nullptr)
, currentBlock(nullptr)
{
	init(defaultSize);
}

template <class ElemType>
MemoryPool<ElemType>::MemoryPool(const int size)
: memoryBlock(nullptr)
, currentBlock(nullptr)
{
	init(size);
}

template <class ElemType>
MemoryPool<ElemType>::~MemoryPool()
{
    while (memoryBlock)
    {
        MemoryBlock<ElemType>* next = memoryBlock->getNext();
        delete memoryBlock;
        memoryBlock = next;
    }
}

template <class ElemType>
void MemoryPool<ElemType>::addMemoryBlockIntoFreeList()
{
	if (!currentBlock)	return;

	for (int i = blockSize - 1; i >= 0; i--)
	{
		ElemType* item = currentBlock->at(i);
		if (item)	freeList.push_front(item);
	}
}

template <class ElemType>
void MemoryPool<ElemType>::init(const int size)
{
	blockSize = size;
	memoryBlock = new MemoryBlock<ElemType>(size);
	currentBlock = memoryBlock;
	addMemoryBlockIntoFreeList();
}

template <class ElemType>
ElemType* MemoryPool<ElemType>::alloc()
{
	ElemType* item = nullptr;
	std::lock_guard<std::mutex> locker(lock);

	// if available list is empty create a new memory block and
	// populate available list from there
	if (freeList.empty())
	{
        currentBlock->setNext(new MemoryBlock<ElemType>(blockSize));
        currentBlock = currentBlock->getNext();
        addMemoryBlockIntoFreeList();
	}

	// check again to see if memory block was successfuly created
	if (!freeList.empty())
	{
		item = freeList.front();
		freeList.pop_front();
	}

	return item;
}

template <class ElemType>
void MemoryPool<ElemType>::free(ElemType* item)
{
	if (item == nullptr)	return;
	std::lock_guard<std::mutex> locker(lock);
	freeList.push_front(item);
}

#endif // _MEMORYPOOL_H_