//===--- sdto.h - Serialized Data Trandfer Object -------------------------===//
//
// Part of the NekoCdr's "tasknow" project, under the Apache License v2.0
//
//===----------------------------------------------------------------------===//

#ifndef TASKNOW_SDTO_H
#define TASKNOW_SDTO_H

#include "defines.h"

#include <memory>

namespace tasknow::types {

struct sdto {
    Size16_t size;
    std::unique_ptr<unsigned char[]> data;
};

} // end namespace tasknow::types

#endif //TASKNOW_SDTO_H
