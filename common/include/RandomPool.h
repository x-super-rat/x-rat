#ifndef RANDOMPOOL_H
#define RANDOMPOOL_H

#include <atomic>
#include <memory>

class RandomPool {
public:
    void fill(void *buffer, unsigned size);

    template <typename bufT>
    void fill(bufT *buffer)
    {
        fill(buffer, sizeof(bufT));
    }

    static std::shared_ptr<RandomPool> instance()
    {
        allocate();
        return _singletonRandomPool;
    }

private:
    explicit RandomPool() {};

    static void allocate()
    {
        if (!_singletonSet.load()) {
            if (!_singletonNotAllocating.test_and_set()) { // We acquired the lock
                _singletonRandomPool = std::shared_ptr<RandomPool>(new RandomPool);
                _singletonNotAllocating.clear();
                _singletonSet.store(true);
            } else {
                while (!_singletonSet.load()) {
                } // Spin lock
            }
        }
    }

    static std::shared_ptr<RandomPool> _singletonRandomPool;
    static std::atomic_flag _singletonNotAllocating;
    static std::atomic_bool _singletonSet;
};

#endif // RANDOMPOOL_H
