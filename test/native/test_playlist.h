
#include <storage/playlist.h>
#include <unity.h>

void test_function_sort_playlist(void) {
    Playlist playlist;
    playlist.addEntry("/cards/01234567/03 Track 3.mp3");    
    playlist.addEntry("/cards/01234567/01 Track 1.mp3");
    playlist.addEntry("/cards/01234567/02 Track 2.mp3");
    TEST_ASSERT_EQUAL(3, playlist.numEntries);

    TEST_MESSAGE("Checking unsorted list");

    TEST_ASSERT_EQUAL_STRING("/cards/01234567/03 Track 3.mp3", playlist.entries[0]);
    TEST_ASSERT_EQUAL_STRING("/cards/01234567/01 Track 1.mp3", playlist.entries[1]);
    TEST_ASSERT_EQUAL_STRING("/cards/01234567/02 Track 2.mp3", playlist.entries[2]);

    TEST_MESSAGE("Checking sorted list");
    playlist.sort();
    TEST_ASSERT_EQUAL_STRING("/cards/01234567/01 Track 1.mp3", playlist.entries[0]);
    TEST_ASSERT_EQUAL_STRING("/cards/01234567/02 Track 2.mp3", playlist.entries[1]);
    TEST_ASSERT_EQUAL_STRING("/cards/01234567/03 Track 3.mp3", playlist.entries[2]);
}

void test_function_get_next_entry(void) {
    Playlist playlist;
    playlist.addEntry("entry 1");    
    playlist.addEntry("entry 2");
    playlist.addEntry("entry 3");
    
    char *entry = playlist.getNextEntry();
    TEST_ASSERT_EQUAL_STRING("entry 1", entry);

    entry = playlist.getNextEntry();
    TEST_ASSERT_EQUAL_STRING("entry 2", entry);

    entry = playlist.getNextEntry();
    TEST_ASSERT_EQUAL_STRING("entry 3", entry);

    entry = playlist.getNextEntry();
    TEST_ASSERT_NULL(entry);
}