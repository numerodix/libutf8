#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "utf8.h"


void assert_encode(const uint32_t *chars, int charcnt, const char *expected)
{
    // conservatively taking 4 bytes per char + null byte
    char *buffer = malloc(charcnt * 4 + 1);
    int valid = 0;

    utf8_encode(chars, charcnt, buffer);
    valid = utf8_verify(buffer);

    if (strcmp(buffer, expected) != 0) {
        printf("FAILED: %s != %s\n", expected, buffer);
    } else {
        printf("PASSED: %s == %s\n", expected, buffer);
    }

    if (valid != 0) {
        printf("Buffer contains invalid bytes: %s\n", buffer);
    }

    free(buffer);
}

void assert_decode(const char *buffer, const uint32_t *expected)
{
    uint32_t *chars = NULL;
    int buflen;
    int charcnt;

    buflen = strlen(buffer);

    chars = (uint32_t *) malloc((buflen - 1) / 4);

    charcnt = utf8_decode(buffer, chars);

    //if (memcmp(chars, expected, charcnt) != 0) {
    if (chars[0] != expected[0]) {
        printf("FAILED\n");
    } else {
        printf("PASSED\n");
    }

    free(chars);
}


int main(int argc, char **argv)
{
    assert_encode((uint32_t[]) {0x41}, 1, "\x41");
    assert_encode((uint32_t[]) {0xff}, 1, "\xc3\xbf");
    assert_encode((uint32_t[]) {0x1f00}, 1, "\xe1\xbc\x80");
    assert_encode((uint32_t[]) {0x10144}, 1, "\xf0\x90\x85\x84");

    assert_encode((uint32_t[]) {0x41, 0x41}, 2, "\x41\x41");


    assert_decode("\x41", (uint32_t[]) {0x41});
    assert_decode("\xc3\xbf", (uint32_t[]) {0xff});
    assert_decode("\xe1\xbc\x80", (uint32_t[]) {0x1f00});
    assert_decode("\xf0\x90\x85\x84", (uint32_t[]) {0x10144});

    assert_decode("\x41\x41", (uint32_t[]) {0x41, 0x41});

    return 0;
}
