#ifndef TASKNOW_SERIALIZER_H
#define TASKNOW_SERIALIZER_H

#include "types/sdto.h"
#include "types/task.h"

namespace tn_types = tasknow::types;

namespace tasknow::functions {

void serialize(tn_types::task* input, tn_types::sdto* output);

} // end namespace tasknow::functions

#endif //TASKNOW_SERIALIZER_H
