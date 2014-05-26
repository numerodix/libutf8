# -*- coding: utf-8 -*-

import ctypes


libutf8 = ctypes.cdll.LoadLibrary('./libutf8.so')


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


def main():
    assert 0 != verify('\xff\xfe')
    assert 0 != verify('\xd8\x01')
    assert 0 != verify('\xdf\xff')

    assert 'A' == encode(u'A')
    assert 'ÿ' == encode(u'ÿ')
    assert 'ἀ' == encode(u'ἀ')
    assert '𐅄' == encode(u'𐅄')
    assert 'AA' == encode(u'AA')
    assert 'ɰɵƥ' == encode(u'ɰɵƥ')

    assert u'A' == decode('A')
    assert u'ÿ' == decode('ÿ')
    assert u'ἀ' == decode('ἀ')
    assert u'𐅄' == decode('𐅄')
    assert u'AA' == decode('AA')
    assert u'ɰɵƥ' == decode('ɰɵƥ')


if __name__ == '__main__':
    main()
