#ifndef __MULTI_AV_COMP_H__
#define __MULTI_AV_COMP_H__
#include <stdio.h>

struct video_source {
    int venc_channel;
    int vpss_group;
    int vpss_channel;
};

struct uvc_device_info {
	struct usbd_endpoint video_in_ep;
	struct usbd_interface vc_intf;
	struct usbd_interface vs_intf;
    uint8_t ep;        //assigned in-endpoint
    uint8_t formats;   //number of supported format
	volatile bool streaming_on;
	volatile bool tx_busy;
    volatile bool xfer_flag;
    volatile uint32_t xfer_len;
    volatile uint32_t xfer_offset;
    volatile bool update_flag;
    struct video_source video;
    struct uvc_format_info_st *format_info;
	uint8_t *packet_buffer_uvc;
    bool header_flip;


	// interval max_frame_size max_payload_size
};

#ifndef CONFIG_MULTI_AV_COMP_UVC_NUM
#define CONFIG_MULTI_AV_COMP_UVC_NUM 3
#endif

#endif