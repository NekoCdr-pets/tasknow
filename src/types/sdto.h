#ifndef TASKNOW_SDTO_H
#define TASKNOW_SDTO_H

#include "defines.h"

namespace tasknow::types {

struct sdto {
    size16_t size;
    unsigned char* data;
};

} // end namespace tasknow::types

#endif //TASKNOW_SDTO_H
