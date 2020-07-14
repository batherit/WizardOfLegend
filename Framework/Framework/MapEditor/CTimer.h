#pragma once

class CTimer final {
public:
	CTimer(TIMER::E_TYPE _eTimerType = TIMER::TIMER_TYPE_TIME);
	~CTimer();

	void Reset();						// Ÿ�̸� �Ӽ��� �ʱ�ȭ
	void RunTick();						// �� ���� ƽ (�� ����� �ð��� �����Ѵ�.)
	void RunToPause();					// �Ͻ� ����
	void PauseToRun();					// Ÿ�̸� �۵� (���� ȣ�� �� ù ȣ�� �������� �ð� ����)

	float GetElapsedTimePerFrame();		// ������ �� ����� �ð�(sec)
	float GetCurTime();					// ���� �ð�(sec)
	float GetTotalTimeFromRun();		// ���� RunTick ���� �� ����� �ð�

private:
	bool _bIsPaused;					// �Ͻ������Ǿ�����?
	TIMER::E_TYPE m_eTimerType;			// Ÿ�̸� Ÿ�� (time.h || windows.h)
	time_t m_timeBase;					// ������ �⺻ �ð� (ī��Ʈ)
	time_t m_timeBefore;				// ���� �ð� (ī��Ʈ)
	time_t m_timeCurrent;				// ���� �ð� (ī��Ʈ)
	time_t m_timePausePoint;  		// �Ͻ������� ���� (ī��Ʈ)
	time_t m_timePauseTotal;		// ������ �Ͻ����� �� �ð� (ī��Ʈ)

	float m_fPeriod;					// �ֱ� : �� ���� ī��Ʈ �� �ҿ�Ǵ� �ð� (sec)
	float m_fElapsedTimePerFrame; 		// ������ �� ��� �ð� (sec)
};

class CGameWorldTimer {
public:
	static CTimer* Instance() {
		static CTimer *instance = new CTimer(TIMER::TIMER_TYPE_TICKCOUNT);
		return instance;
	}
};