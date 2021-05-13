
#include <storage/playlist.h>
#include <unity.h>

void test_function_sort_playlist(void) {
    Playlist playlist;
    playlist.addEntry("/cards/764DF329/02.mp3");    
    playlist.addEntry("/cards/764DF329/03.mp3");    
    playlist.addEntry("/cards/764DF329/04.mp3");    
    playlist.addEntry("/cards/764DF329/06.mp3");    
    playlist.addEntry("/cards/764DF329/07.mp3");    
    playlist.addEntry("/cards/764DF329/08.mp3");    
    playlist.addEntry("/cards/764DF329/09.mp3");    
    playlist.addEntry("/cards/764DF329/05.mp3");    
    playlist.addEntry("/cards/764DF329/01.mp3");    
    TEST_ASSERT_EQUAL(9, playlist.numEntries);

    TEST_MESSAGE("Checking unsorted list");

    TEST_ASSERT_EQUAL_STRING("/cards/764DF329/02.mp3", playlist.entries[0]);
    TEST_ASSERT_EQUAL_STRING("/cards/764DF329/03.mp3", playlist.entries[1]);
    TEST_ASSERT_EQUAL_STRING("/cards/764DF329/04.mp3", playlist.entries[2]);
    TEST_ASSERT_EQUAL_STRING("/cards/764DF329/06.mp3", playlist.entries[3]);
    TEST_ASSERT_EQUAL_STRING("/cards/764DF329/07.mp3", playlist.entries[4]);
    TEST_ASSERT_EQUAL_STRING("/cards/764DF329/08.mp3", playlist.entries[5]);
    TEST_ASSERT_EQUAL_STRING("/cards/764DF329/09.mp3", playlist.entries[6]);
    TEST_ASSERT_EQUAL_STRING("/cards/764DF329/05.mp3", playlist.entries[7]);
    TEST_ASSERT_EQUAL_STRING("/cards/764DF329/01.mp3", playlist.entries[8]);

    TEST_MESSAGE("Checking sorted list");
    playlist.sort();
    TEST_ASSERT_EQUAL_STRING("/cards/764DF329/01.mp3", playlist.entries[0]);
    TEST_ASSERT_EQUAL_STRING("/cards/764DF329/02.mp3", playlist.entries[1]);
    TEST_ASSERT_EQUAL_STRING("/cards/764DF329/03.mp3", playlist.entries[2]);
    TEST_ASSERT_EQUAL_STRING("/cards/764DF329/04.mp3", playlist.entries[3]);
    TEST_ASSERT_EQUAL_STRING("/cards/764DF329/05.mp3", playlist.entries[4]);
    TEST_ASSERT_EQUAL_STRING("/cards/764DF329/06.mp3", playlist.entries[5]);
    TEST_ASSERT_EQUAL_STRING("/cards/764DF329/07.mp3", playlist.entries[6]);
    TEST_ASSERT_EQUAL_STRING("/cards/764DF329/08.mp3", playlist.entries[7]);
    TEST_ASSERT_EQUAL_STRING("/cards/764DF329/09.mp3", playlist.entries[8]);
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