#ifndef _BLOCKING_SYNC_BUFFER_HPP_
#define _BLOCKING_SYNC_BUFFER_HPP_

#include <boost/thread.hpp>

template <class T> class BlockingSyncBuffer {
 public:
    BlockingSyncBuffer<T>() {
        this->data = new T;
    }
    ~BlockingSyncBuffer<T>() {
        this->lock.lock();
        delete this->data;
        this->lock.unlock();
    }

    /**
     * Shallow copies data from the input parameter into the stored buffer.
     */
    void writeData(T in) {
        this->lock.lock();
        memcpy(this->data, *in, sizeof(T));
        this->lock.unlock();
    }

    /**
     * Preforms a shallow copy of the stored data and returns its pointer.
     * You are responsible to destroy it.
     */
    T* readData() {
        this->lock.lock();
        T* retval = new T();
        memcpy(retval, this->data, sizeof(T));
        this->lock.unlock();
        return retval;
    }

    void freeData(void* target) {
        this->lock.lock();
        free(target);
        this->lock.unlock();
    }

    /**
     * Applies a function onto the data.
     */
    void apply(std::function<void(T*)> application) {
        this->lock.lock();
        application(this->data);
        this->lock.unlock();
    }

 protected:
    T* data;
    boost::mutex lock;
};

#endif  // _BLOCKING_SYNC_BUFFER_HPP_
