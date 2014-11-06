#ifndef RINGBUFFERREADER_H
#define RINGBUFFERREADER_H


#include "ARingBufferReader.h"


template<class T> RingBuffer;
template<class T> RingBufferWriter;

template<class T>
class RingBufferReader : public ARingBufferReader<T>
{
    friend class RingBuffer<T>;
    friend class RingBufferWriter<T>;

    public:
        RingBufferReader<T>(RingBuffer<T> *ringBuffer);

    protected:
        // TODO Replace with simple ARingBuffer<T>* data member?
        inline RingBuffer<T> *ringBuffer() const override { return rb; }

        void acquireResources(int count) override;
        void releaseResources(int count) override;

    private:
        RingBuffer<T> *rb;
};

template<class T>
RingBufferReader<T>::RingBufferReader(RingBuffer<T> *ringBuffer) :
    rb(ringBuffer)
{ }

template<class T>
void RingBufferReader<T>::acquireResources(int count)
{
    if ((rb->getWriter()->writeIndex - this->readIndex + rb->capacity) % rb->capacity < count) {
        // TODO exeception
        throw std::runtime_error("");
    }
}

template<class T>
void RingBufferReader<T>::releaseResources(int count)
{ }

#endif // RINGBUFFERREADER_H
