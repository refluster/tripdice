#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <jpeglib.h>
#include "img_util.h"
#include <setjmp.h>

typedef struct my_error_mgr {
	struct jpeg_error_mgr jerr;
	jmp_buf jmpbuf;
} my_error_mgr;

static void error_exit(j_common_ptr cinfo) {
	my_error_mgr *err = (my_error_mgr *) cinfo->err;
	(*cinfo->err->output_message)(cinfo);
	longjmp(err->jmpbuf, 1);
}

image_t *read_jpeg_file(const char *filename) {
	FILE *fp;
	if ((fp = fopen(filename, "rb")) == NULL) {
		perror(filename);
		return NULL;
	}
	image_t *img = read_jpeg_stream(fp);
	fclose(fp);
	return img;
}

image_t *read_jpeg_stream(FILE *fp) {
	result_t result = FAILURE;
	uint32_t x, y;
	struct jpeg_decompress_struct jpegd;
	my_error_mgr myerr;
	image_t *img = NULL;
	JSAMPROW buffer = NULL;
	JSAMPROW row;
	int stride;
	jpegd.err = jpeg_std_error(&myerr.jerr);
	myerr.jerr.error_exit = error_exit;
	if (setjmp(myerr.jmpbuf)) {
		goto error;
	}
	jpeg_create_decompress(&jpegd);
	jpeg_stdio_src(&jpegd, fp);
	if (jpeg_read_header(&jpegd, TRUE) != JPEG_HEADER_OK) {
		goto error;
	}
	jpeg_start_decompress(&jpegd);
	if (jpegd.out_color_space != JCS_RGB) {
		goto error;
	}
	stride = sizeof(JSAMPLE) * jpegd.output_width * jpegd.output_components;
	if ((buffer = (JSAMPROW)calloc(stride, 1)) == NULL) {
		goto error;
	}
	if ((img = allocate_image(jpegd.output_width, jpegd.output_height,
							  COLOR_TYPE_RGB)) == NULL) {
		goto error;
	}
	for (y = 0; y < jpegd.output_height; y++) {
		jpeg_read_scanlines(&jpegd, &buffer, 1);
		row = buffer;
		for (x = 0; x < jpegd.output_width; x++) {
			img->map[y][x].c.r = *row++;
			img->map[y][x].c.g = *row++;
			img->map[y][x].c.b = *row++;
			img->map[y][x].c.a = 0xff;
			printf("(%3d,%3d,%3d)\n", 
				   img->map[y][x].c.r,
				   img->map[y][x].c.g,
				   img->map[y][x].c.b);
		}
		break;
	}
	jpeg_finish_decompress(&jpegd);
	result = SUCCESS;
error:
	jpeg_destroy_decompress(&jpegd);
	free(buffer);
	if (result != SUCCESS) {
		free_image(img);
		img = NULL;
	}
	return img;
}

image_t *allocate_image(uint32_t width, uint32_t height, uint8_t type) {
	uint32_t i;
	image_t *img;
	if ((img = (image_t*)calloc(1, sizeof(image_t))) == NULL) {
		return NULL;
	}
	img->width = width;
	img->height = height;
	img->color_type = type;
	if ((img->map = (pixcel_t**)calloc(height, sizeof(pixcel_t*))) == NULL) {
		goto error;
	}
	for (i = 0; i < height; i++) {
		if ((img->map[i] = (pixcel_t*)calloc(width, sizeof(pixcel_t))) == NULL) {
			goto error;
		}
	}
	return img;
error:
	free_image(img);
	return NULL;
}

void free_image(image_t *img) {
	uint32_t i;
	if (img == NULL) {
		return;
	}
	for (i = 0; i < img->height; i++) {
		free(img->map[i]);
	}
	free(img->map);
	free(img);
}

int main() {
	read_jpeg_file("test.jpg");
}
