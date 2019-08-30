#pragma once

#include "pusher.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


#include <cstdio> 


using namespace std;
extern "C" {

	_declspec(dllexport) int __stdcall GetCurrentPosition();
	_declspec(dllexport) int __stdcall GetPositionCount();
	_declspec(dllexport) int __stdcall SetPosition(double position);

	_declspec(dllexport) int __stdcall Start(char* video_fname, char* rtmp_url);
	_declspec(dllexport) int __stdcall Test();
	_declspec(dllexport) char* __stdcall Test2(char* video_fname);
}
