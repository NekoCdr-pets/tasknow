#include "defines.h"
#include "serializer.h"

#include <cstddef>
#include <cstdlib>
#include <cstring>

namespace tasknow::functions {

void serialize(tn_types::task* input, tn_types::sdto* output)
{
    size_t title_size = input->title.size();

    size_t raw_data_size{sizeof16_t + sizeof16_t + title_size};
    size_t offsets[3]{
        0,
        sizeof16_t,
        sizeof16_t * 2,
    };

    unsigned char* buff = static_cast<unsigned char*>(malloc(raw_data_size));

    memcpy(buff + offsets[0], &raw_data_size, sizeof16_t);
    memcpy(buff + offsets[1], &title_size, sizeof16_t);
    memcpy(buff + offsets[2], input->title.data(), title_size);

    output->size = static_cast<size16_t>(raw_data_size);
    output->data = buff;
}

} // end namespace tasknow::functions
