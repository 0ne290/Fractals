#include "ILogger.h"
#include "../Typedefs.h"
#include <string>

namespace Fractals::Core::Interfaces::InnerUtils
{
    SharedString InvertSlash(const char* str)
    {
        SharedString ret = MAKE_SHARED_STRING(str);

        static const std::string backSlash("\\");
        static const std::string forwardSlash("/");
        static const size_t len(1);

        size_t pos(ret->find(backSlash));
        while (pos != std::string::npos)
        {
            ret->replace(pos, len, forwardSlash);
            pos = ret->find(backSlash);
        }

        return ret;
    }
}