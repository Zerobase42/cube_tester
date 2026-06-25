import base64
import zlib
import time
import os
import numpy as np

WIDTH = 120
HEIGHT = 45

def decode_frame(line):
    packed = zlib.decompress(base64.b85decode(line.strip()))

    bits = []

    for byte in packed:
        for shift in range(7, -1, -1):
            bits.append((byte >> shift) & 1)

    return bits[:WIDTH * HEIGHT]

with open("chars.txt", "r", encoding="utf-8") as f:
    CH_LIST = f.read().rstrip("\n")
CHS = len(CH_LIST) - 1
lut = np.array(list(CH_LIST))

with open("bad_apple.txt", "r", encoding="ascii") as f:
    for line in f:
        gray=decode_frame(line)

        indices=(gray.astype(np.uint16) * CHS // 255)

        chars = lut[indices]

        os.system("cls")

        for y in range(HEIGHT):
            start = y * WIDTH
            end = start + WIDTH

            print(''.join(chars[start:end]))

        time.sleep(1 / 15)