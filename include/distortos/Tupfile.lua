--
-- file: Tupfile.lua
--
-- author: Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
--
-- This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
-- distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
--
-- date: 2015-08-20
--

tup.rule("$(TOP)/distortosConfiguration.mk", "$(TOP)/scripts/makeDistortosConfiguration.awk %f > %o", "%B.h")