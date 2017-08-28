#ifndef __TIMER_H_
#define __TIMER_H_
#include <sys/time.h>
#include <string>

using namespace std;

#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, "DEBUG", fmt, ##args)

class Timer {
public:
  Timer(const char *_name) : name(_name), pTotalTime(NULL) {
    gettimeofday(&start, NULL);
  }

  Timer(const char *_name, float *_pTotal) : name(_name), pTotalTime(_pTotal) {
    gettimeofday(&start, NULL);
  }

  ~Timer() {
    gettimeofday(&end, NULL);
    float interval = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000.0f;
    if (pTotalTime) {
      *pTotalTime += interval;
      LOGE("Total time of %s: %f ms\n", name.c_str(), *pTotalTime);
    }
    else {
      LOGE("T:%lu.%lu, %s: %f ms\n", end.tv_sec, end.tv_usec, name.c_str(), interval);
    }
  }

private:
  string name;
  struct timeval start;
  struct timeval end;
  float *pTotalTime;
};

#endif
