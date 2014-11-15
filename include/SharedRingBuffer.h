#ifndef SHAREDRINGBUFFER_H
#define SHAREDRINGBUFFER_H


#include "ARingBuffer.h"
#include "SharedRingBufferWriter.h"


template<class T> class SharedRingBufferReader;

template<class T>
class SharedRingBuffer : public ARingBuffer<T>
{
    friend class SharedRingBufferWriter<T>;
    friend class SharedRingBufferReader<T>;

    public:
        SharedRingBuffer<T>(int capacity);
        SharedRingBuffer<T>(T* buffer, int capacity);

        SharedRingBufferWriter<T> * getWriter() override;

    private:
        SharedRingBufferWriter<T> writer;
};


template<class T>
SharedRingBuffer<T>::SharedRingBuffer(int capacity) :
    ARingBuffer<T>(capacity), writer(this)
{ }

template<class T>
SharedRingBuffer<T>::SharedRingBuffer(T* buffer, int capacity) :
    ARingBuffer<T>(buffer, capacity), writer(this)
{ }

template<class T>
SharedRingBufferWriter<T> * SharedRingBuffer<T>::getWriter() const
{
    return &writer;
}

#endif // SHAREDRINGBUFFER_H
