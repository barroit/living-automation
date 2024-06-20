/****************************************************************************
**
** Copyright 2024 Jiamu Sun
** Contact: barroit@linux.com
**
** This file is part of livaut.
**
** livaut is free software: you can redistribute it and/or modify it
** under the terms of the GNU General Public License as published by the
** Free Software Foundation, either version 3 of the License, or (at your
** option) any later version.
**
** livaut is distributed in the hope that it will be useful, but
** WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** General Public License for more details.
**
** You should have received a copy of the GNU General Public License along
** with livaut. If not, see <https://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "helper.h"
#include "i2c-bus.h"
#include "sign.h"

/* just for fun ;) */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
void fn(void)
{
	u8 sign = 0xAA;

	while (true) {
		show_sign((sign = ~sign));
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void app_main(void)
{
	if (ROE_ESP(init_mst_bus(), "i2c_bus"))
		goto schedule_task;

	if (ROE_ESP(init_sign(), "sign_init"))
		dsty_mst_bus();

schedule_task:
	fn();
}
