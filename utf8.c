#include "stdio.h"

#include "utf8.h"


int utf8_encode(const uint32_t *chars, int charcnt, char *buffer)
{
    int i;
    int cnt = 0;
    uint32_t ch;

    for (i = 0; i < charcnt; i++) {
        ch = chars[i];

        if (ch <= 0x7f) {
            buffer[cnt++] = ch;

        } else if (ch <= 0x7ff) {
            buffer[cnt++] = 0xc0 | (ch >> 6);
            buffer[cnt++] = 0x80 | (ch & 0x3f);

        } else if (ch <= 0xffff) {
            buffer[cnt++] = 0xe0 | (ch >> 12);
            buffer[cnt++] = 0x80 | ((ch >> 6) & 0x3f);
            buffer[cnt++] = 0x80 | (ch & 0x3f);

        } else if (ch <= 0x1fffff) {
            buffer[cnt++] = 0xf0 | (ch >> 18);
            buffer[cnt++] = 0x80 | ((ch >> 12) & 0x3f);
            buffer[cnt++] = 0x80 | ((ch >>  6) & 0x3f);
            buffer[cnt++] = 0x80 | (ch & 0x3f);

        }
    }

    buffer[cnt++] = '\0';
 
    return cnt;
}

int utf8_decode(const char *buffer, uint32_t *chars)
{
    int cnt = 0;
    char by = *buffer;

    while (by != '\0') {

        // 4 bytes
        if ((by & 0xf0) == 0xf0) {
            chars[cnt] = (by & 0x7) << 18;
            buffer++;
            by = *buffer;
            chars[cnt] |= (by & 0x3f) << 12;
            buffer++;
            by = *buffer;
            chars[cnt] |= (by & 0x3f) << 6;
            buffer++;
            by = *buffer;
            chars[cnt] |= by & 0x3f;

        // 3 bytes
        } else if ((by & 0xe0) == 0xe0) {
            chars[cnt] = (by & 0xf) << 12;
            buffer++;
            by = *buffer;
            chars[cnt] |= (by & 0x3f) << 6;
            buffer++;
            by = *buffer;
            chars[cnt] |= by & 0x3f;

        // 2 bytes
        } else if ((by & 0xc0) == 0xc0) {
            chars[cnt] = (by & 0x1f) << 6;
            buffer++;
            by = *buffer;
            chars[cnt] |= by & 0x3f;

        // 1 byte
        } else if (by <= 0x7f) {
            chars[cnt] = by;

        }

        cnt++;  // advance to the next character

        buffer++;
        by = *buffer;
    }

    return cnt;
}


int utf8_verify(const char *buffer)
{
    char prev, cur;

    prev = *buffer;
    buffer++;
    cur = *buffer;

    while (cur != '\0') {
        // 0xd800 <= byte <= 0xdfff
        if ((0xd8 <= prev) && (prev <= 0xdf)) {
            return -1;
        }

        // 0xfffe <= byte <= 0xffff
        if ((prev == 0xff) && ((0xfe <= cur) && (cur <= 0xff))) {
            return -1;
        }

        prev = cur;
        buffer++;
        cur = *buffer;
    }

    return 0;
}
