/**
 * \file
 * \brief lowLevelInitialization1() definition
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-10-30
 */

#include "distortos/Thread.hpp"

#include "distortos/scheduler/getScheduler.hpp"
#include "distortos/scheduler/Scheduler.hpp"
#include "distortos/scheduler/idleThreadFunction.hpp"
#include "distortos/scheduler/MainThreadControlBlock.hpp"

namespace distortos
{

namespace scheduler
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// size of idle thread's stack, bytes
constexpr size_t idleThreadStackSize {128};

/// idle thread's stack
uint64_t idleThreadStack[idleThreadStackSize / sizeof(uint64_t)];

/// type of idle thread
using IdleThread = decltype(makeThread(idleThreadStack, sizeof(idleThreadStack), 0, idleThreadFunction));

/// storage for idle thread instance
std::aligned_storage<sizeof(IdleThread), alignof(IdleThread)>::type idleThreadStorage;

/// storage for main thread instance
std::aligned_storage<sizeof(MainThreadControlBlock), alignof(MainThreadControlBlock)>::type
		mainThreadControlBlockStorage;

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Initializes main instance of system's Scheduler
 *
 * This function is called before constructors for global and static objects.
 */

extern "C" void lowLevelInitialization1()
{
	auto& mainThreadControlBlock = *new (&mainThreadControlBlockStorage) MainThreadControlBlock {UINT8_MAX};
	auto& idleThread = *new (&idleThreadStorage) IdleThread {idleThreadStack, sizeof(idleThreadStack), 0,
			idleThreadFunction};

	auto& schedulerInstance = getScheduler();
	new (&schedulerInstance) Scheduler {mainThreadControlBlock, idleThread};
}

}	// namespace scheduler

}	// namespace distortos