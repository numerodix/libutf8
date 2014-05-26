#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "utf8.h"


char *sprint_chars(const uint32_t *chars, int charcnt)
{
    // [0x10144, 0x41]
    char *out = malloc(charcnt * (2 + 5 + 2));
    int cur = 0;
    int i;
    uint32_t ch;

    out[cur++] = '[';
    for (i = 0; i < charcnt; i++) {
        ch = chars[i];
        cur += sprintf(&(out[cur]), "0x%x", ch);

        if (i < charcnt - 1) {
            out[cur++] = ',';
            out[cur++] = ' ';
        }
    }
    out[cur++] = ']';
    out[cur] = '\0';

    return out;
}

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

    // drop one for the null byte, be conservative and assume
    // every char is one byte
    chars = (uint32_t *) malloc(sizeof(uint32_t) * (buflen - 1));

    charcnt = utf8_decode(buffer, chars);

    char *chars_str = sprint_chars(chars, charcnt);
    char *expec_str = sprint_chars(expected, charcnt);

    if (memcmp(chars, expected, charcnt * sizeof(uint32_t)) != 0) {
        printf("FAILED %s != %s\n", expec_str, chars_str);
    } else {
        printf("PASSED %s == %s\n", expec_str, chars_str);
    }

    free(chars_str);
    free(expec_str);
    free(chars);
}


int main(int argc, char **argv)
{
    // A
    assert_encode((uint32_t[]) {0x41}, 1, "\x41");
    // ÿ
    assert_encode((uint32_t[]) {0xff}, 1, "\xc3\xbf");
    // ἀ
    assert_encode((uint32_t[]) {0x1f00}, 1, "\xe1\xbc\x80");
    // 𐅄
    assert_encode((uint32_t[]) {0x10144}, 1, "\xf0\x90\x85\x84");

    // AA
    assert_encode((uint32_t[]) {0x41, 0x41}, 2, "\x41\x41");
    // ɰɵƥ
    assert_encode((uint32_t[]) {0x270, 0x275, 0x1a5}, 3, "\xc9\xb0\xc9\xb5\xc6\xa5");


    assert_decode("\x41", (uint32_t[]) {0x41});
    assert_decode("\xc3\xbf", (uint32_t[]) {0xff});
    assert_decode("\xe1\xbc\x80", (uint32_t[]) {0x1f00});
    assert_decode("\xf0\x90\x85\x84", (uint32_t[]) {0x10144});

    assert_decode("\x41\x41", (uint32_t[]) {0x41, 0x41});
    assert_decode("\xc9\xb0\xc9\xb5\xc6\xa5", (uint32_t[]) {0x270, 0x275, 0x1a5});

    return 0;
}
