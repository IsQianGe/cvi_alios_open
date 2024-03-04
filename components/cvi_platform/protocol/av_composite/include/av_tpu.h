#include "vfs.h"
#include "fatfs_vfs.h"
#include "cviruntime.h"
#include "cvi_tpu_interface.h"
#include <math.h>

void init_yolo(CVI_MODEL_HANDLE *model, int32_t *input_num, int32_t *output_num, CVI_TENSOR **input_tensors, CVI_TENSOR **output_tensors);
void run_yolo(CVI_MODEL_HANDLE model, int32_t *input_num, int32_t *output_num, CVI_TENSOR **input_tensors, CVI_TENSOR **output_tensors, VIDEO_FRAME_S stVFrame);
void draw_res(VIDEO_FRAME_S stVFrame, int32_t input_num, int32_t output_num, CVI_TENSOR *input_tensors, CVI_TENSOR *output_tensors, uint64_t start_time);
void RGB2YUV(uint8_t *yuv2, uint8_t *bgr, uint32_t width, uint32_t height);