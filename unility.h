#pragma once

#define SEVER_SETUP_FALL			1		//启动服务器失败
#define SERVERPORT					5559	//服务器TCP端口
#define TIMEFOR_THREAD_EXIT			5000	//主线程睡眠时间
#define TIMEFOR_THREAD_HELP			1500	//清理资源线程退出时间
#define TIMEFOR_THREAD_SLEEP		500		//等待客户端请求线程睡眠时间
#define MAX_NUM_BUF		            48		//缓冲区的最大长度
#define MIN_NUM_BUF					6		//数据的最小长度
#define TIMEFOR_THREAD_CLIENT		500		//线程睡眠时间
#define ARRAYLEN					(MAX_NUM_BUF+1)
