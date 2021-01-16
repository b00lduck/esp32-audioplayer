
#include <utils.h>
#include <unity.h>

void test_function_filenameHasExtension(void) {
    TEST_ASSERT_TRUE(filenameHasExtension("/cards/01234567/03 Track 3.mp3", "MP3"));
    TEST_ASSERT_FALSE(filenameHasExtension("/cards/01234567/03 Track 3.mp3", "WAV"));
    TEST_ASSERT_FALSE(filenameHasExtension("/cards/01234567/03 Track 3", "WAV"));
    TEST_ASSERT_TRUE(filenameHasExtension("/cards/01234567/.blah", "blah"));
}
