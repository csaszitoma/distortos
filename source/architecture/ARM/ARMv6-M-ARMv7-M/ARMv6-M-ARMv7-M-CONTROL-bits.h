/**
 * \file
 * \brief Header with definition of bits in CONTROL register in ARMv6-M and ARMv7-M
 *
 * \author Copyright (C) 2014-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_ARCHITECTURE_ARM_ARMV6_M_ARMV7_M_ARMV6_M_ARMV7_M_CONTROL_BITS_H_
#define SOURCE_ARCHITECTURE_ARM_ARMV6_M_ARMV7_M_ARMV6_M_ARMV7_M_CONTROL_BITS_H_

/*---------------------------------------------------------------------------------------------------------------------+
| CONTROL - The special-purpose control register
+---------------------------------------------------------------------------------------------------------------------*/

#define CONTROL_nPRIV_bit					0
#define CONTROL_SPSEL_bit					1

#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)

#define CONTROL_FPCA_bit					2

#endif	/* defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__) */

#define CONTROL_nPRIV						(1 << CONTROL_nPRIV_bit)
#define CONTROL_SPSEL						(1 << CONTROL_SPSEL_bit)

#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)

#define CONTROL_FPCA						(1 << CONTROL_FPCA_bit)

#endif	/* defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__) */

#endif	/* SOURCE_ARCHITECTURE_ARM_ARMV6_M_ARMV7_M_ARMV6_M_ARMV7_M_CONTROL_BITS_H_ */
