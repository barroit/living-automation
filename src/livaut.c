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

#include "bus.h"
#include "sign.h"
#include "debug.h"
#include "jumper.h"
#include "run-action.h"
#include "soc/gpio_num.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "wifi.h"
#include "nvs.h"

DEFINE_ACTION_SIGNATURE(auto_control);
DEFINE_ACTION_SIGNATURE(receive_signal);

static const struct action actions[] = {
	ACTION(auto_control,   GPIO_NUM_32, GPIO_NUM_26),
	ACTION(receive_signal, GPIO_NUM_32, GPIO_NUM_25),
	ACTION_TAIL(),
};

void app_main(void)
{
	if (init_nvs_flash())
		goto setup_jumper;

	xTaskCreate(setup_wifi_task, "wifi_init", 1024, NULL, 10, NULL);

setup_jumper:
	config_jumper();

	if (install_mst_bus())
		goto do_action;

	debugging()
		bus_dev_scan_7bit();

	if (init_sign())
		uninstall_mst_bus();

do_action:
	xTaskCreate(run_action, "action_exec", 3072, (void *)actions, 15, NULL);
}
