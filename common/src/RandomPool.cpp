#include "RandomPool.h"

std::atomic_flag RandomPool::_singletonNotAllocating = ATOMIC_FLAG_INIT;
