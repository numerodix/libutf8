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

int main(int argc, char **argv)
{
    assert_encode((uint32_t[]) {0x41}, 1, "\x41");
    assert_encode((uint32_t[]) {0xff}, 1, "\xc3\xbf");
    assert_encode((uint32_t[]) {0x1f00}, 1, "\xe1\xbc\x80");
    assert_encode((uint32_t[]) {0x10144}, 1, "\xf0\x90\x85\x84");

    assert_encode((uint32_t[]) {0x41, 0x41}, 2, "\x41\x41");

    return 0;
}
