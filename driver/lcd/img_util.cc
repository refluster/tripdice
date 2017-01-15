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

uint16_t *read_jpeg_file(const char *filename) {
	FILE *fp;
	if ((fp = fopen(filename, "rb")) == NULL) {
		perror(filename);
		return NULL;
	}
	uint16_t *img = read_jpeg_stream(fp);
	fclose(fp);
	return img;
}

uint16_t *read_jpeg_stream(FILE *fp) {
	int success = 0;
	uint32_t x, y;
	struct jpeg_decompress_struct jpegd;
	my_error_mgr myerr;
	uint16_t *img = NULL;
	JSAMPROW buffer = NULL;
	JSAMPROW row;
	int stride;
	uint16_t *wp;
	uint8_t (*bmp)[3];
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
	if ((img = allocate_image(jpegd.output_width, jpegd.output_height)) == NULL) {
		goto error;
	}

	// set rgb byte data
	bmp = (uint8_t(*)[3])malloc(jpegd.output_width*jpegd.output_height*sizeof(uint8_t)*3);
	for (y = 0; y < jpegd.output_height; y++) {
		jpeg_read_scanlines(&jpegd, &buffer, 1);
		row = buffer;
		for (x = 0; x < jpegd.output_width; x++) {
			bmp[y*jpegd.output_width + x][0] = *row++;
			bmp[y*jpegd.output_width + x][1] = *row++;
			bmp[y*jpegd.output_width + x][2] = *row++;
		}
	}
	// transposition pixel
	wp = img;
	for (x = 0; x < jpegd.output_width; x++) {
		for (y = 0; y < jpegd.output_height; y++) {
			uint8_t *c = bmp[y*jpegd.output_width + x];
			*wp = (((c[0] >> 3) << 11) | ((c[1] >> 2) << 5) | (c[2] >> 3));
			wp++;
		}
	}
	free(bmp);
	jpeg_finish_decompress(&jpegd);
	success = 1;
error:
	jpeg_destroy_decompress(&jpegd);
	free(buffer);
	if (!success) {
		free_image(img);
		img = NULL;
	}
	return img;
}

uint16_t *allocate_image(uint32_t width, uint32_t height) {
	uint16_t *img;
	if ((img = (uint16_t*)malloc(width*height*sizeof(uint16_t))) == NULL) {
		goto error;
	}
	return img;
error:
	free_image(img);
	return NULL;
}

void free_image(uint16_t *img) {
	if (img == NULL) {
		return;
	}
	free(img);
}
