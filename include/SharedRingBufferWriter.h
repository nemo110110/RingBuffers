#ifndef SHAREDRINGBUFFERWRITER_H
#define SHAREDRINGBUFFERWRITER_H


#include "ARingBufferWriter.h"
#include <mutex>


template<class T> SharedRingBuffer;
template<class T> SharedRingBufferReader;

template<class T>
class SharedRingBufferWriter : public ARingBufferWriter<T>
{
    friend class SharedRingBuffer<T>;
    friend class SharedRingBufferReader<T>;

    private:
        SharedRingBufferWriter<T>(SharedRingBuffer<T> *rb);

    protected:
        int available() const override;

        void acquireResources(int count) override;
        void releaseResources(int count) override;

    private:
        SharedRingBuffer<T> * const rb;
        std::mutex mutex;
};


#include "SharedRingBuffer.h"
#include "SharedRingBufferReader.h"
#include <list>


template<class T>
SharedRingBufferWriter<T>::SharedRingBufferWriter(SharedRingBuffer<T> *rb) :
    ARingBufferWriter<T>(), rb(rb)
{ }

template<class T>
int SharedRingBufferWriter<T>::available() const
{
    std::list<SharedRingBufferReader<T>*> &readers = static_cast<std::list<SharedRingBufferReader<T>*> >(rb->readers);
    int min = this->ringBuffer()->capacity;
    int current;
    for (std::list<SharedRingBufferReader<T>*>::const_iterator it = readers.begin(); it < readers.end(); ++it) {
        current = (*it)->behind.available();
        if (current < min) {
            min = current;
        }
    }
    return min;
}

template<class T>
void SharedRingBufferWriter<T>::acquireResources(int count)
{
    std::list<SharedRingBufferReader<T>*> &readers = static_cast<std::list<SharedRingBufferReader<T>*> >(rb->readers);
    for (std::list<SharedRingBufferReader<T>*>::const_iterator it = readers.begin(); it < readers.end(); ++it) {
        (*it)->behind.acquire(count);
    }
}

template<class T>
void SharedRingBufferWriter<T>::releaseResources(int count)
{
    std::list<SharedRingBufferReader<T>*> &readers = static_cast<std::list<SharedRingBufferReader<T>*> >(rb->readers);
    for (std::list<SharedRingBufferReader<T>*>::const_iterator it = readers.begin(); it < readers.end(); ++it) {
        (*it)->ahead.release(count);
    }
}

#endif // SHAREDRINGBUFFERWRITER_H
