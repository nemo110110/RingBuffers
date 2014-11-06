#ifndef RINGBUFFERWRITER_H
#define RINGBUFFERWRITER_H


#include "ARingBufferWriter.h"


template<class T> RingBuffer;
template<class T> RingBufferReader;

template<class T>
class RingBufferWriter : public ARingBufferWriter<T>
{
    friend class RingBuffer<T>;
    friend class RingBufferReader<T>;

    protected:
        inline RingBuffer<T> *ringBuffer() const override { return rb; }

        void acquireResources(int count) override;
        void releaseResources(int count) override;
};

template<class T>
void RingBufferWriter<T>::acquireResources(int count) override
{
    RingBuffer<T> *rb = ringBuffer();
    // TODO For each reader, etc
    /*if ((rb->getWriter()->writeIndex - this->readIndex + rb->capacity) % rb->capacity < count) {
        // TODO exeception
        throw std::runtime_error("");
    }*/
}

template<class T>
void RingBufferWriter<T>::releaseResources(int count) override
{ }

#endif // RINGBUFFERWRITER_H
