import base64
import zlib
import time
import os
import numpy as np
import ctypes

os.system("mode con: cols=120 lines=45")

FRAME_MSEC=(1/24)/2 # 24 fps
STD_OUTPUT_HANDLE = -11

hOut = ctypes.windll.kernel32.GetStdHandle(STD_OUTPUT_HANDLE)

def gotoxy(x=0, y=0):
    pos = (y << 16) | x
    ctypes.windll.kernel32.SetConsoleCursorPosition(
        hOut,
        pos
    )

WIDTH = 120
HEIGHT = 45

import numpy as np

def decode_frame(line):
    data = zlib.decompress(
        base64.b85decode(line.strip())
    )

    return np.frombuffer(data, dtype=np.uint8)

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
chars_path = os.path.join(BASE_DIR, "chars.txt")

with open(chars_path, "r", encoding="utf-8") as f:
    CH_LIST = f.read().rstrip("\n")
CHS = len(CH_LIST) - 1
lut = np.array(list(CH_LIST))

os.system("cls")
start_time=time.time()

badapple_path = os.path.join(BASE_DIR, "bad_apple.txt")
with open(badapple_path, "r", encoding="ascii") as f:
    for line in f:
        gray=decode_frame(line)

        indices=(gray.astype(np.uint16) * CHS // 255)

        chars = lut[indices]

        gotoxy()

        for y in range(HEIGHT):
            start = y * WIDTH
            end = start + WIDTH

            print(''.join(chars[start:end]))
        time.sleep(FRAME_MSEC)

end_time=time.time()

running_time=end_time-start_time

print(running_time)
# 3:39.12=219.12
