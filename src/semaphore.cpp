#include "semaphore.h"


semaphore::semaphore(int count) :
    count(count)
{ }

void semaphore::signal(int n)
{
    if (n <= 0) {
        return;
    }

    std::unique_lock<std::mutex> lock(mutex);
    count += n;
    cv.notify_all();
}

void semaphore::wait(int n)
{
    if (n <= 0) {
        return;
    }

    std::unique_lock<std::mutex> lock(mutex);

    while (count < n) {
        cv.wait(lock);
    }

    count -= n;
}

bool semaphore::trywait(int n)
{
    if (n <= 0) {
        return true;
    }

    std::unique_lock<std::mutex> lock(mutex);

    if (n <= count) {
        count -= n;
        return true;
    } else {
        return false;
    }
}

int semaphore::available() const
{
    return count;
}