//===--- sdto.h - Serialized Data Trandfer Object -------------------------===//
//
// Part of the NekoCdr's "tasknow" project, under the Apache License v2.0
//
//===----------------------------------------------------------------------===//

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
