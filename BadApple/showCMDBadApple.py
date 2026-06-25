import base64
import zlib
import os

WIDTH = 120
HEIGHT = 45

def decode_frame(line):
    packed = zlib.decompress(
        base64.b85decode(line.strip())
    )

    bits = []

    for byte in packed:
        for shift in range(7, -1, -1):
            bits.append((byte >> shift) & 1)

    return bits[:WIDTH * HEIGHT]

f=open("chars.txt","r",encoding="ascii")
charList=list(f.read())
f.close()

with open("bad_apple.txt", "r", encoding="ascii") as f:
    for line in f:
        frame = decode_frame(line)

        os.system("cls")

        for y in range(HEIGHT):
            start = y * WIDTH
            end = start + WIDTH

            print(
                ''.join(
                    '█' if bit else ' '
                    for bit in frame[start:end]
                )
            )