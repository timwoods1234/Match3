#ifndef __TETIMER_H__
#define __TETIMER_H__

//+----------------------------------------
//| The game timer.
//| Version 1.001
//+----------------------------------------

class TETimer
{
private:
	bool			m_bTimerInitialized;
	bool			m_bUsingQPF;
	bool  			m_bTimerStopped;

    __int64 		m_nQPFTicksPerSec;
	__int64			m_nCurrElapsedTime;
	__int64			m_nLastElapsedTime;
	__int64			m_nStopTime;

	__int64			m_nBaseTime;

	float			m_g_fElapsedTime;
	float			m_fSecsPerCount;

	float			m_fNumFrames;
	float			m_fFrameTimer;
	float			m_fFPS;				// Frames per second
	float			m_g_fMsPerFrame;

public:
	TETimer();
	~TETimer();

	void			Init();
	void			Update();

	void			Pause(bool pause);
	void			Reset();
	void			Start();
	void			Stop();
	void			Advance();
	void			FrameMove();

	float			GetAppTime()			const;
	float			GetAbsoluteTime()		const;
	float			GetElapsedTime()		const;
	float			GetMsPerFrame()			const;
	float			GetFramesPerSecond()	const;
}; // TETimer

#endif
