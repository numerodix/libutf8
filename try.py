#!/usr/bin/env python


def tochars(buf):
    buf = [chr(by) for by in buf]
    buf = ''.join(buf)
    return buf

def valid_byte(byte):
    if 0xd800 <= byte <= 0xdfff:
        raise Exception(byte)
    if 0xfffe <= byte <= 0xffff:
        raise Exception(byte)

def encode_char(num):
    buf = []

    # x??? ????
    # 0111 1111
    if num <= 0x7f:
        by0 = num
        buf.append(by0)

    # xxx? ????  xx?? ????

    # 0000 0111  1111 1111
    # 110? ????  10?? ????
    # xxx1 1111  xx11 1111
    elif num <= 0x7ff:
        by0 = 0b11000000 | (num >> 6)
        by1 = 0b10000000 | (num & 0b00111111)
        buf.append(by0)
        buf.append(by1)

    # xxxx ????  xx?? ????  xx?? ????

    # 0000 0000  1111 1111  1111 1111
    # 1110 ????  10?? ????  10?? ????
    # xxxx 1111  xx11 1111  xx11 1111
    elif num <= 0xffff:
        by0 = 0b11100000 | (num >> 12)
        by1 = 0b10000000 | ((num >> 6) & 0b00111111)
        by2 = 0b10000000 | (num & 0b00111111)
        buf.append(by0)
        buf.append(by1)
        buf.append(by2)

    # xxxx x???  xx?? ????  xx?? ????  xx?? ????

    # 0000 0000  0001 1111  1111 1111  1111 1111
    # 1111 0???  10?? ????  10?? ????  10?? ????
    # xxxx x111  xx11 1111  xx11 1111  xx11 1111
    elif num <= 0x1fffff:
        by0 = 0b11110000 | (num >> 18)
        by1 = 0b10000000 | ((num >> 12) & 0b00111111)
        by2 = 0b10000000 | ((num >> 6)  & 0b00111111)
        by3 = 0b10000000 | (num & 0b00111111)
        buf.append(by0)
        buf.append(by1)
        buf.append(by2)
        buf.append(by3)

    [valid_byte(b) for b in buf]

    return buf

def main():
    v1 = tochars(encode_char(0x41))
    v2 = tochars(encode_char(0xff))
    v3 = tochars(encode_char(0x1f00))
    v4 = tochars(encode_char(0x10144))

    assert v1 == '\x41'
    assert v2 == '\xc3\xbf'
    assert v3 == '\xe1\xbc\x80'
    assert v4 == '\xf0\x90\x85\x84'



if __name__ == '__main__':
    main()
