#ifndef TASKNOW_SDTO_H
#define TASKNOW_SDTO_H

#include <cstdint>

namespace tasknow::types {

struct sdto {
    int16_t size;
    unsigned char* data;
};

} // end namespace tasknow::types

#endif //TASKNOW_SDTO_H
