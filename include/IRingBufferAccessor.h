#ifndef IRINGBUFFERACCESSOR_H
#define IRINGBUFFERACCESSOR_H


template<class T> class ARingBuffer;

template<class T>
class IRingBufferAccessor
{
    public:
        // TODO should maybe use template method pattern here for interface separation?
        virtual int available() const = 0;

    protected:
    	// TODO Replace with simple ARingBuffer<T> * const data member?
        virtual ARingBuffer<T> *ringBuffer() const = 0;

        virtual void acquireResources(int size) = 0;
        virtual void releaseResources(int size) = 0;
};

#endif // IRINGBUFFERACCESSOR_H