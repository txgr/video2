// TestCpp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "../../../vs2017/Video.Core/Video.Core/pusher.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <cstdlib>
#include <Windows.h>
using namespace std;
//#pragma comment(lib,"Video.Core.dll")

//线程函数
DWORD WINAPI Fun(LPVOID lpParamter)
{
	const  char* video_fname = "G:\\0.flv";
	const  char* rtmp_url = "rtmp://159.138.5.74:2250/live/fghgfhgfhfgdgd";
	Start(const_cast<char*>(video_fname), const_cast<char*>(rtmp_url));
	return 0;

}

int main()
{
	
	HANDLE hThread = CreateThread(NULL, 0, Fun, NULL, 0, NULL);
	WaitForSingleObject(hThread, INFINITE); // 等待，直到线程被激发
//	CloseHandle(hThread);


    std::cout << "Hello World!\n";
	
	int count = GetPositionCount();

	SetPosition(50000);
	int sdf = GetCurrentPosition();
	int dsff = 0;
}




void* say_hello(void* args)
{
	cout << "Hello Runoob！" << endl;
	return 0;
}


