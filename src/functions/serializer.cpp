#include "serializer.h"

#include <cstddef>
#include <cstdlib>
#include <cstring>

namespace tasknow::functions {

void serialize(tn_types::task* input, tn_types::sdto* output)
{
    size_t title_size = input->title.size();

    size_t raw_data_size{title_size + 2};

    unsigned char* buff = static_cast<unsigned char*>(malloc(raw_data_size));

    memcpy(buff, &title_size, sizeof(d_size_t));
    memcpy(buff + sizeof(d_size_t), input->title.data(), title_size);

    output->size = static_cast<d_size_t>(raw_data_size);
    output->data = buff;
}

} // end namespace tasknow::functions
