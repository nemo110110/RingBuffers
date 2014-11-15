#ifndef RINGBUFFERWRITER_H
#define RINGBUFFERWRITER_H


#include "ARingBufferWriter.h"


template<class T> class RingBuffer;
template<class T> class RingBufferReader;

template<class T>
class RingBufferWriter : public ARingBufferWriter<T>
{
    friend class RingBuffer<T>;
    friend class RingBufferReader<T>;

    private:
        RingBufferWriter<T>(RingBuffer<T> *rb);
        int available() const override;

    protected:
        // TODO Replace with simple ARingBuffer<T>* data member?
        inline RingBuffer<T> *ringBuffer() const override { return rb; }

        void acquireResources(int count) override;
        void releaseResources(int count) override;

    private:
        RingBuffer<T> * const rb;
};


#include "RingBuffer.h"
#include "RingBufferReader.h"
#include <stdexcept>
#include <list>


template<class T>
RingBufferWriter<T>::RingBufferWriter(RingBuffer<T> *rb) :
    ARingBufferWriter<T>(), rb(rb)
{ }

template<class T>
int RingBufferWriter<T>::available() const
{
    std::list<ARingBufferReader<T>*> *readers = &rb->readers;
    RingBufferReader<T> *r;

    int min = this->ringBuffer()->capacity;
    int current;
    for (typename std::list<ARingBufferReader<T>*>::const_iterator it = readers->begin(); it != readers->end(); ++it) {
        r = static_cast<RingBufferReader<T>*>(*it);
        current = rb->capacity - r->ahead;
        if (current < min) {
            min = current;
        }
    }
    return min;
}

template<class T>
void RingBufferWriter<T>::acquireResources(int count)
{
    std::list<ARingBufferReader<T>*> *readers = &rb->readers;
    RingBufferReader<T> *r;
    for (typename std::list<ARingBufferReader<T>*>::const_iterator it = readers->begin(); it != readers->end(); ++it) {
        r = static_cast<RingBufferReader<T>*>(*it);
        if (rb->capacity - r->ahead < count) {
            throw std::runtime_error("Insufficient available space in RingBuffer for RingBufferWriter resource acquisition");
        }
        r->ahead += count;
    }
}

template<class T>
void RingBufferWriter<T>::releaseResources(int count)
{ }

#endif // RINGBUFFERWRITER_H
