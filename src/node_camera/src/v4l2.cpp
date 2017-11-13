#include "node_camera/v4l2.h"

v4l2_t *	v4l2_init(char *device, int width, int height, int format)
{
	v4l2_t *v4l2 = NULL;
	int i = 0;

	XXNULL(device, NULL);
	
	v4l2 = (v4l2_t *)xcalloc(1, sizeof(v4l2_t));
	XXNULL(v4l2, NULL);

	v4l2->width = width;
	v4l2->height = height;
	v4l2->frame_size_in = width * height << 1;
	v4l2->format = format;

	v4l2->camfd = open(device, O_RDWR | O_NONBLOCK);
	if (v4l2->camfd < 0) {
		xerror("open DEVICE %s error\n", DEVICE);
		goto err;
	}

	//取得设备的capability，看看设备具有什么功能，比如是否具有视频输入,或者音频输入输出等
	if (-1 == ioctl(v4l2->camfd, VIDIOC_QUERYCAP, &v4l2->cap)) {
		xerror("query camera failed");
		goto err;
	}
	
    if (0 == (v4l2->cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        xerror("video capture not supported./n");
		goto err;
	}

/*
	//获取支持的Image Format
	xmemset(&v4l2->fmt, 0, sizeof(struct v4l2_format));
	v4l2->fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (-1 == ioctl(v4l2->camfd, VIDIOC_G_FMT, &v4l2->fmt)) {
		xerror("get format eeror\n");
		goto err;
	}

	if (V4L2_PIX_FMT_MJPEG & v4l2->fmt.pix.pixelformat)
		v4l2->format = V4L2_PIX_FMT_MJPEG;
	else if (V4L2_PIX_FMT_YUYV & v4l2->fmt.pix.pixelformat)		
		v4l2->format = V4L2_PIX_FMT_YUYV;
	else {
		xerror("v4l2 format error\n");
		goto err;
	}
	*/

	//设置视频的制式和帧格式，制式包括PAL，NTSC，帧的格式个包括宽度和高度等。
	xmemset(&v4l2->fmt, 0, sizeof(struct v4l2_format));
    v4l2->fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2->fmt.fmt.pix.width       = v4l2->width;
    v4l2->fmt.fmt.pix.height      = v4l2->height;
    v4l2->fmt.fmt.pix.field  =	V4L2_FIELD_ANY;
    v4l2->fmt.fmt.pix.pixelformat = v4l2->format;
    if (-1 == ioctl(v4l2->camfd, VIDIOC_S_FMT, &v4l2->fmt)) {
		xerror("unable to set format \n");
		goto err;
    }

	//申请缓存
	xmemset(&v4l2->rb, 0, sizeof(struct v4l2_requestbuffers));
    v4l2->rb.count      = N_BUFFER; /**< 4 buffers */
    v4l2->rb.type       = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2->rb.memory  = V4L2_MEMORY_MMAP;
    if (-1 == ioctl(v4l2->camfd, VIDIOC_REQBUFS, &v4l2->rb)) {
        xerror("unable to allocte buffers\n");
		goto err;
    }

	
    /* map the buffers(4 buffer) */
    for (i = 0; i < N_BUFFER; ++i)
    {
        xmemset(&v4l2->buf, 0, sizeof(struct v4l2_buffer));
        v4l2->buf.index = i;
        v4l2->buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        v4l2->buf.memory = V4L2_MEMORY_MMAP;
        if (-1 == ioctl(v4l2->camfd, VIDIOC_QUERYBUF, &v4l2->buf)) {
            xerror("unable to query buffer");
			goto err;
        }
        /* map it, 0 means anywhere */
        v4l2->mem[i] = mmap(0, v4l2->buf.length, PROT_READ, MAP_SHARED, \
        					v4l2->camfd, v4l2->buf.m.offset);
        /* MAP_FAILED = (void *)-1 */
        if (MAP_FAILED == v4l2->mem[i]) {
            xerror("unable to map buffer");
			goto err;
        }
    }

	//将缓存放到队列中
	for (i = 0; i < N_BUFFER; ++i)
	{
		memset(&v4l2->buf, 0, sizeof(struct v4l2_buffer));
		v4l2->buf.index = i;
		v4l2->buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		v4l2->buf.memory = V4L2_MEMORY_MMAP;
		if (-1 == ioctl(v4l2->camfd, VIDIOC_QBUF, &v4l2->buf))
				xerror("unable to queue the buffers");
	}
	
	switch (v4l2->format)    /* 分配内存 */
    {
    	case V4L2_PIX_FMT_MJPEG:
			{
				
				v4l2->tmp_buffer = (u8 *)xcalloc(1, (size_t)v4l2->frame_size_in);
				if (v4l2->tmp_buffer == NULL) {
					xerror("unable alloc tmp_buffer");
					goto err;
				}
				v4l2->frame_buffer = (u8 *)xcalloc(1, (size_t)v4l2->width * (v4l2->height+8) * 2);
				if (v4l2->frame_buffer == NULL) {
					xerror("unable alloc frame_buffer");
					goto err;
				}
				
			}
		break;

	    case V4L2_PIX_FMT_YUYV:
			{
				v4l2->frame_buffer = (u8 *)xcalloc(1,(size_t)v4l2->frame_size_in);
				if (v4l2->frame_buffer == NULL) {
					xerror("unable alloc frame_buffer");
					goto err;
				}
			}
	    break;
		
	    default:
	        xerror("error!\n");
	        goto err;
		break;
	}

	return v4l2;
err:
	v4l2_destory(v4l2);
	return NULL;
}

int			v4l2_destory(v4l2_t *v4l2)
{	
	u16 i = 0;

	XXNULL(v4l2, -1);
	
    if (v4l2->is_streaming)  /**< stop if it is still capturing */
        v4l2_stopcapture(v4l2);
    if (v4l2->frame_buffer)
        xfree(v4l2->frame_buffer);
    if (v4l2->tmp_buffer)
        xfree(v4l2->tmp_buffer);
	
    v4l2->frame_buffer = NULL;
    v4l2->tmp_buffer = NULL;
	
    /* it is a good thing to  unmap! */
    for (i = 0; i < N_BUFFER; i++)
    {
    	if (!v4l2->mem[i])
			continue;
        if (-1 == munmap(v4l2->mem[i], v4l2->buf.length))
            xerror("munmap");
    }
	
	xmessage("destory video\n");
	
    close(v4l2->camfd);
	
	xfree(v4l2);
	
	return 0;
}

int			v4l2_startcapture(v4l2_t *v4l2)
{
	XXNULL(v4l2, -1);

	v4l2->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	
    if (-1 == ioctl(v4l2->camfd, VIDIOC_STREAMON, &v4l2->type)) {
		xerror("unable to start capture\n");
		return -1;
	}
	
	xmessage("start capture\n");
	
	v4l2->is_streaming = 1;
	
	return 0;	
}
int			v4l2_stopcapture(v4l2_t *v4l2)
{
	XXNULL(v4l2, -1);

	v4l2->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	
    if (-1 == ioctl(v4l2->camfd, VIDIOC_STREAMOFF, &v4l2->type)) {
        xerror("unable to stop capture\n");
		return -1;
    }
	
    v4l2->is_streaming = 0;
	
	xmessage("stop capture\n");
	
	return 0;
}


int		v4l2_collect(v4l2_t *v4l2, unsigned char *out_buf, int *out_size)
{
	
#define HEADFRAME1 0xaf
	int i = 0, buf_size = 0, buf_maxsize = 0;
	
	static int count = 0;
	
	XXNULL(v4l2, -1);	
	XXNULL(out_buf, -1);
	XXNULL(out_size, -1);

	if (!v4l2->is_streaming) /**< if stream is off, start it */
    {
        if (v4l2_startcapture(v4l2))  /*开始捕捉*/
            goto err;
    }
	
    xmemset(&v4l2->buf, 0, sizeof(struct v4l2_buffer));
    v4l2->buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2->buf.memory = V4L2_MEMORY_MMAP;
    /* get data from buffers */
    if (-1 == ioctl(v4l2->camfd, VIDIOC_DQBUF, &v4l2->buf))
    {
        xerror("unable to dequeue buffer\n");
        goto err;
    }
	buf_size = v4l2->buf.bytesused;
	buf_maxsize = v4l2->frame_size_in;
    switch (v4l2->format)
    {
    case V4L2_PIX_FMT_MJPEG:
        if (buf_size <= HEADFRAME1)
        {
            xerror("ignore empty frame.../n");
            return 0;
        }
        /* we can save tmp_buff to a jpg file,just write it! */
        xmemcpy(v4l2->tmp_buffer, v4l2->mem[v4l2->buf.index], buf_size);
		xmemcpy(v4l2->frame_buffer, v4l2->tmp_buffer, buf_size);
        /* here decode MJPEG,so we can dispaly it */
		/*
        if (jpeg_decode(&v4l2->frame_buffer, v4l2->tmp_buffer,
                        &v4l2->width, &v4l2->height) < 0 )
        {
            xerror("decode jpeg error/n");
            goto err;
        }*/
        break;
    case V4L2_PIX_FMT_YUYV:
		buf_size = buf_maxsize > buf_size ? buf_size : buf_maxsize;
        xmemcpy(v4l2->frame_buffer, v4l2->mem[v4l2->buf.index], buf_size);
        break;
    default:
        goto err;
        break;
    }
    /* here you can process the frame! */
    //v4l2_process(v4l2);
    /* queue buffer again */
    if (-1 == ioctl(v4l2->camfd, VIDIOC_QBUF, &v4l2->buf))
    {
        xerror("requeue error\n");
        goto err;
    }
    xmessage("frame:%d/n", count++);
    xmessage("frame size in: %d Byte\n", buf_size);

	xmemcpy(out_buf, v4l2->frame_buffer, buf_size);
	*out_size = buf_size;
    return 0;
err:
    v4l2->is_quit = 0;
    return -1;
}






























