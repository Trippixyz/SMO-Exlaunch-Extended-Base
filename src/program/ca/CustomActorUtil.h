#include <stdio.h>

namespace ca {

    #define LOG_NERVE() mCurrentNerveName = __func__
    #define LOG_NERVE_Impl() const char* mCurrentNerveName
    #define LOG_NERVE_Kill(actor)   void actor::kill() {                \
                                        mCurrentNerveName = nullptr;    \
                                    }
}