#ifndef TIME_H
#define TIME_H

#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <math.h>

static double qpcFrequency;


struct Time
{
	static Time& getInstance() {
		static Time instance;
		return instance;
	}

	void init() {
		if (QueryPerformanceCounter((LARGE_INTEGER*)&m_initialTime)) {
			m_QPC = true;

			//std::cout << m_initialTime << std::endl;

			m_freq = 1000.0f / m_initialTime;

			m_MMTimerStart = timeGetTime();
			//std::cout << "FREQUENCE : " << m_freq << "\n" << std::endl;

			m_prevFrameTime = (unsigned)(m_initialTime * m_freq);

			//std::cout << "FIRST FRAME : " << m_prevFrameTime << std::endl;
		}
		else {
			m_QPC = false;

			m_MMTimerStart = timeGetTime();
			m_MMTimerElapsed = m_MMTimerStart;
			m_freq = 1.0f / 1000.0f;
			m_initialTime = timeGetTime();
		}
	}

	// Internal time and clock access functions
	unsigned systemTime()
	{
		if (m_QPC)
		{
			static LONGLONG qpcMillisPerTick;
			QueryPerformanceCounter((LARGE_INTEGER*)&qpcMillisPerTick);
			return (unsigned)(qpcMillisPerTick * qpcFrequency);
		}
		else
		{
			return unsigned(timeGetTime());
		}

	}

#if TIMING_WINDOWS
	unsigned long systemClock()
	{
		__asm {
			rdtsc;
		}
	}
#endif

	void update() {
		m_nbFrames++;

		//std::cout << m_nbFrames << " frames" << std::endl;

		unsigned now = timeGetTime();

		//std::cout << m_prevFrameTime << std::endl;
		
		m_frameDuration = now - m_prevFrameTime;
		//std::cout << "Now : " << now << " ---- prevFrameTime : " << m_prevFrameTime << " ---- frameDuration : " << m_frameDuration << std::endl;

		m_prevFrameTime = now;
		m_hundredFramesDuration += m_frameDuration;

		if (m_nbFrames % 100 == 0) {
			m_avFrameDuration = m_hundredFramesDuration / 100;
			m_hundredFramesDuration = 0;
		}
		else {
			m_avFrameDuration = m_frameDuration;
		}

		m_fps = (float)(1000.0f / m_avFrameDuration);
	}

	unsigned getNow() const {
		/*if (m_QPC) {
			static LONGLONG qpcMillisPerTick;
			QueryPerformanceCounter((LARGE_INTEGER*)&qpcMillisPerTick);
			return (unsigned)(qpcMillisPerTick * qpcFrequency);

		}
		else {
			return (unsigned)(timeGetTime() );
		}*/
		return timeGetTime();
	}

	unsigned getFrameDuration() const {
		return m_frameDuration;
	}

	float getFPS() const {
		return m_fps;
	}

	~Time() {}


private:
	Time() :
		m_initialTime(0), m_freq(0), m_QPC(true), m_fps(0.0f), m_prevFrameTime(0), m_avFrameDuration(0),
		m_hundredFramesDuration(0), m_frameDuration(0), m_nbFrames(0)
	{}

	unsigned long long m_initialTime;
	double m_freq;
	bool m_QPC;

	// Multimedia timer variables
	unsigned long m_MMTimerStart;
	unsigned long m_MMTimerElapsed;

	// FPS variables
	unsigned m_prevFrameTime;
	unsigned m_frameDuration;
	unsigned m_avFrameDuration;
	unsigned m_hundredFramesDuration;
	float m_fps;
	unsigned m_nbFrames;
};

#endif