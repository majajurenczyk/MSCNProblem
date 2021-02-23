#include "pch.h"
#include "CTimer.h"


CTimer::CTimer(){
}


CTimer::~CTimer(){
}

void CTimer::vSetTimer(double dTime) {
	LARGE_INTEGER li_actCount, li_freq;

	QueryPerformanceFrequency(&li_freq);
	QueryPerformanceCounter(&li_actCount);

	l_timer_stop_time.QuadPart = li_actCount.QuadPart + dTime * li_freq.QuadPart;
}

bool CTimer::bCheckIfTimePassed() {
	LARGE_INTEGER li_actCount;
	QueryPerformanceCounter(&li_actCount);
	return li_actCount.QuadPart >= l_timer_stop_time.QuadPart;
}