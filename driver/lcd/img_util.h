#ifndef IMAGE_H_
#define IMAGE_H_
#include <stdio.h>
#include <stdint.h>

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
