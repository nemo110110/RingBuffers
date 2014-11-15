#ifndef RINGBUFFERREADER_H
#define RINGBUFFERREADER_H


#include "ARingBufferReader.h"


template<class T> class RingBuffer;
template<class T> class RingBufferWriter;

template<class T>
class RingBufferReader : public ARingBufferReader<T>
{
    friend class RingBuffer<T>;
    friend class RingBufferWriter<T>;

    public:
        RingBufferReader<T>(RingBuffer<T> *rb);

        int available() const override;

    protected:
        // TODO Replace with simple ARingBuffer<T>* data member?
        inline RingBuffer<T> *ringBuffer() const override { return rb; }

        void acquireResources(int count) override;
        void releaseResources(int count) override;

    private:
        RingBuffer<T> *rb;

        int ahead;
};


#include "RingBuffer.h"
#include "RingBufferWriter.h"
#include <stdexcept>


template<class T>
RingBufferReader<T>::RingBufferReader(RingBuffer<T> *rb) :
    ARingBufferReader<T>(rb), rb(rb),
    ahead(0)
{ }

template<class T>
int RingBufferReader<T>::available() const
{
    return ahead;
}

template<class T>
void RingBufferReader<T>::acquireResources(int count)
{
    if (ahead < count) {
        throw std::runtime_error("Insufficient available elements in RingBuffer for RingBufferReader resource acquisition");
    }
    ahead -= count;
}

template<class T>
void RingBufferReader<T>::releaseResources(int count)
{ }

#endif // RINGBUFFERREADER_H
