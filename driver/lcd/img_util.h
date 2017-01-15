#ifndef IMAGE_H_
#define IMAGE_H_
#include <stdio.h>
#include <stdint.h>

uint16_t *allocate_image(uint32_t width, uint32_t height, uint8_t type);
void free_image(uint16_t *img);

uint16_t *read_jpeg_file(const char *filename, const int color_type);
uint16_t *read_jpeg_stream(FILE *fp, const int color_type);

#endif
