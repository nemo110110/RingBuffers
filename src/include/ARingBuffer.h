#ifndef ARINGBUFFER_H
#define ARINGBUFFER_H


#include <list>

template<class T> class ARingBufferWriter;
template<class T> class ARingBufferReader;

template<class T>
class ARingBuffer
{
    friend class ARingBufferWriter<T>;
    friend class ARingBufferReader<T>;

    public:
        ARingBuffer<T>(int capacity);
        ARingBuffer<T>(T* buffer, int capacity);

        virtual ~ARingBuffer<T>();
        ARingBuffer<T>(const ARingBuffer& other);
        ARingBuffer<T>& operator=(const ARingBuffer& other);

        virtual ARingBufferWriter<T> * getWriter() = 0;

    private:
        const bool isBufferOwned;

    protected:
        T * const buffer;
        std::list<ARingBufferReader<T>*> readers;

    public:
        const int capacity;
};

template<class T>
ARingBuffer<T>::ARingBuffer(int capacity) :
    isBufferOwned(true), buffer(new T[capacity]), capacity(capacity)
{ }

template<class T>
ARingBuffer<T>::ARingBuffer(T* buffer, int capacity) :
    isBufferOwned(false), buffer(buffer), capacity(capacity)
{ }

template<class T>
ARingBuffer<T>::~ARingBuffer()
{
    if (isBufferOwned) {
        delete[] buffer;
    }
}

#endif // ARINGBUFFER_H
