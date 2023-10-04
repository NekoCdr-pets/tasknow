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

using namespace boost::ut;

using tasknow::Size16_t;
using tasknow::Sizeof16T;

namespace tn_types = tasknow::types;
namespace tn_functions = tasknow::functions;

// NOLINTNEXTLINE
int main() {
    "task_serialize"_test = [=] mutable {
        const char* str{"abc"};
        tn_types::task task{str};
        tn_types::sdto sdto{};

        tn_functions::serialize(&task, &sdto);
    };
}
