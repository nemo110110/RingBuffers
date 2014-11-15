#ifndef ARINGBUFFERREADER_H
#define ARINGBUFFERREADER_H


#include "IRingBufferAccessor.h"

template<class T> class ARingBuffer;

template<class T>
class ARingBufferReader : public IRingBufferAccessor<T>
{
    public:
        ARingBufferReader<T>(ARingBuffer<T> *rb);

        virtual ~ARingBufferReader<T>() = default;
        ARingBufferReader<T>(const ARingBufferReader& other);
        ARingBufferReader<T>& operator=(const ARingBufferReader& other);

    public:
        T peek() const;
        T at(int index) const;

        T dequeue();
        void read(T *target, int count);
        void skip(int count);

    protected:
        int readIndex;
};


#include "ARingBuffer.h"
#include "ARingBufferWriter.h"
#include <list>


template<class T>
ARingBufferReader<T>::ARingBufferReader(ARingBuffer<T> *rb) :
    readIndex(rb->getWriter()->writeIndex)
{
    rb->readers.push_back(this);
}

template<class T>
T ARingBufferReader<T>::peek() const
{
    return this->ringBuffer()->buffer[readIndex];
}

template<class T>
T ARingBufferReader<T>::at(int index) const
{
    ARingBuffer<T> *rb = this->ringBuffer();
    return rb->buffer[(readIndex + index) % rb->capacity];
}

template<class T>
T ARingBufferReader<T>::dequeue()
{
    this->acquireResources(1);

    T* returnValue = &(this->ringBuffer()->buffer[readIndex]);
    readIndex = (readIndex + 1) % this->ringBuffer()->capacity;

    this->releaseResources(1);

    return *returnValue;
}

template<class T>
void ARingBufferReader<T>::read(T *target, int count)
{
    T *buffer = this->ringBuffer()->buffer;
    int capacity = this->ringBuffer()->capacity;

    this->acquireResources(count);

    for (int i = 0; i < count; ++i) {
        target[i] = buffer[(readIndex + i) % capacity];
    }
    readIndex = (readIndex + count) % capacity;

    this->releaseResources(count);
}

template<class T>
void ARingBufferReader<T>::skip(int count)
{
    this->acquireResources(count);

    readIndex += count;

    this->releaseResources(count);
}

#endif // ARINGBUFFERREADER_H
