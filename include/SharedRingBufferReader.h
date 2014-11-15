#ifndef SHAREDRINGBUFFERREADER_H
#define SHAREDRINGBUFFERREADER_H


#include "ARingBufferReader.h"
#include <mutex>


template<class T> SharedRingBuffer;
template<class T> SharedRingBufferWriter;

template<class T>
class SharedRingBufferReader : public ARingBufferReader<T>
{
    friend class SharedRingBuffer<T>;
    friend class SharedRingBufferWriter<T>;

    public:
        SharedRingBufferReader<T>(SharedRingBuffer<T> *ringBuffer);

    protected:
        // TODO Replace with simple ARingBuffer<T>* data member?
        inline SharedRingBuffer<T> *ringBuffer() const override { return rb; }

        void acquireResources(int count) override;
        void releaseResources(int count) override;

    private:
        SharedRingBuffer<T> *rb;
        std::mutex mutex;
        Semaphore toRead;
};


#include "SharedRingBuffer.h"
#include "SharedRingBufferWriter.h"


template<class T>
SharedRingBufferReader<T>::SharedRingBufferReader(SharedRingBuffer<T> *ringBuffer) :
    ARingBufferReader<T>(rb), rb(ringBuffer)
{ }

template<class T>
void SharedRingBufferReader<T>::acquireResources(int count)
{

}

template<class T>
void SharedRingBufferReader<T>::releaseResources(int count)
{

}

#endif // SHAREDRINGBUFFERREADER_H
