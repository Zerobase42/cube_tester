/*
 * bad_apple_player.c
 *
 * 원본 파이썬 코드를 C로 변환한 버전입니다.
 *
 * 필요한 파일 (실행 파일과 같은 폴더에 위치):
 *   - chars.txt       : 밝기 -> 문자 매핑에 쓰이는 문자열 (UTF-8, 개행 1개 포함 가능)
 *   - bad_apple.txt    : 한 줄에 한 프레임씩, base85(zlib(raw grayscale)) 인코딩된 텍스트
 *
 * 컴파일 (zlib 필요):
 *   Linux/macOS : gcc -O2 -o bad_apple_player bad_apple_player.c -lz
 *   Windows(MinGW): gcc -O2 -o bad_apple_player.exe bad_apple_player.c -lz -lkernel32
 *                   (zlib1.dll 또는 정적 libz.a 가 필요합니다)
 *
 * 원본 파이썬 코드의 버그 수정 사항:
 *   - FRAME_MSEC = (60/24)/1000 (초 단위, 24fps 기준으로 잘못됨)
 *     -> 24fps 라면 프레임 간격은 1/24초(약 41.67ms) 여야 합니다.
 *     이 변환본에서는 FRAME_DELAY_MS = 1000.0/24.0 (ms) 로 정정했습니다.
 *
 * 그 외 유의 사항:
 *   - chars.txt 는 UTF-8로 저장되어 있을 수 있고, 각 "문자"가 여러 바이트로
 *     이루어진 유니코드일 수 있습니다(예: 박스 문자 등). 파이썬의
 *     list(CH_LIST) 는 유니코드 코드포인트 단위로 나누므로, 이 C 버전도
 *     바이트 단위가 아니라 UTF-8 코드포인트 단위로 문자를 잘라 배열로
 *     저장한 뒤 인덱싱합니다.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <zlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define WIDTH 120
#define HEIGHT 45
#define FRAME_SIZE (WIDTH * HEIGHT)    /* 프레임 1개의 원본(압축 해제 후) 바이트 수 */
#define FRAME_DELAY_MS (1000.0 / 24.0) /* 24 fps => 약 41.667 ms/frame */

/* ------------------------------------------------------------------ */
/* base85 디코딩 (Python base64.b85decode 와 동일한 알파벳/알고리즘)   */
/* ------------------------------------------------------------------ */

static const unsigned char B85_ALPHABET[] =
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz!#$%&()*+-;<=>?@^_`{|}~";

static int b85_dec[256];

static void init_b85_table(void) {
    for (int i = 0; i < 256; i++) b85_dec[i] = -1;
    for (int i = 0; i < 85; i++) b85_dec[B85_ALPHABET[i]] = i;
}

/*
 * s, len : 입력 base85 문자열과 길이
 * out_len: 성공 시 디코딩된 바이트 수가 채워짐
 * 반환값 : malloc 된 버퍼(성공, out_len 만큼 유효) 또는 NULL(실패)
 */
static unsigned char* b85_decode(const char* s, size_t len, size_t* out_len) {
    size_t padding = (5 - (len % 5)) % 5;
    size_t padded_len = len + padding;

    unsigned char* buf = (unsigned char*)malloc(padded_len ? padded_len : 1);
    if (!buf) return NULL;
    memcpy(buf, s, len);
    memset(buf + len, '~', padding); /* Python 구현과 동일하게 '~'로 패딩 */

    size_t nchunks = padded_len / 5;
    size_t out_size = nchunks * 4;
    unsigned char* out = (unsigned char*)malloc(out_size > 0 ? out_size : 1);
    if (!out) {
        free(buf);
        return NULL;
    }

    for (size_t i = 0; i < nchunks; i++) {
        uint64_t acc = 0;
        for (int j = 0; j < 5; j++) {
            unsigned char c = buf[i * 5 + j];
            int v = b85_dec[c];
            if (v < 0) {
                fprintf(stderr, "bad base85 character at position %zu\n", i * 5 + j);
                free(buf);
                free(out);
                return NULL;
            }
            acc = acc * 85 + (uint64_t)v;
        }
        if (acc > 0xFFFFFFFFULL) {
            fprintf(stderr, "base85 overflow in hunk starting at byte %zu\n", i * 5);
            free(buf);
            free(out);
            return NULL;
        }
        uint32_t val = (uint32_t)acc;
        out[i * 4 + 0] = (unsigned char)(val >> 24);
        out[i * 4 + 1] = (unsigned char)(val >> 16);
        out[i * 4 + 2] = (unsigned char)(val >> 8);
        out[i * 4 + 3] = (unsigned char)(val);
    }

    free(buf);
    *out_len = nchunks * 4 - padding;
    return out;
}

/* ------------------------------------------------------------------ */
/* UTF-8 코드포인트 단위로 chars.txt 파싱                              */
/* ------------------------------------------------------------------ */

typedef struct {
    const char* ptr;
    int len;
} Utf8Char;

static Utf8Char* g_chars = NULL;
static int g_chars_count = 0;
static char* g_chars_buf = NULL; /* g_chars 가 가리키는 원본 버퍼 (소유) */

static int utf8_char_len(unsigned char c) {
    if ((c & 0x80) == 0x00) return 1;
    if ((c & 0xE0) == 0xC0) return 2;
    if ((c & 0xF0) == 0xE0) return 3;
    if ((c & 0xF8) == 0xF0) return 4;
    return 1; /* 잘못된 바이트는 1바이트로 취급(방어적 처리) */
}

/* CH_LIST(UTF-8 문자열)를 코드포인트 배열로 분리 */
static int load_chars(const char* path) {
    FILE* f = fopen(path, "rb");
    if (!f) {
        perror(path);
        return -1;
    }

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (fsize < 0) {
        fclose(f);
        return -1;
    }

    g_chars_buf = (char*)malloc((size_t)fsize + 1);
    if (!g_chars_buf) {
        fclose(f);
        return -1;
    }

    size_t rd = fread(g_chars_buf, 1, (size_t)fsize, f);
    fclose(f);
    g_chars_buf[rd] = '\0';

    /* 파이썬의 f.read().rstrip("\n") 과 동일하게 끝의 개행만 제거 */
    size_t n = rd;
    while (n > 0 && g_chars_buf[n - 1] == '\n') n--;
    if (n > 0 && g_chars_buf[n - 1] == '\r') n--;
    g_chars_buf[n] = '\0';

    /* 코드포인트 개수 세기 */
    size_t cnt = 0;
    for (size_t i = 0; i < n;) {
        int clen = utf8_char_len((unsigned char)g_chars_buf[i]);
        if (i + (size_t)clen > n) clen = (int)(n - i);
        i += (size_t)clen;
        cnt++;
    }

    g_chars = (Utf8Char*)malloc(sizeof(Utf8Char) * (cnt ? cnt : 1));
    if (!g_chars) return -1;

    size_t idx = 0;
    for (size_t i = 0; i < n;) {
        int clen = utf8_char_len((unsigned char)g_chars_buf[i]);
        if (i + (size_t)clen > n) clen = (int)(n - i);
        g_chars[idx].ptr = g_chars_buf + i;
        g_chars[idx].len = clen;
        idx++;
        i += (size_t)clen;
    }
    g_chars_count = (int)idx;
    return 0;
}

/* ------------------------------------------------------------------ */
/* 콘솔 제어 (커서 이동 / 화면 지우기 / sleep) - Windows / POSIX 겸용   */
/* ------------------------------------------------------------------ */

#ifdef _WIN32
static HANDLE g_hOut;

static void console_init(void) {
    system("mode con: cols=120 lines=45");
    g_hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls");
}

static void gotoxy(int x, int y) {
    COORD coord;
    coord.X = (SHORT)x;
    coord.Y = (SHORT)y;
    SetConsoleCursorPosition(g_hOut, coord);
}

static void sleep_ms(double ms) {
    if (ms > 0) Sleep((DWORD)ms);
}

static double now_seconds(void) {
    LARGE_INTEGER freq, cnt;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&cnt);
    return (double)cnt.QuadPart / (double)freq.QuadPart;
}
#else
static void console_init(void) {
    system("clear");
}

static void gotoxy(int x, int y) {
    printf("\x1b[%d;%dH", y + 1, x + 1); /* ANSI escape: 1-based 좌표 */
}

static void sleep_ms(double ms) {
    if (ms > 0) usleep((useconds_t)(ms * 1000));
}

static double now_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}
#endif

/* ------------------------------------------------------------------ */
/* 한 줄씩 읽는 유틸(플랫폼 독립적, POSIX getline 미사용)               */
/* ------------------------------------------------------------------ */

static int read_line(FILE* f, char** buf, size_t* cap) {
    if (*buf == NULL) {
        *cap = 4096;
        *buf = (char*)malloc(*cap);
        if (!*buf) return -1;
    }

    size_t len = 0;
    int c;
    int got_any = 0;

    while ((c = fgetc(f)) != EOF) {
        got_any = 1;
        if (len + 1 >= *cap) {
            *cap *= 2;
            char* nb = (char*)realloc(*buf, *cap);
            if (!nb) return -1;
            *buf = nb;
        }
        (*buf)[len++] = (char)c;
        if (c == '\n') break;
    }

    if (!got_any) return -1; /* EOF, 아무것도 못 읽음 */

    (*buf)[len] = '\0';
    return (int)len;
}

/* ------------------------------------------------------------------ */
/* main                                                                */
/* ------------------------------------------------------------------ */

int main(int argc, char** argv) {
    const char* chars_path = (argc > 1) ? argv[1] : "chars.txt";
    const char* video_path = (argc > 2) ? argv[2] : "bad_apple.txt";

    init_b85_table();

    if (load_chars(chars_path) != 0) {
        fprintf(stderr, "%s 파일을 읽는 데 실패했습니다.\n", chars_path);
        return 1;
    }
    int CHS = g_chars_count - 1;
    if (CHS < 1) {
        fprintf(stderr, "%s 의 문자 개수가 너무 적습니다.\n", chars_path);
        return 1;
    }

    FILE* fvideo = fopen(video_path, "r");
    if (!fvideo) {
        perror(video_path);
        return 1;
    }

    console_init();

    unsigned char frame[FRAME_SIZE];
    char* linebuf = NULL;
    size_t linecap = 0;
    int linelen;

    /* 출력용 버퍼: 한 줄(WIDTH 칸)의 각 칸이 여러 바이트(UTF-8)일 수 있으므로
       넉넉하게 잡는다 (문자 하나당 최대 4바이트 가정) */
    char* outrow = (char*)malloc((size_t)WIDTH * 4 + 1);
    if (!outrow) {
        fprintf(stderr, "out of memory\n");
        return 1;
    }

    double start_time = now_seconds();

    while ((linelen = read_line(fvideo, &linebuf, &linecap)) != -1) {
        /* 끝의 개행/공백 제거 (파이썬의 line.strip() 과 유사) */
        while (linelen > 0 &&
               (linebuf[linelen - 1] == '\n' || linebuf[linelen - 1] == '\r' ||
                linebuf[linelen - 1] == ' ' || linebuf[linelen - 1] == '\t')) {
            linebuf[--linelen] = '\0';
        }
        if (linelen == 0) continue;

        size_t declen = 0;
        unsigned char* compressed = b85_decode(linebuf, (size_t)linelen, &declen);
        if (!compressed) {
            fprintf(stderr, "base85 디코딩 실패, 프레임 건너뜀\n");
            continue;
        }

        uLongf destlen = FRAME_SIZE;
        int zr = uncompress(frame, &destlen, compressed, (uLong)declen);
        free(compressed);
        if (zr != Z_OK || destlen != FRAME_SIZE) {
            fprintf(stderr, "zlib 압축 해제 실패(code=%d), 프레임 건너뜀\n", zr);
            continue;
        }

        gotoxy(0, 0);

        for (int y = 0; y < HEIGHT; y++) {
            int base = y * WIDTH;
            int pos = 0;
            for (int x = 0; x < WIDTH; x++) {
                int idx = (frame[base + x] * CHS) / 255;
                const Utf8Char* ch = &g_chars[idx];
                memcpy(outrow + pos, ch->ptr, (size_t)ch->len);
                pos += ch->len;
            }
            outrow[pos] = '\0';
            fputs(outrow, stdout);
            fputc('\n', stdout);
        }
        fflush(stdout);

        sleep_ms(FRAME_DELAY_MS);
    }

    double end_time = now_seconds();
    double running_time = end_time - start_time;

    free(outrow);
    free(linebuf);
    free(g_chars);
    free(g_chars_buf);
    fclose(fvideo);

    printf("%f\n", running_time);

    return 0;
}