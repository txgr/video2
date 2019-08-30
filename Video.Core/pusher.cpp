#include "pch.h"
#include "pusher.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "streamer.hpp"
#include <libavutil/avassert.h>
#include <libavutil/channel_layout.h>
#include <libavutil/opt.h>
#include <libavutil/mathematics.h>
#include <libavutil/timestamp.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <cstdio> 
#include <ctime>
#include <windows.h>

#define STREAM_DURATION   10.0
#define STREAM_FRAME_RATE 25 /* 25 images/s */
#define STREAM_PIX_FMT    AV_PIX_FMT_YUV420P /* default pix_fmt */

#define SCALE_FLAGS SWS_BICUBIC


using namespace std;
using namespace streamer;


class MovingAverage
{
	int size;
	int pos;
	bool crossed;
	std::vector<double> v;

public:
	explicit MovingAverage(int sz)
	{
		size = sz;
		v.resize(size);
		pos = 0;
		crossed = false;
	}

	void add_value(double value)
	{
		v[pos] = value;
		pos++;
		if (pos == size) {
			pos = 0;
			crossed = true;
		}
	}

	double get_average()
	{
		double avg = 0.0;
		int last = crossed ? size : pos;
		int k = 0;
		for (k = 0; k < last; k++) {
			avg += v[k];
		}
		return avg / (double)last;
	}
};




void usleep(__int64 usec)
{
	HANDLE timer;
	LARGE_INTEGER ft;

	ft.QuadPart = -(10 * usec); // Convert to 100 nanosecond interval, negative value indicates relative time

	timer = CreateWaitableTimer(NULL, TRUE, NULL);
	SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
}

static void add_delay(size_t streamed_frames, size_t fps, double elapsed, double avg_frame_time)
{
	//compute min number of frames that should have been streamed based on fps and elapsed
	double dfps = fps;
	size_t min_streamed = (size_t)(dfps * elapsed);
	size_t min_plus_margin = min_streamed + 2;

	if (streamed_frames > min_plus_margin) {
		size_t excess = streamed_frames - min_plus_margin;
		double dexcess = excess;

		//add a delay ~ excess*processing_time
//#define SHOW_DELAY
#ifdef SHOW_DELAY
		double delay = dexcess * avg_frame_time * 1000000.0;
		printf("frame %07lu adding delay %.4f\n", streamed_frames, delay);
		printf("avg fps = %.2f\n", streamed_frames / elapsed);
#endif
		usleep(dexcess * avg_frame_time * 1000000.0);
		//std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

void process_frame(const cv::Mat& in, cv::Mat& out)
{
	in.copyTo(out);
}

string  getTime() {

	time_t timep;
	time(&timep);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));
	return tmp;
}

char* __stdcall Test2(char* video_fname) {
	return video_fname;
}
int __stdcall Test() {
	return 999999;
}

cv::VideoCapture video_capture;

//视频总帧数
int __stdcall GetPositionCount() {
	int count = video_capture.get(cv::CAP_PROP_FRAME_COUNT);
	return count;
 }

//设置播放位置
int __stdcall SetPosition(double position) {
	//设置播放到哪一帧，这里设置为第0帧  CV_CAP_PROP_POS_FRAMES
	return video_capture.set(cv::CAP_PROP_POS_FRAMES, position);
 }

//当前播放位置
int __stdcall GetCurrentPosition() {
	return video_capture.get(cv::CAP_PROP_POS_MSEC);
}
int __stdcall  Start(char* video_fname, char* rtmp_url)
{
//	std::string video_fname = "G:\\0.flv";
//	std::string rtmp_url = "rtmp://159.138.5.74:2250/live/fghgfhgfhfgdgd";
	
	video_capture = cv::VideoCapture(video_fname, cv::CAP_FFMPEG);

	if (!video_capture.isOpened()) {
		//fprintf(stderr, "could not open video %s\n", video_fname.c_str());
		video_capture.release();
		return 1;
	}

	int cap_frame_width = video_capture.get(cv::CAP_PROP_FRAME_WIDTH);
	int cap_frame_height = video_capture.get(cv::CAP_PROP_FRAME_HEIGHT);

	int cap_fps = video_capture.get(cv::CAP_PROP_FPS);
	printf("video info w = %d, h = %d, fps = %d\n", cap_frame_width, cap_frame_height, cap_fps);

	int stream_fps = cap_fps;
	
	int bitrate = 500000;
	Streamer streamer;
	StreamerConfig streamer_config(cap_frame_width, cap_frame_height,
		640, 360,
		stream_fps, bitrate, "high444", rtmp_url);

	
	streamer.enable_av_debug_log();
	streamer.init(streamer_config);

	size_t streamed_frames = 0;

	std::chrono::high_resolution_clock clk;
	std::chrono::high_resolution_clock::time_point time_start = clk.now();
	std::chrono::high_resolution_clock::time_point time_stop = time_start;
	std::chrono::high_resolution_clock::time_point time_prev = time_start;

	MovingAverage moving_average(10);
	double avg_frame_time;

	cv::Mat read_frame;
	cv::Mat proc_frame;
	bool ok = video_capture.read(read_frame);

	std::chrono::duration<double> elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(time_stop - time_start);
	std::chrono::duration<double> frame_time = std::chrono::duration_cast<std::chrono::duration<double>>(time_stop - time_prev);

	int count = GetPositionCount();

	SetPosition(6768);
	int sdf = GetCurrentPosition();

	while (ok) {
		process_frame(read_frame, proc_frame);

		string text = getTime();

		int font_face = cv::FONT_HERSHEY_COMPLEX;
		double font_scale = 2;
		int thickness = 2;
		int baseline;
		//获取文本框的长宽
		cv::Size text_size = cv::getTextSize(text, font_face, font_scale, thickness, &baseline);

		//将文本框居中绘制
		cv::Point origin;
		origin.x = proc_frame.cols / 2 - text_size.width / 2;
		origin.y = proc_frame.rows / 2 + text_size.height / 2;
		cv::putText(proc_frame, text, origin, font_face, font_scale, cv::Scalar(0, 255, 255), thickness, 8, 0);


		streamer.stream_frame(proc_frame, frame_time.count() * streamer.inv_stream_timebase);

		time_stop = clk.now();
		elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(time_stop - time_start);
		frame_time = std::chrono::duration_cast<std::chrono::duration<double>>(time_stop - time_prev);


		streamed_frames++;
		moving_average.add_value(frame_time.count());
		avg_frame_time = moving_average.get_average();
		add_delay(streamed_frames, stream_fps, elapsed_time.count(), avg_frame_time);


		ok = video_capture.read(read_frame);
		time_prev = time_stop;
	}
	video_capture.release();

	return 0;
}