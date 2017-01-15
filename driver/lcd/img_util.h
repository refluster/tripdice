#ifndef IMAGE_H_
#define IMAGE_H_
#include <stdio.h>
#include <stdint.h>

uint16_t *allocate_image(uint32_t width, uint32_t height);
void free_image(uint16_t *img);

uint16_t *read_jpeg_file(const char *filename);
uint16_t *read_jpeg_stream(FILE *fp);

#endif
