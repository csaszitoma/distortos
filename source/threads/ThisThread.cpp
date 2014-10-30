/**
 * \file
 * \brief ThisThread namespace implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-10-30
 */

#include "distortos/ThisThread.hpp"

#include "distortos/scheduler/getScheduler.hpp"
#include "distortos/scheduler/Scheduler.hpp"

namespace distortos
{

namespace ThisThread
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

uint8_t getPriority()
{
	return scheduler::getScheduler().getCurrentThreadControlBlock().getPriority();
}

void setPriority(const uint8_t priority, const bool alwaysBehind)
{
	scheduler::getScheduler().getCurrentThreadControlBlock().setPriority(priority, alwaysBehind);
}

void sleepFor(const TickClock::duration duration)
{
	sleepUntil(TickClock::now() + duration + TickClock::duration{1});
}

void sleepUntil(const TickClock::time_point timePoint)
{
	scheduler::ThreadControlBlockList sleepingList {scheduler::getScheduler().getThreadControlBlockListAllocator(),
			scheduler::ThreadControlBlock::State::Sleeping};
	scheduler::getScheduler().blockUntil(sleepingList, timePoint);
}

void yield()
{
	scheduler::getScheduler().yield();
}

}	// namespace ThisThread

}	// namespace distortos