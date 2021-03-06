/**
 * \file
 * \brief Header for FLASH-related functions for STM32F4
 *
 * This file covers devices as described in following places:
 * - RM0368 reference manual (STM32F401xB/C and STM32F401xD/E), Revision 4, 2015-05-04
 * - RM0090 reference manual (STM32F405/415, STM32F407/417, STM32F427/437 and STM32F429/439), Revision 11, 2015-10-20
 * - RM0401 reference manual (STM32F410), Revision 2, 2015-10-26
 * - RM0383 reference manual (STM32F411xC/E), Revision 1, 2014-07-24
 * - RM0390 reference manual (STM32F446xx), Revision 2, 2016-01-12
 * - RM0386 reference manual (STM32F469xx and STM32F479xx), Revision 2, 2015-11-19
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_FLASH_HPP_
#define SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_FLASH_HPP_

#include "distortos/distortosConfiguration.h"

#include <cstdint>

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global constants
+---------------------------------------------------------------------------------------------------------------------*/

/// maximum allowed value of flash latency
#if defined(CONFIG_CHIP_STM32F405) || defined(CONFIG_CHIP_STM32F407) || defined(CONFIG_CHIP_STM32F415) || \
		defined(CONFIG_CHIP_STM32F417)
constexpr uint8_t maxFlashLatency {7};
#else
constexpr uint8_t maxFlashLatency {15};
#endif

/*---------------------------------------------------------------------------------------------------------------------+
| global functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Configures flash latency.
 *
 * \param [in] latency is the new flash latency, [0; maxFlashLatency]
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a latency value is invalid;
 */

int configureFlashLatency(uint8_t latency);

/**
 * \brief Configures instruction prefetch.
 *
 * \param [in] enable selects whether instruction prefetch will be disabled (false) or enabled (true)
 */

void configureInstructionPrefetch(bool enable);

/**
 * \brief Disables data cache.
 */

void disableDataCache();

/**
 * \brief Disables instruction cache.
 */

void disableInstructionCache();

/**
 * \brief Enables data cache.
 *
 * The cache is first disabled, then reset and finally enabled.
 */

void enableDataCache();

/**
 * \brief Enables instruction cache.
 *
 * The cache is first disabled, then reset and finally enabled.
 */

void enableInstructionCache();

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_FLASH_HPP_
