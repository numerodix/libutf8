#ifndef utf8_h__
#define utf8_h__

#include "stdint.h"


extern int utf8_encode(const uint32_t *chars, int charcnt, char *buffer);
extern int utf8_decode(const char *buffer, uint32_t *chars);

// check that all bytes are valid in utf8
extern int utf8_verify(const char *buffer);

#endif // utf8_h__
