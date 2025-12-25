#pragma once

#include "Common.hpp"
#include <chrono>

class CTickCountDelay
{
public:
	inline auto Start( ULONGLONG dwTime ) -> void
	{
		if ( !m_dwDelayTime && !m_bActive )
		{
			m_dwDelayTime = GetTickCount64() + dwTime;
			m_bActive = true;
		}
	}

	inline auto IsEnd() -> bool
	{
		if ( m_dwDelayTime <= GetTickCount64() )
		{
			m_bActive = false;
			return true;
		}

		return false;
	}

	inline auto Reset() -> void
	{
		m_dwDelayTime = 0;
		m_bActive = false;
	}

private:
	bool m_bActive = false;
	ULONGLONG m_dwDelayTime = 0;
};

class CChronoDelay
{
public:
	inline auto Start( int64_t DelayTime ) -> void
	{
		if ( !m_nDelayEndTime && !m_bActive )
		{
			auto Now = std::chrono::high_resolution_clock::now();
			auto CurrentMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>( Now.time_since_epoch() ).count();

			m_nDelayEndTime = CurrentMilliseconds + DelayTime;
			m_bActive = true;
		}
	}

	inline auto IsEnd() -> bool
	{
		auto Now = std::chrono::high_resolution_clock::now();
		auto CurrentMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>( Now.time_since_epoch() ).count();

		if ( m_nDelayEndTime <= CurrentMilliseconds )
		{
			m_bActive = false;
			return true;
		}

		return false;
	}

	inline auto Reset() -> void
	{
		m_nDelayEndTime = 0;
		m_bActive = false;
	}

private:
	bool m_bActive = false;
	int64_t m_nDelayEndTime = 0;
};
