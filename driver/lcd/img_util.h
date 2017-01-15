#ifndef IMAGE_H_
#define IMAGE_H_
#include <stdio.h>
#include <stdint.h>

enum {
	COLOR_TYPE_RGB = 0,
};

typedef struct image_t {
	uint32_t width;
	uint32_t height;
	unsigned int color_type;
	uint16_t *bin;
} image_t;

image_t *allocate_image(uint32_t width, uint32_t height, uint8_t type);
void free_image(image_t *img);

image_t *read_jpeg_file(const char *filename, const int color_type);
image_t *read_jpeg_stream(FILE *fp, const int color_type);

#endif
