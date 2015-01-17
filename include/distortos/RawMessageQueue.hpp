/**
 * \file
 * \brief RawMessageQueue class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-01-17
 */

#ifndef INCLUDE_DISTORTOS_RAWMESSAGEQUEUE_HPP_
#define INCLUDE_DISTORTOS_RAWMESSAGEQUEUE_HPP_

#include "distortos/synchronization/MessageQueueBase.hpp"

namespace distortos
{

/**
 * \brief RawMessageQueue class is very similar to MessageQueue, but optimized for binary serializable types (like POD
 * types). Type T can be used with both RawMessageQueue and MessageQueue<T> only when
 * std::is_trivially_copyable<T>::value == true, otherwise only MessageQueue<T> use is safe, while using RawMessageQueue
 * results in undefined behavior.
 *
 * Similar to POSIX mqd_t - http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/mqueue.h.html
 */

class RawMessageQueue
{
public:

	/// type of uninitialized storage for Entry with link
	using EntryStorage = synchronization::MessageQueueBase::EntryStorage;

	/**
	 * \brief RawMessageQueue's constructor
	 *
	 * \param [in] entryStorage is an array of EntryStorage elements
	 * \param [in] valueStorage is a memory block for elements, sufficiently large for \a maxElements, each
	 * \a elementSize bytes long
	 * \param [in] elementSize is the size of single queue element, bytes
	 * \param [in] maxElements is the number of elements in \a entryStorage array and \a valueStorage memory block
	 */

	RawMessageQueue(EntryStorage* const entryStorage, void* const valueStorage, const size_t elementSize,
			const size_t maxElements) :
			messageQueueBase_{entryStorage, valueStorage, elementSize, maxElements},
			elementSize_{elementSize}
	{

	}

	/**
	 * \brief RawMessageQueue's constructor
	 *
	 * \param T is the type of data in queue
	 * \param N is the number of elements in \a entryStorage array and \a valueStorage memory block
	 *
	 * \param [in] entryStorage is a reference to an array of \a N EntryStorage elements
	 * \param [in] valueStorage is a reference to array that will be used as storage for \a N elements, each sizeof(T)
	 * bytes long
	 */

	template<typename T, size_t N>
	RawMessageQueue(EntryStorage (& entryStorage)[N], T (& valueStorage)[N]) :
			RawMessageQueue{entryStorage, valueStorage, sizeof(*valueStorage),
					sizeof(valueStorage) / sizeof(*valueStorage)}
	{

	}

	/**
	 * \brief RawMessageQueue's constructor
	 *
	 * \param T is the type of data in queue
	 * \param N is the number of elements in \a entryStorage array and \a valueStorage memory block
	 *
	 * \param [in] entryStorage is a reference to an std::array of \a N EntryStorage elements
	 * \param [in] valueStorage is a reference to std::array that will be used as storage for \a N elements, each
	 * sizeof(T) bytes long
	 */

	template<typename T, size_t N>
	RawMessageQueue(std::array<EntryStorage, N>& entryStorage, std::array<T, N>& valueStorage) :
			RawMessageQueue{entryStorage.data(), valueStorage.data(), sizeof(*valueStorage.data()), valueStorage.size()}
	{

	}

	/**
	 * \brief Pops oldest element with highest priority from the queue.
	 *
	 * Similar to mq_receive() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html#
	 *
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] buffer is a pointer to buffer for popped element
	 * \param [in] size is the size of \a buffer, bytes - must be equal to the \a elementSize attribute of
	 * RawMessageQueue
	 *
	 * \return zero if element was popped successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	int pop(uint8_t& priority, void* buffer, size_t size);

	/**
	 * \brief Pushes the element to the queue.
	 *
	 * Similar to mq_send() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \param [in] priority is the priority of new element
	 * \param [in] data is a pointer to data that will be pushed to RawMessageQueue
	 * \param [in] size is the size of \a data, bytes - must be equal to the \a elementSize attribute of RawMessageQueue
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	int push(uint8_t priority, const void* data, size_t size);

	/**
	 * \brief Pushes the element to the queue.
	 *
	 * Similar to mq_send() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \param T is the type of data pushed to the queue
	 *
	 * \param [in] priority is the priority of new element
	 * \param [in] data is a reference to data that will be pushed to RawMessageQueue
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - EMSGSIZE - sizeof(T) doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename T>
	int push(const uint8_t priority, const T& data)
	{
		return push(priority, &data, sizeof(data));
	}

private:

	/**
	 * \brief Pops oldest element with highest priority from the queue.
	 *
	 * Internal version - builds the Functor object.
	 *
	 * \param [in] waitSemaphoreFunctor is a reference to SemaphoreFunctor which will be executed with \a popSemaphore_
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] buffer is a pointer to buffer for popped element
	 * \param [in] size is the size of \a buffer, bytes - must be equal to the \a elementSize attribute of
	 * RawMessageQueue
	 *
	 * \return zero if element was popped successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	int popInternal(const synchronization::SemaphoreFunctor& waitSemaphoreFunctor, uint8_t& priority, void* buffer,
			size_t size);

	/**
	 * \brief Pushes the element to the queue.
	 *
	 * Internal version - builds the Functor object.
	 *
	 * \param [in] waitSemaphoreFunctor is a reference to SemaphoreFunctor which will be executed with \a pushSemaphore_
	 * \param [in] priority is the priority of new element
	 * \param [in] data is a pointer to data that will be pushed to RawMessageQueue
	 * \param [in] size is the size of \a data, bytes - must be equal to the \a elementSize attribute of RawMessageQueue
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	int pushInternal(const synchronization::SemaphoreFunctor& waitSemaphoreFunctor, uint8_t priority, const void* data,
			size_t size);

	/// contained synchronization::MessageQueueBase object which implements base functionality
	synchronization::MessageQueueBase messageQueueBase_;

	/// size of single queue element, bytes
	const size_t elementSize_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_RAWMESSAGEQUEUE_HPP_