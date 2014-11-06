#ifndef ARINGBUFFERREADER_H
#define ARINGBUFFERREADER_H


#include "IRingBufferAccessor.h"

template<class T>
class ARingBufferReader : public IRingBufferAccessor<T>
{
    public:
        ARingBufferReader<T>();

        virtual ~ARingBufferReader<T>() = default;
        ARingBufferReader<T>(const ARingBufferReader& other);
        ARingBufferReader<T>& operator=(const ARingBufferReader& other);

    public:
        int available() const override;

        T dequeue();
        T peek();
        T at(int index) const;
        void read(const T *target, int count);
        void skip(int count);

    protected:
        int readIndex;
};

template<class T>
ARingBufferReader<T>::ARingBufferReader() :
    readIndex(0)
{ }

template<class T>
int ARingBufferReader<T>::available() const
{
    ARingBuffer<T> *rb = ringBuffer();
    return (rb->getWriter()->writeIndex - readIndex + rb->capacity) % rb->capacity;
}

template<class T>
T ARingBufferReader<T>::at(int index) const
{
    ARingBuffer<T> *rb = ringBuffer();
    return rb->buffer[(readIndex + index) % rb->capacity];
}

template<class T>
T ARingBufferReader<T>::dequeue() const
{
    acquireResources(1);

    T* returnValue = &(ringBuffer()->buffer[readIndex]);
    readIndex = (readIndex + 1) % capacity;

    releaseResources(1);

    return *returnValue;
}

template<class T>
T ARingBufferReader<T>::peek() const
{
    return ringBuffer()->buffer[readIndex];
}

template<class T>
void ARingBufferReader<T>::read(const T *target, int count) const
{
    T *buffer = ringBuffer()->buffer;
    int capacity = ringBuffer()->capacity;

    acquireResources(count);

    for (int i = 0; i < count; ++i) {
        target[i] = buffer[(readIndex + i) % capacity];
    }
    readIndex = (readIndex + count) % capacity;

    releaseResources(count);
}

template<class T>
void ARingBufferReader<T>::skip(int count) const
{
    acquireResources(count);
    readIndex += count;
    releaseResources(count);
}

#endif // ARINGBUFFERREADER_H
