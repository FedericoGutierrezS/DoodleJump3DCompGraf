#pragma once
#include "Timer.h"


	Timer::Timer() {
		m_time.now = SDL_GetPerformanceCounter();
		m_time.then = m_time.now;
		m_time.frequency = SDL_GetPerformanceFrequency();
	}
	// Note: delta is since last touch OR peek
	const TimeData& Timer::touch() {
		m_time.then = m_time.now;
		m_time.now = SDL_GetPerformanceCounter();
		m_time.delta = double(m_time.now - m_time.then) / double(m_time.frequency);

		return m_time;
	}
	// Note: delta is since last touch
	const TimeData& Timer::peek() {
		m_time.now = SDL_GetPerformanceCounter();
		m_time.delta = double(m_time.now - m_time.then) / double(m_time.frequency);
		return m_time;
	}