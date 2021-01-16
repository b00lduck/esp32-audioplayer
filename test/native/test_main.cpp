#include <unity.h>
#include "test_playlist.h"
#include "test_utils.h"

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_function_sort_playlist);
    RUN_TEST(test_function_get_next_entry);
    RUN_TEST(test_function_filenameHasExtension);
    UNITY_END();
    return 0;
}