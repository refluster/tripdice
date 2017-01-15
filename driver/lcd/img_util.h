/**
 * @file image.h
 *
 * Copyright(c) 2015 大前良介(OHMAE Ryosuke)
 *
 * This software is released under the MIT License.
 * http://opensource.org/licenses/MIT
 *
 * @brief 画像を扱うための共通ヘッダ
 * @author <a href="mailto:ryo@mm2d.net">大前良介(OHMAE Ryosuke)</a>
 * @date 2015/02/07
 */

#ifndef IMAGE_H_
#define IMAGE_H_
#include <stdio.h>
#include <stdint.h>

#ifdef _DEBUG_
#define ERR(fmt, ...) fprintf(stderr, "\033[31m[%-15.15s:%4u] " fmt "\033[0m\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define DBG(fmt, ...) fprintf(stderr, "\033[33m[%-15.15s:%4u] " fmt "\033[0m\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG(fmt, ...) fprintf(stderr, "[%-15.15s:%4u] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define PRT(fmt, ...) fprintf(stderror, fmt, ##__VA_ARGS__)
#else
#define ERR(fmt, ...)
#define DBG(fmt, ...)
#define LOG(fmt, ...)
#define PRT(fmt, ...)
#endif

#define TRUE  1
#define FALSE 0

/**
 * @brief 成功失敗を表現する
 */
typedef enum result_t {
  SUCCESS = 0, /**< 成功 */
  FAILURE = -1, /**< 失敗 */
} result_t;

enum {
	COLOR_TYPE_RGB = 0,
};

/**
 * @brief 色情報
 *
 * RGBAの色情報を保持する構造体
 */
typedef struct color_t {
  uint8_t r; /**< Red */
  uint8_t g; /**< Green */
  uint8_t b; /**< Blue */
  uint8_t a; /**< Alpha */
} color_t;

/**
 * @brief 画素情報
 *
 * 共用体になっており、
 * RGBA値、グレースケール、カラーインデックス、のいずれかを表現する。
 * 単体ではどの表現形式になっているかを判断することはできない。
 */
typedef union pixcel_t {
  color_t c; /**< RGBA */
  uint8_t g; /**< グレースケール */
  uint8_t i; /**< カラーインデックス */
} pixcel_t;

/**
 * @brief 画像データ保持の構造体
 *
 * 画像データとして保持するために必要最小限の情報を格納する。
 * 各種メタデータの保持については今後の課題。
 *
 * 画素情報については、ポインタのポインタで表現しており
 * 各行へのポインタを保持する配列へのポインタとなっている。
 */
typedef struct image_t {
  uint32_t width;       /**< 幅 */
  uint32_t height;      /**< 高さ */
  uint16_t color_type;  /**< 色表現の種別 */
  pixcel_t **map;       /**< 画像データ */
} image_t;

void dump_image_info(image_t *img);
image_t *allocate_image(uint32_t width, uint32_t height, uint8_t type);
image_t *clone_image(image_t *img);
void free_image(image_t *img);
color_t color_from_rgb(uint8_t r, uint8_t g, uint8_t b);
color_t color_from_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
image_t *image_to_index(image_t *img);
image_t *image_to_gray(image_t *img);
image_t *image_to_rgb(image_t *img);
image_t *image_to_rgba(image_t *img);
image_t *image_index_to_rgb(image_t *img);
image_t *image_rgb_to_index(image_t *img);
image_t *image_gray_to_index(image_t *img);
image_t *image_rgba_to_rgb(image_t *img, color_t bg);
image_t *image_rgba_to_rgb_ignore_alpha(image_t *img);
image_t *image_gray_to_rgb(image_t *img);
image_t *image_rgb_to_gray(image_t *img);
image_t *image_gray_to_binary(image_t *img);

/* PNG形式の読み書き */
image_t *read_png_file(const char *filename);
image_t *read_png_stream(FILE *fp);
result_t write_png_file(const char *filename, image_t *img);
result_t write_png_stream(FILE *fp, image_t *img);

/* JPG形式の読み書き */
image_t *read_jpeg_file(const char *filename);
image_t *read_jpeg_stream(FILE *fp);
result_t write_jpeg_file(const char *filename, image_t *img);
result_t write_jpeg_stream(FILE *fp, image_t *img);

/* BMP形式の読み書き */
image_t *read_bmp_file(const char *filename);
image_t *read_bmp_stream(FILE *fp);
result_t write_bmp_file(const char *filename, image_t *img, int compress);
result_t write_bmp_stream(FILE *fp, image_t *img, int compress);

image_t *read_bmp_simple_file(const char *filename);
image_t *read_bmp_simple_stream(FILE *fp);
result_t write_bmp_simple_file(const char *filename, image_t *img);
result_t write_bmp_simple_stream(FILE *fp, image_t *img);

/* PNM(PPM/PGM/PBM)形式の読み書き */
image_t *read_pnm_file(const char *filename);
image_t *read_pnm_stream(FILE *fp);
result_t write_pnm_file(const char *filename, image_t *img, int type);
result_t write_pnm_stream(FILE *fp, image_t *img, int type);

#endif /* IMAGE_H_ */
