#pragma once

#include <string>
#include <memory>
#include <vector>

using SharedString = std::shared_ptr<std::string>;
#define MAKE_SHARED_STRING std::make_shared<std::string>

template<class T>
using SharedVector = std::shared_ptr<std::vector<T>>;
#define MAKE_SHARED_VECTOR(T) std::make_shared<std::vector<##T>>