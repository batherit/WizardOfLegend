#pragma once

class CTimer final {
public:
	CTimer(TIMER::E_TYPE _eTimerType = TIMER::TIMER_TYPE_TIME);
	~CTimer();

	void Reset();						// 타이머 속성을 초기화
	void RunTick();						// 한 번의 틱 (곧 경과된 시간을 측정한다.)
	void RunToPause();					// 일시 정지
	void PauseToRun();					// 타이머 작동 (연속 호출 시 첫 호출 기준으로 시간 측정)

	float GetElapsedTimePerFrame();		// 프레임 당 경과된 시간(sec)
	float GetCurTime();					// 현재 시간(sec)
	float GetTotalTimeFromRun();		// 최초 RunTick 실행 후 경과된 시간

private:
	bool _bIsPaused;					// 일시정지되었는지?
	TIMER::E_TYPE m_eTimerType;			// 타이머 타입 (time.h || windows.h)
	time_t m_timeBase;					// 최초의 기본 시간 (카운트)
	time_t m_timeBefore;				// 이전 시간 (카운트)
	time_t m_timeCurrent;				// 현재 시간 (카운트)
	time_t m_timePausePoint;  		// 일시정지된 시점 (카운트)
	time_t m_timePauseTotal;		// 누적된 일시정지 총 시간 (카운트)

	float m_fPeriod;					// 주기 : 한 번의 카운트 당 소요되는 시간 (sec)
	float m_fElapsedTimePerFrame; 		// 프레임 당 경과 시간 (sec)
};

class CGameWorldTimer {
public:
	static CTimer* Instance() {
		static CTimer *instance = new CTimer(TIMER::TIMER_TYPE_TICKCOUNT);
		return instance;
	}
};