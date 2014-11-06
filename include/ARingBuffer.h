#ifndef ARINGBUFFER_H
#define ARINGBUFFER_H


template<class T>
class ARingBuffer
{
    public:
        ARingBuffer<T>(int capacity);
        ARingBuffer<T>(T* buffer, int capacity);

        virtual ~ARingBuffer<T>();
        ARingBuffer<T>(const ARingBuffer& other);
        ARingBuffer<T>& operator=(const ARingBuffer& other);

        virtual ARingBufferWriter<T> * getWriter() const = 0;

    private:
        const bool isBufferOwned;

    protected:
        T* const buffer;

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

template<class T>
int ARingBuffer<T>::getCapacity() const
{
    return capacity;
}

#endif // ARINGBUFFER_H
