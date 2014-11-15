#ifndef SHAREDRINGBUFFERREADER_H
#define SHAREDRINGBUFFERREADER_H


#include "ARingBufferReader.h"
#include <mutex>
#include "semaphore.h"


template<class T> class SharedRingBuffer;
template<class T> class SharedRingBufferWriter;

template<class T>
class SharedRingBufferReader : public ARingBufferReader<T>
{
    friend class SharedRingBuffer<T>;
    friend class SharedRingBufferWriter<T>;

    public:
        SharedRingBufferReader<T>(SharedRingBuffer<T> *rb);

        int available() const;

    protected:
        // TODO Replace with simple ARingBuffer<T>* data member?
        inline SharedRingBuffer<T> *ringBuffer() const override { return rb; }

        void acquireResources(int count) override;
        void releaseResources(int count) override;

    private:
        SharedRingBuffer<T> *rb;

        std::mutex mutex;
        semaphore ahead;
        semaphore behind;
};


#include "SharedRingBuffer.h"


template<class T>
SharedRingBufferReader<T>::SharedRingBufferReader(SharedRingBuffer<T> *rb) :
    ARingBufferReader<T>(rb), rb(rb),
    ahead(0), behind(rb->capacity)
{ }

template<class T>
int SharedRingBufferReader<T>::available() const
{
    return ahead.available();
}

template<class T>
void SharedRingBufferReader<T>::acquireResources(int count)
{
    mutex.lock();
    ahead.wait(count);
}

template<class T>
void SharedRingBufferReader<T>::releaseResources(int count)
{
    behind.signal(count);
    mutex.unlock();
}

#endif // SHAREDRINGBUFFERREADER_H
