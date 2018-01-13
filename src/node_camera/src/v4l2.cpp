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

	xmemset(&v4l2->fmt, 0, sizeof(struct v4l2_format));
	v4l2->fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (-1 == ioctl(v4l2->camfd, VIDIOC_G_FMT, &v4l2->fmt)) {
		xerror("get format eeror\n");
		goto err;
	}

	xmessage("fmt.type:\t\t%d\n",v4l2->fmt.type);   
 	xmessage("pix.pixelformat:\t%c%c%c%c\n", v4l2->fmt.fmt.pix.pixelformat & 0xFF, (v4l2->fmt.fmt.pix.pixelformat >> 8) & 0xFF, (v4l2->fmt.fmt.pix.pixelformat >> 16) & 0xFF, (v4l2->fmt.fmt.pix.pixelformat >> 24) & 0xFF);  
	xmessage("pix.width:\t\t%d\n",v4l2->fmt.fmt.pix.width);  
	xmessage("pix.height:\t\t%d\n",v4l2->fmt.fmt.pix.height);  

	v4l2->width = v4l2->fmt.fmt.pix.width;
	v4l2->height = v4l2->fmt.fmt.pix.height;

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







#define OUTPUT_BUF_SIZE  4096

typedef struct {
    struct jpeg_destination_mgr pub; /* public fields */

    JOCTET * buffer;    /* start of buffer */

    unsigned char *outbuffer;
    int outbuffer_size;
    unsigned char *outbuffer_cursor;
    int *written;

} mjpg_destination_mgr;

typedef mjpg_destination_mgr * mjpg_dest_ptr;

/******************************************************************************
Description.:
Input Value.:
Return Value:
******************************************************************************/
METHODDEF(void) init_destination(j_compress_ptr cinfo)
{
    mjpg_dest_ptr dest = (mjpg_dest_ptr) cinfo->dest;

    /* Allocate the output buffer --- it will be released when done with image */
    dest->buffer = (JOCTET *)(*cinfo->mem->alloc_small)((j_common_ptr) cinfo, JPOOL_IMAGE, OUTPUT_BUF_SIZE * sizeof(JOCTET));

    *(dest->written) = 0;

    dest->pub.next_output_byte = dest->buffer;
    dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;
}

/******************************************************************************
Description.: called whenever local jpeg buffer fills up
Input Value.:
Return Value:
******************************************************************************/
METHODDEF(boolean) empty_output_buffer(j_compress_ptr cinfo)
{
    mjpg_dest_ptr dest = (mjpg_dest_ptr) cinfo->dest;

    memcpy(dest->outbuffer_cursor, dest->buffer, OUTPUT_BUF_SIZE);
    dest->outbuffer_cursor += OUTPUT_BUF_SIZE;
    *(dest->written) += OUTPUT_BUF_SIZE;

    dest->pub.next_output_byte = dest->buffer;
    dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;

    return TRUE;
}

/******************************************************************************
Description.: called by jpeg_finish_compress after all data has been written.
              Usually needs to flush buffer.
Input Value.:
Return Value:
******************************************************************************/
METHODDEF(void) term_destination(j_compress_ptr cinfo)
{
    mjpg_dest_ptr dest = (mjpg_dest_ptr) cinfo->dest;
    size_t datacount = OUTPUT_BUF_SIZE - dest->pub.free_in_buffer;

    /* Write any data remaining in the buffer */
    memcpy(dest->outbuffer_cursor, dest->buffer, datacount);
    dest->outbuffer_cursor += datacount;
    *(dest->written) += datacount;
}

/******************************************************************************
Description.: Prepare for output to a stdio stream.
Input Value.: buffer is the already allocated buffer memory that will hold
              the compressed picture. "size" is the size in bytes.
Return Value: -
******************************************************************************/
GLOBAL(void) dest_buffer(j_compress_ptr cinfo, unsigned char *buffer, int size, int *written)
{
    mjpg_dest_ptr dest;

    if(cinfo->dest == NULL) {
        cinfo->dest = (struct jpeg_destination_mgr *)(*cinfo->mem->alloc_small)((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof(mjpg_destination_mgr));
    }

    dest = (mjpg_dest_ptr) cinfo->dest;
    dest->pub.init_destination = init_destination;
    dest->pub.empty_output_buffer = empty_output_buffer;
    dest->pub.term_destination = term_destination;
    dest->outbuffer = buffer;
    dest->outbuffer_size = size;
    dest->outbuffer_cursor = buffer;
    dest->written = written;
}

void yuyv_to_rgb(int width, int height, unsigned char* yuv,unsigned char* rgb)  
{  
    unsigned int i;  
    unsigned char* y0 = yuv + 0;     
    unsigned char* u0 = yuv + 1;  
    unsigned char* y1 = yuv + 2;  
    unsigned char* v0 = yuv + 3;  
  
    unsigned  char* r0 = rgb + 0;  
    unsigned  char* g0 = rgb + 1;  
    unsigned  char* b0 = rgb + 2;  
    unsigned  char* r1 = rgb + 3;  
    unsigned  char* g1 = rgb + 4;  
    unsigned  char* b1 = rgb + 5;  
     
    float rt0 = 0, gt0 = 0, bt0 = 0, rt1 = 0, gt1 = 0, bt1 = 0;  
  
    for(i = 0; i <= (width * height) / 2 ;i++)  
    {  
        bt0 = 1.164 * (*y0 - 16) + 2.018 * (*u0 - 128);   
        gt0 = 1.164 * (*y0 - 16) - 0.813 * (*v0 - 128) - 0.394 * (*u0 - 128);   
        rt0 = 1.164 * (*y0 - 16) + 1.596 * (*v0 - 128);   
     
        bt1 = 1.164 * (*y1 - 16) + 2.018 * (*u0 - 128);   
        gt1 = 1.164 * (*y1 - 16) - 0.813 * (*v0 - 128) - 0.394 * (*u0 - 128);   
        rt1 = 1.164 * (*y1 - 16) + 1.596 * (*v0 - 128);   
      
        
                if(rt0 > 250)    rt0 = 255;  
        if(rt0< 0)       rt0 = 0;      
  
        if(gt0 > 250)    gt0 = 255;  
        if(gt0 < 0)  gt0 = 0;      
  
        if(bt0 > 250)    bt0 = 255;  
        if(bt0 < 0)  bt0 = 0;      
  
        if(rt1 > 250)    rt1 = 255;  
        if(rt1 < 0)  rt1 = 0;      
  
        if(gt1 > 250)    gt1 = 255;  
        if(gt1 < 0)  gt1 = 0;      
  
        if(bt1 > 250)    bt1 = 255;  
        if(bt1 < 0)  bt1 = 0;      
                      
        *r0 = (unsigned char)rt0;  
        *g0 = (unsigned char)gt0;  
        *b0 = (unsigned char)bt0;  
      
        *r1 = (unsigned char)rt1;  
        *g1 = (unsigned char)gt1;  
        *b1 = (unsigned char)bt1;  
  
        yuv = yuv + 4;  
        rgb = rgb + 6;  
        if(yuv == NULL)  
          break;  
  
        y0 = yuv;  
        u0 = yuv + 1;  
        y1 = yuv + 2;  
        v0 = yuv + 3;  
    
        r0 = rgb + 0;  
        g0 = rgb + 1;  
        b0 = rgb + 2;  
        r1 = rgb + 3;  
        g1 = rgb + 4;  
        b1 = rgb + 5;  
    }     
}

/*
for(x = 0; x < v4l2->width; x++) {
	int r, g, b;
	int y, u, v;
	int y0, y1;

	if(!z)
		y = yuyv[0] << 8;
	else
		y = yuyv[2] << 8;
	u = yuyv[1] - 128;
	v = yuyv[3] - 128;

	r = (y + (359 * v)) >> 8;
	g = (y - (88 * u) - (183 * v)) >> 8;
	b = (y + (454 * u)) >> 8;

	*(ptr++) = (r > 255) ? 255 : ((r < 0) ? 0 : r);
	*(ptr++) = (g > 255) ? 255 : ((g < 0) ? 0 : g);
	*(ptr++) = (b > 255) ? 255 : ((b < 0) ? 0 : b);

	if(z++) {
		z = 0;
		yuyv += 4;
	}
}

*/


int compress_image_to_jpeg(v4l2_t *v4l2, unsigned char *buffer, int size, int quality)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    JSAMPROW row_pointer[1];
    unsigned char *line_buffer, *yuyv;
    int z;
    static int written;
	static int inx = 0;
	FILE *fptr_jpg = NULL;

    line_buffer = (unsigned char *)calloc(v4l2->width * 3, 1);
    yuyv = v4l2->frame_buffer;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    //jpeg_stdio_dest (&cinfo, file); 
    //jpeg_mem_dest(&jcs, jpeg, &jpeg_size);
    dest_buffer(&cinfo, buffer, size, &written);

	if (++inx <= 10) {
		char buf[1024];
		xsprintf(buf, "/home/pi/Car/%d.jpg", inx);
		fptr_jpg = fopen (buf,"wb");//注意这里为什么用fopen而不用open  
	    if(fptr_jpg==NULL)  
	    {  
	    xerror("Encoder:open file failed!/n") ;  
	     return -1;  
	    }
		jpeg_stdio_dest(&cinfo, fptr_jpg);  
	}

    cinfo.image_width = v4l2->width;
    cinfo.image_height = v4l2->height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);

    jpeg_start_compress(&cinfo, TRUE);

    z = 0;
    if (v4l2->format == V4L2_PIX_FMT_YUYV) {
        while(cinfo.next_scanline < v4l2->height) {
            int x;
            unsigned char *ptr = line_buffer;

			for(x = 0; x < v4l2->width; x++)
			{
				int r, g, b;
				int y, u, v;
				
				if(!z)
					y = yuyv[0] << 8;
				else
					y = yuyv[2] << 8;
				u = yuyv[1] - 128;
				v = yuyv[3] - 128;

				r = (y + (359 * v)) >> 8;
				g = (y - (88 * u) - (183 * v)) >> 8;
				b = (y + (454 * u)) >> 8;

				*(ptr++) = (r > 255) ? 255 : ((r < 0) ? 0 : r);
				*(ptr++) = (g > 255) ? 255 : ((g < 0) ? 0 : g);
				*(ptr++) = (b > 255) ? 255 : ((b < 0) ? 0 : b);

				if(z++) {
					z = 0;
					yuyv += 4;
				}
			}

            row_pointer[0] = line_buffer;
            jpeg_write_scanlines(&cinfo, row_pointer, 1);
        }
    }
	
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    free(line_buffer);
	if (fptr_jpg) fclose(fptr_jpg);

    return (written);
}

const static unsigned char dht_data[] = {
    0xff, 0xc4, 0x01, 0xa2, 0x00, 0x00, 0x01, 0x05, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02,
    0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x01, 0x00, 0x03,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
    0x0a, 0x0b, 0x10, 0x00, 0x02, 0x01, 0x03, 0x03, 0x02, 0x04, 0x03, 0x05,
    0x05, 0x04, 0x04, 0x00, 0x00, 0x01, 0x7d, 0x01, 0x02, 0x03, 0x00, 0x04,
    0x11, 0x05, 0x12, 0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07, 0x22,
    0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08, 0x23, 0x42, 0xb1, 0xc1, 0x15,
    0x52, 0xd1, 0xf0, 0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16, 0x17,
    0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x34, 0x35, 0x36,
    0x37, 0x38, 0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a,
    0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x63, 0x64, 0x65, 0x66,
    0x67, 0x68, 0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a,
    0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95,
    0x96, 0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8,
    0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2,
    0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4, 0xd5,
    0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
    0xe8, 0xe9, 0xea, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9,
    0xfa, 0x11, 0x00, 0x02, 0x01, 0x02, 0x04, 0x04, 0x03, 0x04, 0x07, 0x05,
    0x04, 0x04, 0x00, 0x01, 0x02, 0x77, 0x00, 0x01, 0x02, 0x03, 0x11, 0x04,
    0x05, 0x21, 0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71, 0x13, 0x22,
    0x32, 0x81, 0x08, 0x14, 0x42, 0x91, 0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33,
    0x52, 0xf0, 0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34, 0xe1, 0x25,
    0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x35, 0x36,
    0x37, 0x38, 0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a,
    0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x63, 0x64, 0x65, 0x66,
    0x67, 0x68, 0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a,
    0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x92, 0x93, 0x94,
    0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
    0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba,
    0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4,
    0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
    0xe8, 0xe9, 0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa
};


int is_huffman(unsigned char *buf)
{
    unsigned char *ptbuf;
    int i = 0;
    ptbuf = buf;
    while(((ptbuf[0] << 8) | ptbuf[1]) != 0xffda) {
        if(i++ > 2048)
            return 0;
        if(((ptbuf[0] << 8) | ptbuf[1]) == 0xffc4)
            return 1;
        ptbuf++;
    }
    return 0;
}

int memcpy_picture(unsigned char *out, unsigned char *buf, int size)
{
    unsigned char *ptdeb, *ptlimit, *ptcur = buf;
    int sizein, pos = 0;

    if(!is_huffman(buf)) {
        ptdeb = ptcur = buf;
        ptlimit = buf + size;
        while((((ptcur[0] << 8) | ptcur[1]) != 0xffc0) && (ptcur < ptlimit))
            ptcur++;
        if(ptcur >= ptlimit)
            return pos;
        sizein = ptcur - ptdeb;

        memcpy(out + pos, buf, sizein); pos += sizein;
        memcpy(out + pos, dht_data, sizeof(dht_data)); pos += sizeof(dht_data);
        memcpy(out + pos, ptcur, size - sizein); pos += size - sizein;
    } else {
        memcpy(out + pos, ptcur, size); pos += size;
    }
    return pos;
}



int		v4l2_collect(v4l2_t *v4l2, unsigned char *out_buf, int *out_size)
{
	
#define HEADFRAME1 0xaf
	int i = 0, buf_size = 0, buf_maxsize = 0, jpeg_size = 0;
	
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

        xmemcpy(v4l2->tmp_buffer, v4l2->mem[v4l2->buf.index], buf_size);
		xmemcpy(v4l2->frame_buffer, v4l2->tmp_buffer, buf_size);		
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
	if (V4L2_PIX_FMT_YUYV == v4l2->format) {
		jpeg_size = compress_image_to_jpeg(v4l2, out_buf, *out_size, 80);
		*out_size = jpeg_size;
		xmessage("frame jpeg  size in: %d Byte\n", jpeg_size);
	} else {
		//xmemcpy(out_buf, v4l2->frame_buffer, buf_size);
		buf_size = memcpy_picture(out_buf, v4l2->frame_buffer, buf_size);
		*out_size = buf_size;
	}
	//xmemcpy(out_buf, v4l2->frame_buffer, buf_size);
	//*out_size = buf_size;
    return 0;
err:
    v4l2->is_quit = 0;
    return -1;
}






























