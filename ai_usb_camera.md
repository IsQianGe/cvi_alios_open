# ai usb camera
## 简介
使设备变为带有ai功能的usb摄像头，并将模型识别结果输出到视频中。

## 编译步骤
```shell
make sscma_at_server
```
将生成的`sscma_at_server/generated/images`下的fib.bin及imtb文件拷贝sd卡中，插入设备。
注：sd卡中还需要放置模型文件(yolov5.cvimodel)

## 新建项目需要的配置
```shell
  - cherryusb: develop

CONFIG_APP_AV_COMP_SUPPORT:1
CONFIG_SUPPORT_TPU:1
CONFIG_SUPPORT_USB_DC: 1
CONFIG_USB_DWC2_PORT: 1
CONFIG_APP_AV_COMP_SUPPORT: 1
CONFIG_USB_HS: 1
```
## 执行命令
执行命令前需要先将模型(名字为`yolov5.cvimodel`的模型)放入到sd(fat32)卡中并插到设备。
```shell
(cli-uart)# av_comp app_init
```

## 执行结果
在主机上运行yuv视频播放软件，打开摄像头设备，查看摄像头输出的视频。

## cvimodel模型生成
步骤：
* yolov5模型pytorch版本转换为onnx模型
* onnx模型转换为cvimodel格式

### pt模型转换为onnx
1）下载yolov5官方pt模型
[yolov5s](https://github.com/ultralytics/yolov5/releases/download/v7.0/yolov5s.pt)

2）下载yolov5官方仓库代码

```shell
git clone https://github.com/ultralytics/yolov5.git
```
3）修改后处理代码
换onnx前，需要修改`yolov5/models/yolo.py`文件中`Detect`类中的`forward`函数：
```python
    def forward(self, x):
        """Processes input through YOLOv5 layers, altering shape for detection: `x(bs, 3, ny, nx, 85)`."""
        z = []  # inference output
        for i in range(self.nl):
            x[i] = self.m[i](x[i])  # conv
        return x 
```
后处理部分在设备上实现。

4）pt模型转换为onnx
```shell
python export.py --weights ../yolov5n.pt --img-size 640 640 --include onnx
```

### onnx模型转换为cvimodel
1）配置工具链环境
TPU-MLIR需要在Docker环境开发，从DockerHub https://hub.docker.com/r/sophgo/tpuc_dev 下载所需的镜像：

```shell
docker pull sophgo/tpuc_dev:v3.1
```
确保安装包在当前目录，然后在当前目录创建容器如下：

```shell
docker run --privileged --name sohgo -v $PWD:/workspace -it sophgo/tpuc_dev:v3.1
```

2）加载tpu-mlir工具包
下载[tpu-mlir](https://developer.sophgo.com/thread/473.html)工具包
新建一个文件夹`tpu_mlir`，将新工具链解压到`tpu_mlir/`目录下，并设置环境变量：
```shell
mkdir tpu_mlir & cd tpu_mlir
cp path/to/tpu-mlir_xxx.tar.gz ./
tar zxf tpu-mlir_xxx.tar.gz
source tpu_mlir_v1_2/envsetup.sh
```

3）将模型转换为cvimodel
```shell
model_transform.py \
--model_name yolov5s \
--model_def ../yolov5s.onnx \
--input_shapes [[1,3,640,640]] \
--mean 0.0,0.0,0.0 \
--scale 0.0039216,0.0039216,0.0039216 \
--keep_aspect_ratio \
--pixel_format rgb \
--test_input ../dog.jpg \
--test_result yolov5s_top_outputs.npz \
--mlir yolov5s.mlir
```
其中`model_transform.py`主要参数说明如下表所示，完整的介绍参考**tpu_mlir_xxxxx/doc/TPU-MLIR开发参考手册用户界面章节**

4）MLIR转INT8模型
转 INT8 模型前需要跑 calibration，得到校准表；输入数据的数量根据情况准备 100~1000 张 左右。 然后用校准表，生成cvimodel
生成校对表的图片尽可能和训练数据分布相似
```shell
run_calibration.py yolov5s.mlir \
--dataset ../model_yolov5n_onnx/COCO2017 \
--input_num 100 \
-o yolov5s_cali_table
```

5）生成cvimodel
```shell
model_deploy.py \
--mlir yolov5s.mlir \
--quantize INT8 \
--fuse_preprocess \
--calibration_table yolov5s_cali_table \
--chip cv181x \
--test_input ../dog.jpg \
--test_reference yolov5s_top_outputs.npz \
--tolerance 0.85,0.45 \
--model yolov5.cvimodel

model_deploy.py \
--mlir yolov5s.mlir \
--quantize INT8 \
--fuse_preprocess \
--calibration_table yolov5s_cali_table \
--chip cv180x \
--test_input ../image/dog.jpg \
--test_reference yolov5s_top_outputs.npz \
--tolerance 0.85,0.45 \
--model yolov5s_320_cv180x.cvimodel

```
其中`fuse_preprocess`是指定是否将预处理融合到模型中，如果指定了此参数，则模型输入为uint8类型，直接输入resize后的原图即可
编译完成后，会生成名为`$yolov5.cvimodel$`的文件,将其拷贝到sd卡即可。

### 模型性能结果
| 模型类型          | 模型输入分辨率     | 推理耗时(ms)    | 运行总耗时(ms)  |
|------------------|--------------------|----------------|----------------|
| YOLOV5s          | 320x320            | 25             | 59             |
| YOLOV5s          | 640x640            | 95             | 152            |
| YOLOV5n          | 320x320            | 9              | 42             |
| YOLOV5n          | 640x640            | 35             | 95             |    





