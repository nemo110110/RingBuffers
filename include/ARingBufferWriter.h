#ifndef ARINGBUFFERWRITER_H
#define ARINGBUFFERWRITER_H


#include "IRingBufferAccessor.h"

template<class T> class ARingBuffer;
template<class T> class ARingBufferReader;

template<class T>
class ARingBufferWriter : public IRingBufferAccessor<T>
{
    friend class ARingBuffer<T>;
    friend class ARingBufferReader<T>;

    private:
        ARingBufferWriter<T>(const ARingBufferWriter& other);
        ARingBufferWriter<T>& operator=(const ARingBufferWriter& other);

    protected:
        ARingBufferWriter<T>();
        virtual ~ARingBufferWriter<T>() = default;

    public:
        void enqueue(const T& element);
        void write(const T *elements, int count);
        void skip(int count);

    protected:
        int writeIndex;
};


#include "ARingBuffer.h"
#include "ARingBufferReader.h"
#include <list>


template<class T>
ARingBufferWriter<T>::ARingBufferWriter() :
    writeIndex(0)
{ }

template<class T>
void ARingBufferWriter<T>::enqueue(const T& element)
{
    ARingBuffer<T> *rb = this->ringBuffer();

    this->acquireResources(1);

    rb->buffer[writeIndex] = element;
    writeIndex = (writeIndex + 1) % rb->capacity;

    this->releaseResources(1);
}

template<class T>
void ARingBufferWriter<T>::write(const T *elements, int count)
{
    ARingBuffer<T> *rb = this->ringBuffer();

    this->acquireResources(count);

    for (int i = 0; i < count; ++i) {
        rb->buffer[writeIndex] = elements[i];
        writeIndex = (writeIndex + 1) % rb->capacity;
    }

    this->releaseResources(count);
}

template<class T>
void ARingBufferWriter<T>::skip(int count)
{
    this->acquireResources(count);

    writeIndex = (writeIndex + count) % this->ringBuffer()->capacity;

    this->releaseResources(count);
}

#endif // ARINGBUFFERWRITER_H
