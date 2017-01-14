/**
 * @file jpeg.c
 *
 * Copyright(c) 2015 大前良介(OHMAE Ryosuke)
 *
 * This software is released under the MIT License.
 * http://opensource.org/licenses/MIT
 *
 * @brief JPEGファイルの読み書き処理
 * @author <a href="mailto:ryo@mm2d.net">大前良介(OHMAE Ryosuke)</a>
 * @date 2015/02/07
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <jpeglib.h>
#include "image.h"
#include <setjmp.h>

/**
 * jpeg_error_mgrの拡張。
 */
typedef struct my_error_mgr {
  struct jpeg_error_mgr jerr;
  jmp_buf jmpbuf;
} my_error_mgr;

/**
 * 致命的エラー発生時の処理。
 */
static void error_exit(j_common_ptr cinfo) {
  my_error_mgr *err = (my_error_mgr *) cinfo->err;
  (*cinfo->err->output_message)(cinfo);
  longjmp(err->jmpbuf, 1);
}

/**
 * @brief JPEG形式のファイルを読み込む。
 *
 * @param[in] filename ファイル名
 * @return 読み込んだ画像、読み込みに失敗した場合NULL
 */
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

/**
 * @brief JPEG形式のファイルを読み込む。
 *
 * @param[in] fp ファイルストリーム
 * @return 読み込んだ画像、読み込みに失敗した場合NULL
 */
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
  if ((buffer = calloc(stride, 1)) == NULL) {
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
    }
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
