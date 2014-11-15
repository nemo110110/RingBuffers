#ifndef RINGBUFFER_H
#define RINGBUFFER_H


#include "ARingBuffer.h"
#include "RingBufferWriter.h"
#include "RingBufferReader.h"


template<class T> class RingBufferReader;

template<class T>
class RingBuffer : public ARingBuffer<T>
{
    friend class RingBufferWriter<T>;
    friend class RingBufferReader<T>;

    public:
        RingBuffer<T>(int capacity);
        RingBuffer<T>(T* buffer, int capacity);

        RingBufferWriter<T> * getWriter() override;

    private:
        RingBufferWriter<T> writer;
};


template<class T>
RingBuffer<T>::RingBuffer(int capacity) :
    ARingBuffer<T>(capacity), writer(this)
{ }

template<class T>
RingBuffer<T>::RingBuffer(T* buffer, int capacity) :
    ARingBuffer<T>(buffer, capacity), writer(this)
{ }

template<class T>
RingBufferWriter<T> * RingBuffer<T>::getWriter()
{
    return &writer;
}

#endif // RINGBUFFER_H
