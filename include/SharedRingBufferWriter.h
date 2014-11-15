#ifndef SHAREDRINGBUFFERWRITER_H
#define SHAREDRINGBUFFERWRITER_H


#include "ARingBufferWriter.h"
#include <mutex>


template<class T> class SharedRingBuffer;
template<class T> class SharedRingBufferReader;

template<class T>
class SharedRingBufferWriter : public ARingBufferWriter<T>
{
    friend class SharedRingBuffer<T>;
    friend class SharedRingBufferReader<T>;

    private:
        SharedRingBufferWriter<T>(SharedRingBuffer<T> *rb);

        int available() const override;

    protected:
        // TODO Replace with simple ARingBuffer<T>* data member?
        inline SharedRingBuffer<T> *ringBuffer() const override { return rb; }

        void acquireResources(int count) override;
        void releaseResources(int count) override;

    private:
        SharedRingBuffer<T> * const rb;

        std::mutex mutex;
};


#include "SharedRingBuffer.h"
#include "SharedRingBufferReader.h"
#include <list>
#include "semaphore.h"


template<class T>
SharedRingBufferWriter<T>::SharedRingBufferWriter(SharedRingBuffer<T> *rb) :
    ARingBufferWriter<T>(), rb(rb)
{ }

template<class T>
int SharedRingBufferWriter<T>::available() const
{
    int min = rb->capacity;
    int current;

    std::list<ARingBufferReader<T>*>* readers = &rb->readers;
    for (typename std::list<ARingBufferReader<T>*>::const_iterator it = readers->begin(); it != readers->end(); ++it) {
        current = static_cast<SharedRingBufferReader<T>*>(*it)->behind.available();
        if (current < min) {
            min = current;
        }
    }
    return min;
}

template<class T>
void SharedRingBufferWriter<T>::acquireResources(int count)
{
    mutex.lock();

    std::list<ARingBufferReader<T>*>* readers = &rb->readers;
    for (typename std::list<ARingBufferReader<T>*>::iterator it = readers->begin(); it != readers->end(); ++it) {
        static_cast<SharedRingBufferReader<T>*>(*it)->behind.wait(count);
    }
}

template<class T>
void SharedRingBufferWriter<T>::releaseResources(int count)
{
    std::list<ARingBufferReader<T>*>* readers = &rb->readers;
    for (typename std::list<ARingBufferReader<T>*>::iterator it = readers->begin(); it != readers->end(); ++it) {
        static_cast<SharedRingBufferReader<T>*>(*it)->ahead.signal(count);
    }

    mutex.unlock();
}

#endif // SHAREDRINGBUFFERWRITER_H
