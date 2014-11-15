#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <mutex>
#include <condition_variable>


class semaphore
{
    private:
        mutable std::mutex mutex;
        std::condition_variable cv;
        int count;

    public:
        semaphore(int count = 0);

        void signal(int n);
        void wait(int n);
        bool trywait(int n);
        int available() const;
};

#endif // SEMAPHORE_H