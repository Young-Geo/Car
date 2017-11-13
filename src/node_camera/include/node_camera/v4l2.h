#ifndef __V4L2_H__
#define __V4L2_H__

#include <getopt.h>           
#include <fcntl.h>            
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>


#include "afx.h"
#include "xlog.h"

#define DEVICE "/dev/video0"
#define WIDTH 640
#define HEIGHT 480
#define FORMAT V4L2_PIX_FMT_MJPEG
//#define FORMAT V4L2_PIX_FMT_YUYV



#define N_BUFFER 4

typedef struct _v4l2_t {
    int camfd;                  /**< camera file descriptor */
    struct v4l2_capability cap;   //功能
    struct v4l2_format fmt;     //格式
    struct v4l2_requestbuffers rb;  //存放读取到的数据缓存
    struct v4l2_buffer buf;   //缓冲区
    enum v4l2_buf_type type;
    void* mem[N_BUFFER];       /**< main buffers */
    u8* tmp_buffer;          /**< for MJPEG */
    u8* frame_buffer;        /**< one frame buffer here */
    u32 frame_size_in;
    u32 format;              /**< eg YUYV or MJPEG,etc. */
    int width;
    int height;
    int is_streaming;           /**< start capture */
    int is_quit;
} v4l2_t;


v4l2_t *	v4l2_init(char *device, int width, int height, int format);

int			v4l2_destory(v4l2_t *v4l2);

int			v4l2_startcapture(v4l2_t *v4l2);

int			v4l2_stopcapture(v4l2_t *v4l2);

int		v4l2_collect(v4l2_t *v4l2, unsigned char *out_buf, int *out_size);

#endif
