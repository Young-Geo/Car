#ifndef __G_ROBOT_H__
#define __G_ROBOT_H__

#define COMMIP "192.168.0.104"
#define COMMPORT 9001

#define VIDEOIP COMMIP
#define VIDEOPORT 9002

//全局参数
enum _comm {
	CONTROL_MODEL 		= 		0x01,
	AUTO_MODEL 			=		0x02,
	COMM_LEFT 			=		0x03,
	COMM_RIGHT 			= 		0x04,
	COMM_FORWARD 		= 		0x05,
	COMM_BACK 			= 		0x06,
	COMM_STOP 			= 		0x07,
	DANGER 				= 		0x44,
	SAFE 				= 		0x66,
};















#endif