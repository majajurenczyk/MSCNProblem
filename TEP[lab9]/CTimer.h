#pragma once
#include<Windows.h>;
class CTimer
{
public:
	CTimer();
	~CTimer();
	void vSetTimer(double dTime);
	bool bCheckIfTimePassed();
private:
	LARGE_INTEGER l_timer_stop_time;
	
};

