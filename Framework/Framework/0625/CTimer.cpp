#include "stdafx.h"
#include "CTimer.h"


CTimer::CTimer(TIMER::E_TYPE _eTimerType)
	:
	_bIsPaused(false),
	m_eTimerType(_eTimerType),
	m_timePausePoint(0),
	m_timePauseTotal(0),
	m_fElapsedTimePerFrame(0.f)
{
	time_t timeFrequency;
	switch (m_eTimerType)
	{
	case TIMER::TIMER_TYPE_TIME:
		m_timeCurrent = clock();
		timeFrequency = CLOCKS_PER_SEC;
		break;
	case TIMER::TIMER_TYPE_TICKCOUNT:
		m_timeCurrent = GetTickCount();
		timeFrequency = CLOCKS_PER_SEC;
		break;
	case TIMER::TIMER_TYPE_WINDOWS:
		QueryPerformanceCounter((LARGE_INTEGER*)&m_timeCurrent);
		QueryPerformanceFrequency((LARGE_INTEGER*)&timeFrequency);
		break;
	default:
		break;
	}
	m_timeBefore = m_timeCurrent;
	m_timeBase = m_timeCurrent;
	m_fPeriod = 1.f / static_cast<float>(timeFrequency);
}


CTimer::~CTimer()
{
}

void CTimer::Reset() {
	_bIsPaused = false;
	m_timePausePoint = 0;
	m_timePauseTotal = 0;
	m_fElapsedTimePerFrame = 0.f;

	switch (m_eTimerType)
	{
	case TIMER::TIMER_TYPE_TIME:
		m_timeCurrent = clock();
		break;
	case TIMER::TIMER_TYPE_TICKCOUNT:
		m_timeCurrent = GetTickCount();
		break;
	case TIMER::TIMER_TYPE_WINDOWS:
		QueryPerformanceCounter((LARGE_INTEGER*)&m_timeCurrent);
		break;
	default:
		break;
	}
	m_timeBefore = m_timeCurrent;
	m_timeBase = m_timeCurrent;
}

void CTimer::RunTick() {
	if (_bIsPaused) {
		m_fElapsedTimePerFrame = 0.f;
		return;
	}

	switch (m_eTimerType)
	{
	case TIMER::TIMER_TYPE_TIME:
		m_timeCurrent = clock();
		break;
	case TIMER::TIMER_TYPE_TICKCOUNT:
		m_timeCurrent = GetTickCount();
		break;
	case TIMER::TIMER_TYPE_WINDOWS:
		QueryPerformanceCounter((LARGE_INTEGER*)&m_timeCurrent);
		break;
	default:
		break;
	}

	m_fElapsedTimePerFrame = (m_timeCurrent - m_timeBefore) * m_fPeriod;
	m_timeBefore = m_timeCurrent;

	// 프로세서가 절전 모드로 들어가거나 다른 프로세서와 엉키는 경우
	// elapsed_time_per_frame_은 음수가 될 수 있다.
	if (m_fElapsedTimePerFrame < 0.f) {
		m_fElapsedTimePerFrame = 0.f;
	}
}

void CTimer::RunToPause() {
	if (!_bIsPaused) {
		switch (m_eTimerType)
		{
		case TIMER::TIMER_TYPE_TIME:
			m_timePausePoint = clock();
			break;
		case TIMER::TIMER_TYPE_TICKCOUNT:
			m_timePausePoint = GetTickCount();
			break;
		case TIMER::TIMER_TYPE_WINDOWS:
			QueryPerformanceCounter((LARGE_INTEGER*)&m_timePausePoint);
			break;
		default:
			break;
		}
		_bIsPaused = true;
	}
}

void CTimer::PauseToRun() {

	if (_bIsPaused) {
		time_t timeRunPoint;
		switch (m_eTimerType)
		{
		case TIMER::TIMER_TYPE_TIME:
			timeRunPoint = clock();
			break;
		case TIMER::TIMER_TYPE_TICKCOUNT:
			timeRunPoint = GetTickCount();
			break;
		case TIMER::TIMER_TYPE_WINDOWS:
			QueryPerformanceCounter((LARGE_INTEGER*)&timeRunPoint);
			break;
		default:
			break;
		}

		m_timePauseTotal += (timeRunPoint - m_timePausePoint);
		m_timeBefore = timeRunPoint;
		m_timePausePoint = 0;
		_bIsPaused = false;
	}
}

float CTimer::GetElapsedTimePerFrame() {
	return m_fElapsedTimePerFrame;
}

float CTimer::GetCurTime() {
	return m_timeCurrent * m_fPeriod;
}

float CTimer::GetTotalTimeFromRun() {
	float fTotalTime = 0.f;

	if (_bIsPaused) {
		fTotalTime = static_cast<float>((m_timePausePoint - m_timePauseTotal) - m_timeBase) * m_fPeriod;
	}
	else {
		switch (m_eTimerType)
		{
		case TIMER::TIMER_TYPE_TIME:
			m_timeCurrent = clock();
			break;
		case TIMER::TIMER_TYPE_TICKCOUNT:
			m_timeCurrent = GetTickCount();
			break;
		case TIMER::TIMER_TYPE_WINDOWS:
			QueryPerformanceCounter((LARGE_INTEGER*)&m_timeCurrent);
			break;
		default:
			break;
		}
		fTotalTime = static_cast<float>((m_timeCurrent - m_timePauseTotal) - m_timeBase) * m_fPeriod;
	}
	return fTotalTime;
}