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

enum {
	COLOR_TYPE_RGB = 0,
};

typedef struct color_t {
	uint8_t r; /**< Red */
	uint8_t g; /**< Green */
	uint8_t b; /**< Blue */
	uint8_t a; /**< Alpha */
} color_t;

typedef union pixcel_t {
	color_t c; /**< RGBA */
	uint8_t g; /**< グレースケール */
	uint8_t i; /**< カラーインデックス */
} pixcel_t;

typedef struct image_t {
	uint32_t width;       /**< 幅 */
	uint32_t height;      /**< 高さ */
	uint16_t color_type;  /**< 色表現の種別 */
	pixcel_t **map;       /**< 画像データ */
} image_t;

image_t *allocate_image(uint32_t width, uint32_t height, uint8_t type);
void free_image(image_t *img);

image_t *read_jpeg_file(const char *filename);
image_t *read_jpeg_stream(FILE *fp);

#endif /* IMAGE_H_ */
