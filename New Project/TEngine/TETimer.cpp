#include "TETimer.h"
#include <windows.h>

//-------------------------------------------------------------------------------
TETimer::TETimer()
{
	m_bTimerInitialized		= FALSE;
	m_bTimerStopped			= TRUE;

    m_nQPFTicksPerSec		= 0;
	m_nCurrElapsedTime		= 0;
	m_nLastElapsedTime		= 0;
    m_nStopTime				= 0;

	m_nBaseTime				= 0;
	m_nLastElapsedTime		= 0;

	m_g_fElapsedTime			= 0.0f;
	m_fSecsPerCount			= 0.0f;
	m_fNumFrames			= 0.0f;
	m_fFrameTimer			= 0.0f;
	m_fFPS					= 0.0f;
}

//-------------------------------------------------------------------------------
TETimer::~TETimer()
{

}

//-------------------------------------------------------------------------------
void TETimer::Init()
{
    // Initialize the timer
    if( FALSE == m_bTimerInitialized )
    {
        m_bTimerInitialized = TRUE;

        // Use QueryPerformanceFrequency() to get frequency of timer.
		QueryPerformanceFrequency((LARGE_INTEGER*)&m_nQPFTicksPerSec);
		m_fSecsPerCount = 1.0f / (float)m_nQPFTicksPerSec;

		QueryPerformanceCounter((LARGE_INTEGER*)&m_nLastElapsedTime);
    }
}

//-------------------------------------------------------------------------------
void TETimer::Update()
{
    // Get either the current time or the stop
	// time, depending on whether we're stopped
    if( m_nStopTime != 0)
        m_nCurrElapsedTime = m_nStopTime;
    else
        QueryPerformanceCounter( (LARGE_INTEGER*)&m_nCurrElapsedTime );

	// Update Elapsed Time
    m_g_fElapsedTime = (m_nCurrElapsedTime - m_nLastElapsedTime) * m_fSecsPerCount;
    m_nLastElapsedTime = m_nCurrElapsedTime;

	m_fFrameTimer += m_g_fElapsedTime;
	if (m_fFrameTimer > 1.0f)
	{
		m_fFPS			= m_fNumFrames;
		m_g_fMsPerFrame	= 1000.0f / m_fFPS;

		m_fNumFrames	= 0.0f;
		m_fFrameTimer	= 0.0f;
	}
}

//-------------------------------------------------------------------------------
void TETimer::Reset()
{
	// Reset the timer
    m_nBaseTime        = m_nCurrElapsedTime;
    m_nLastElapsedTime = m_nCurrElapsedTime;
    m_nStopTime        = 0;
    m_bTimerStopped    = FALSE;
}

//-------------------------------------------------------------------------------
void TETimer::Start()
{
	// Start the timer
    if( m_bTimerStopped )
        m_nBaseTime += m_nCurrElapsedTime - m_nStopTime;

    m_nStopTime			= 0;
    m_nLastElapsedTime  = m_nCurrElapsedTime;

    m_bTimerStopped = FALSE;
}

//-------------------------------------------------------------------------------
void TETimer::Stop()
{
    // Stop the timer
	m_nStopTime			= m_nCurrElapsedTime;
	m_nLastElapsedTime  = m_nCurrElapsedTime;

	m_bTimerStopped = TRUE;
}

//-------------------------------------------------------------------------------
void TETimer::Advance()
{
	m_nStopTime += m_nQPFTicksPerSec / 10;
}

//-------------------------------------------------------------------------------
void TETimer::FrameMove()
{
	m_fNumFrames += 1.0f;
}

//-------------------------------------------------------------------------------
float TETimer::GetElapsedTime()	const
{
	return m_g_fElapsedTime; 
}

//-------------------------------------------------------------------------------
float TETimer::GetMsPerFrame() const
{
	return m_g_fMsPerFrame; 
}

//-------------------------------------------------------------------------------
float TETimer::GetFramesPerSecond()	const
{
	return m_fFPS;
}

//-------------------------------------------------------------------------------
float TETimer::GetAppTime() const
{
    // Return the current time
	return (float) ( m_nCurrElapsedTime - m_nBaseTime ) * m_fSecsPerCount;
}

//-------------------------------------------------------------------------------
float TETimer::GetAbsoluteTime() const
{
	return (float) m_nCurrElapsedTime * m_fSecsPerCount;
}

//-------------------------------------------------------------------------------
void TETimer::Pause(bool pause)
{
	if (pause)
	{
		Start();
	}
	else
	{
		Stop();
	}
}
