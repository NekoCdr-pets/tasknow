#include "defines.h"
#include "types/sdto.h"
#include "types/task.h"
#include "functions/serializer.h"

#include <boost/ut.hpp>
#include <cstdlib>
#include <cstring>

using namespace boost::ut;
namespace tn_types = tasknow::types;
namespace tn_functions = tasknow::functions;

int main() {
    "task_serialize"_test = [=] mutable {
        const char* str{"abc"};
        tn_types::task task{str};
        tn_types::sdto sdto{};

        tn_functions::serialize(&task, &sdto);

        expect(sdto.size == 5_i16);

        tasknow::d_size_t* title_size = static_cast<tasknow::d_size_t*>(malloc(sizeof(tasknow::d_size_t)));
        memcpy(title_size, sdto.data, sizeof(tasknow::d_size_t));
        expect(*title_size == 3_i16);

        char* title_data = static_cast<char*>(malloc(*title_size * sizeof(char)));
        memcpy(title_data, sdto.data + sizeof(tasknow::d_size_t), *title_size);
        expect(strcmp(title_data, "abc") == 0_i);
    };
}
