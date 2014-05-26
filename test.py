# -*- coding: utf-8 -*-

import ctypes
import random


libutf8 = ctypes.cdll.LoadLibrary('./libutf8.so')

FAILED = []


def encode(ustring):
    us = ctypes.create_unicode_buffer(ustring)
    s = ctypes.create_string_buffer(len(ustring) * 4 + 1)
    libutf8.utf8_encode(us, len(ustring), ctypes.byref(s))
    return s.value

def decode(bstring):
    s = ctypes.create_string_buffer(bstring)
    us = ctypes.create_unicode_buffer(len(bstring))
    libutf8.utf8_decode(s, ctypes.byref(us))
    return us.value

def verify(bstring):
    s = ctypes.create_string_buffer(bstring)
    return libutf8.utf8_verify(s)


def assert_eq(fst, snd):
    global FAILED
    try:
        assert fst == snd
        print("PASSED %s == %s" % (fst, snd))
    except AssertionError:
        FAILED.append((fst, snd))
        print("FAILED %s != %s" % (fst, snd))

def assert_bad(bstring):
    global FAILED
    try:
        assert 0 != verify(bstring)
        print("PASSED %s judged not valid" % bstring)
    except AssertionError:
        FAILED.append((fst, snd))
        print("FAILED %s judged valid" % bstring)

def assert_roundtrip_us(ustring):
    py_ustring = ustring.encode('utf8').decode('utf8')
    print('')
    assert_eq(ustring, decode(encode(ustring)))
    assert_eq(ustring, py_ustring)


def get_random_unicode():
    us = u''
    cnt = random.randint(1, 10)
    for i in range(cnt):
        ch = random.randint(1, 0xffff)
        us += unichr(ch)

    return us

def assert_random():
    us = get_random_unicode()
    assert_roundtrip_us(us)


def main():
    assert_bad('\xff\xfe')
    assert_bad('\xd8\x01')
    assert_bad('\xdf\xff')

    assert_eq('A', encode(u'A'))
    assert_eq('ÿ', encode(u'ÿ'))
    assert_eq('ἀ', encode(u'ἀ'))
    assert_eq('𐅄', encode(u'𐅄'))
    assert_eq('AA', encode(u'AA'))
    assert_eq('ɰɵƥ', encode(u'ɰɵƥ'))

    assert_eq(u'A', decode('A'))
    assert_eq(u'ÿ', decode('ÿ'))
    assert_eq(u'ἀ', decode('ἀ'))
    assert_eq(u'𐅄', decode('𐅄'))
    assert_eq(u'AA', decode('AA'))
    assert_eq(u'ɰɵƥ', decode('ɰɵƥ'))

    # try some random chars
    for i in xrange(100):
        assert_random()

    # try all the chars
    for i in xrange(1, 0xfffff):
        uch = unichr(i)
        ustring = uch + uch + uch
        assert_roundtrip_us(ustring)

    print('')
    print("Failures: %s" % len(FAILED))
    for (fst, snd) in FAILED:
        print("%s != %s" % (fst, snd))


if __name__ == '__main__':
    main()
