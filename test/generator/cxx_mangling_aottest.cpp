#include <stdio.h>

#include "HalideRuntime.h"
#include "HalideBuffer.h"
#include <assert.h>
#include <string.h>

#include "cxx_mangling.h"

using namespace Halide;

int32_t extract_value_global(int32_t *arg) {
    return *arg;
}

namespace HalideTest {

int32_t extract_value_ns(const int32_t *arg) {
    return *arg;
}

}

int main(int argc, char **argv) {
    Image<uint8_t> input(100);

    for (int32_t i = 0; i < 100; i++) {
        input(i) = i;
    }

    Image<double> result(100);

    const halide_filter_metadata_t *m = HalideTest::cxx_mangling_metadata();
    assert(m != NULL);
    printf("Name is: %s\n", m->name);
    assert(strcmp(m->name, "cxx_mangling") == 0);

    int ptr_arg = 42;
    int *int_ptr = &ptr_arg;
    const int *const_int_ptr = &ptr_arg;
    void *void_ptr = nullptr;
    const void *const_void_ptr = nullptr;
    std::string *string_ptr = nullptr;
    const std::string *const_string_ptr = nullptr;

    int r = HalideTest::cxx_mangling(input, -1, 0xff, -1, 0xffff, -1, 0xffffffff,
                                    -1, 0xffffffffffffffffLL, true, 42.0, 4239.0f,
                                    int_ptr, const_int_ptr, void_ptr, const_void_ptr, 
                                    string_ptr, const_string_ptr, result);
    if (r != 0) {
        fprintf(stderr, "Failure!\n");
        exit(1);
    }
    printf("Success!\n");
    return 0;
}
