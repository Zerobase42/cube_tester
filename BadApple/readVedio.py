import cv2
import numpy as np
import zopfli.zlib
import base64
# 출력 해상도
WIDTH=120
HEIGHT=45
def compress(data:bytes)->str:
    return base64.b85encode(zopfli.zlib.compress(data)).decode("utf-8")
def frame_to_bits(frame):
    # 해상도 조정
    frame=cv2.resize(frame,(WIDTH,HEIGHT),interpolation=cv2.INTER_AREA)
    # 흑백 변환
    gray=cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)
    # 0 또는 255
    _,bw=cv2.threshold(gray,128,255,cv2.THRESH_BINARY)
    # True/False → 비트 압축
    packed=np.packbits(bw>0)
    return packed.tobytes()
def main():
    video_path="BadApple.mp4"
    output_path="bad_apple.txt"
    cap=cv2.VideoCapture(video_path)
    if not cap.isOpened():
        print("Failed to open video file.")
        return
    frame_count=0
    with open(output_path,"w",encoding="ascii")as f:
        while True:
            ret,frame=cap.read()
            if not ret:
                break
            bits=frame_to_bits(frame)
            f.write(compress(bits))
            f.write("\n")
            frame_count+=1
            if frame_count%50==0:
                print(f"{frame_count} frames processed...")
    cap.release()
    print(f"Completed. {frame_count} frames saved.")
if __name__=="__main__":
    main()