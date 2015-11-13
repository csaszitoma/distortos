/**
 * \file
 * \brief Thread class implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-11-13
 */

#include "distortos/Thread.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

#include "distortos/scheduler/getScheduler.hpp"
#include "distortos/scheduler/Scheduler.hpp"

#include "distortos/synchronization/SignalsReceiverControlBlock.hpp"

#include <cerrno>

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

Thread::Thread(architecture::Stack&& stack, const uint8_t priority, const SchedulingPolicy schedulingPolicy,
		scheduler::ThreadGroupControlBlock* const threadGroupControlBlock, SignalsReceiver* const signalsReceiver) :
		threadControlBlock_{std::move(stack), priority, schedulingPolicy, threadGroupControlBlock, signalsReceiver,
				*this},
		joinSemaphore_{0}
{

}

Thread::~Thread()
{

}

int Thread::generateSignal(const uint8_t signalNumber)
{
	auto& threadControlBlock = getThreadControlBlock();
	const auto signalsReceiverControlBlock = threadControlBlock.getSignalsReceiverControlBlock();
	if (signalsReceiverControlBlock == nullptr)
		return ENOTSUP;

	return signalsReceiverControlBlock->generateSignal(signalNumber, threadControlBlock);
}

uint8_t Thread::getEffectivePriority() const
{
	return getThreadControlBlock().getEffectivePriority();
}

SignalSet Thread::getPendingSignalSet() const
{
	const auto signalsReceiverControlBlock = getThreadControlBlock().getSignalsReceiverControlBlock();
	if (signalsReceiverControlBlock == nullptr)
		return SignalSet{SignalSet::empty};

	architecture::InterruptMaskingLock interruptMaskingLock;
	return signalsReceiverControlBlock->getPendingSignalSet();
}

uint8_t Thread::getPriority() const
{
	return getThreadControlBlock().getPriority();
}

SchedulingPolicy Thread::getSchedulingPolicy() const
{
	return getThreadControlBlock().getSchedulingPolicy();
}

scheduler::ThreadControlBlock::State Thread::getState() const
{
	return getThreadControlBlock().getState();
}

int Thread::join()
{
	if (&getThreadControlBlock() == &scheduler::getScheduler().getCurrentThreadControlBlock())
		return EDEADLK;

	int ret;
	while ((ret = joinSemaphore_.wait()) == EINTR);
	return ret;
}

int Thread::queueSignal(const uint8_t signalNumber, const sigval value)
{
	auto& threadControlBlock = getThreadControlBlock();
	const auto signalsReceiverControlBlock = threadControlBlock.getSignalsReceiverControlBlock();
	if (signalsReceiverControlBlock == nullptr)
		return ENOTSUP;

	return signalsReceiverControlBlock->queueSignal(signalNumber, value, threadControlBlock);
}

void Thread::setPriority(const uint8_t priority, const bool alwaysBehind)
{
	getThreadControlBlock().setPriority(priority, alwaysBehind);
}

void Thread::setSchedulingPolicy(const SchedulingPolicy schedulingPolicy)
{
	getThreadControlBlock().setSchedulingPolicy(schedulingPolicy);
}

int Thread::start()
{
	if (getState() != scheduler::ThreadControlBlock::State::New)
		return EINVAL;

	return scheduler::getScheduler().add(getThreadControlBlock());
}

/*---------------------------------------------------------------------------------------------------------------------+
| protected functions
+---------------------------------------------------------------------------------------------------------------------*/

scheduler::ThreadControlBlock& Thread::getThreadControlBlock()
{
	return threadControlBlock_;
}

const scheduler::ThreadControlBlock& Thread::getThreadControlBlock() const
{
	return threadControlBlock_;
}

/*---------------------------------------------------------------------------------------------------------------------+
| protected static functions
+---------------------------------------------------------------------------------------------------------------------*/

void Thread::threadRunner(Thread& thread)
{
	thread.run();
	scheduler::getScheduler().remove(&Thread::terminationHook);

	while (1);
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

void Thread::terminationHook()
{
	joinSemaphore_.post();
	getThreadControlBlock().setList(nullptr);
}

}	// namespace distortos
