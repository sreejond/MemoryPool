#ifndef _MEMORYBLOCK_H_
#define _MEMORYBLOCK_H_

/**
 * \class MemoryBlock
 *
 * \brief This class represent a block of memory for a given type.
 * It contains a block of consequtive memory of given size.
 * And each block can point to it's next memory block.
 */
template <class ElemType>
class MemoryBlock
{
public:
	/** \brief constructor to create a memory block of given size into heap
      * \param size size of a block of memory
      */
	MemoryBlock(const int size);

	/**
      * \brief destructor
      */
	~MemoryBlock();
	
	/** \brief get the memory address of single memory unit for given position
      * \param pos position of a single memory unit into the block
      * \return for valid position return the memory address of the given position, otherwise return nullptr
      */
	ElemType* at(const int pos) const;

	/** \brief set the next memory block after this block
      * \param item next memory block
      */
	void setNext(MemoryBlock* item);

	/** \brief get the next memory block after this block
      * \return next memory block
      */
	MemoryBlock* getNext() const;

private:
	// buffer for the memory block
    ElemType* buffer;
    // point to the next memory block
	MemoryBlock* next;
	// block size
	int blockSize;
};

template <class ElemType>
MemoryBlock<ElemType>::MemoryBlock(const int size)
: buffer(new ElemType[size])
, next(nullptr)
, blockSize(size)
{
}

template <class ElemType>
MemoryBlock<ElemType>::~MemoryBlock()
{
    delete[] buffer;
}

template <class ElemType>
ElemType* MemoryBlock<ElemType>::at(const int pos) const
{
	if (pos < 0 || pos >= blockSize)	return nullptr;
	return (buffer) ? buffer + pos : nullptr;
}

template <class ElemType>
void MemoryBlock<ElemType>::setNext(MemoryBlock* item)
{
	next = item;
}

template <class ElemType>
MemoryBlock<ElemType>* MemoryBlock<ElemType>::getNext() const
{
	return next;
}

#endif // _MEMORYBLOCK_H_
