/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
#pragma once

#include <lfant/stdafx.h>

// External
#include <boost/chrono.hpp>
#include <boost/date_time.hpp>

// Internal
#include <lfant/util/qumap.h>
#include <lfant/Subsystem.h>

using namespace boost::chrono;

namespace lfant
{
/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Core
 *	 @{
 */

typedef boost::chrono::high_resolution_clock hclock;

class Timer
{
public:
	string name;
	float time;

	Timer(string name, float time) :
		name(name), time(time)
	{
	}

	~Timer() {}
};

/**
 *
 */
class Time : public Subsystem
{
public:
	Time(Game* game);
	~Time();

	static void ScriptBind();

	virtual void Init();
	virtual void Update();

	/// Gets the amount of seconds since Game::Init was called.
	double GetTime();
	void ResetTime();
	void UpdateTimes();

	void SetTimer(string name, float time);
	void CancelTimer(string name);
	float* GetTimer(string name);

	boost::posix_time::ptime& GetDate();

	// Properties

	/// Deltatime in seconds
	float deltaTime = 0.016667f;
	float deltaTimeFixed = 0.016667f;
	float frameRate = 60.0f;
	float timeScale = 1.0f;
	double lastFrame;

private:
	hclock::time_point startTime;
	boost::posix_time::ptime dateTime;
	std::vector<Timer> timers;
};

/** @} */
/** @} */
}
