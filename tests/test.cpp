//===--- test.cpp - tests entry point  ------------------------------------===//
//
// Part of the NekoCdr's "tasknow" project, under the Apache License v2.0
//
//===----------------------------------------------------------------------===//

#include "defines.h"
#include "types/sdto.h"
#include "types/task.h"
#include "functions/serializer.h"

#include <boost/ut.hpp>
#include <cstdlib>
#include <cstring>

using namespace boost::ut;

using tasknow::size16_t;
using tasknow::sizeof16_t;

namespace tn_types = tasknow::types;
namespace tn_functions = tasknow::functions;

int main() {
    "task_serialize"_test = [=] mutable {
        const char* str{"abc"};
        tn_types::task task{str};
        tn_types::sdto sdto{};

        tn_functions::serialize(&task, &sdto);

        size_t offsets[3]{
            0,
            sizeof16_t,
            sizeof16_t * 2,
        };

        expect(sdto.size == 7_i16);

        size16_t data_size;
        memcpy(&data_size, sdto.data + offsets[0], sizeof16_t);
        expect(data_size == 7_i16);

        size16_t title_size;
        memcpy(&title_size, sdto.data + offsets[1], sizeof16_t);
        expect(title_size == 3_i16);

        char* title_data = static_cast<char*>(malloc(title_size * sizeof(char)));
        memcpy(title_data, sdto.data + offsets[2], title_size);
        expect(strcmp(title_data, str) == 0_i);
    };
}
