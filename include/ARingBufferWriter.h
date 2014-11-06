#ifndef ARINGBUFFERWRITER_H
#define ARINGBUFFERWRITER_H


#include "IRingBufferAccessor.h"

template<class T>
class ARingBufferWriter : public IRingBufferAccessor<T>
{
    private:
        ARingBufferWriter<T>(const ARingBufferWriter& other);
        ARingBufferWriter<T>& operator=(const ARingBufferWriter& other);

    protected:
        ARingBufferWriter<T>();
        virtual ~ARingBufferWriter<T>() = default;

    public:
        int available() const override;

        void enqueue(const T& element);
        void write(const T *elements, int count);
        void skip(int count);

    protected:
        int writeIndex;
};

template<class T>
ARingBufferWriter<T>::ARingBufferWriter() :
    writeIndex(0)
{ }

template<class T>
int ARingBufferWriter<T>::available() const
{
    // for each reader... blabla
}

template<class T>
void ARingBufferWriter<T>::enqueue(const T& element) const
{
    acquireResources(1);
    ringBuffer()->buffer[writeIndex] = element;
    writeIndex = (writeIndex + 1) % ringBuffer()->capacity;
    releaseResources(1);
}

template<class T>
void ARingBufferWriter<T>::write(const T *elements, int count) const
{
    acquireResources(count);
    // TODO
    releaseResources(count);
}

template<class T>
void ARingBufferWriter<T>::skip(int count) const
{
    // TODO
}

#endif // ARINGBUFFERWRITER_H
