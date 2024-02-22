/*
 * Copyright (C) Cvitek Co., Ltd. 2019-2020. All rights reserved.
 *
 * File Name: custom_viparam.c
 * Description:
 *   ....
 */
#include "custom_param.h"

#define BIN_DATA_SIZE 174281
extern unsigned char cvi_sensor_pq_param[];

PARAM_CLASSDEFINE(PARAM_SNS_CFG_S,SENSORCFG,CTX,Sensor)[] = {
    {
        .enSnsType = CONFIG_SNS0_TYPE,
        .s32I2cAddr = 0x36,
        .s8I2cDev = 2,
    }
};

PARAM_CLASSDEFINE(PARAM_ISP_CFG_S,ISPCFG,CTX,ISP)[] = {
    {
        .bMonoSet = {0},
        .bUseSingleBin = 0,
        .astPQBinDes[0] =
        {
            .pIspBinData = cvi_sensor_pq_param,
            .u32IspBinDataLen = BIN_DATA_SIZE,
            .binID = CVI_BIN_ID_ISP0,
        },
    }
};

PARAM_CLASSDEFINE(PARAM_DEV_CFG_S,VIDEVCFG,CTX,VI)[] = {
    {
        .pViDmaBuf = NULL,
        .u32ViDmaBufSize = 0,
    #if CONFIG_SENSOR_DUAL_SWITCH
        .isMux = true,
        .u8AttachDev = 0,
        .switchGpioIdx = -1,
        .switchGpioPin = -1,
        .switchGPioPol = -1,
    #endif
    },
};

PARAM_VI_CFG_S g_stViCtx = {
    .u32WorkSnsCnt = 1,
    .pstSensorCfg = PARAM_CLASS(SENSORCFG,CTX,Sensor),
    .pstIspCfg = PARAM_CLASS(ISPCFG,CTX,ISP),
    .pstDevInfo = PARAM_CLASS(VIDEVCFG,CTX,VI)
};

PARAM_VI_CFG_S * PARAM_GET_VI_CFG(void) {
    return &g_stViCtx;
}







