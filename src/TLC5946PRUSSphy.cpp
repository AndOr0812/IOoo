/*
 * TLC5946phy.cpp
 *
 *  Created on: Jun 2, 2013
 *      Author: jacek
 */

#include "device/TLC5946PRUSSphy.h"
#include <errno.h>
#include <stdio.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include "debug.h"

TLC5946PRUSSphy::TLC5946PRUSSphy(SPI *spi, GPIOpin *ctrl, char *pruBinFile) :
		TLC5946phy(spi, ctrl)
{
	use_pruss = true;

	//active has been set to true by superclass constructor
	active = false;

	gsclk_pin_pin = ctrl->findPinIndex((char *) "gsclk");
	printf("prussdrv_open()\n");
	if (prussdrv_open(PRU_EVTOUT_0))
	{
		iooo_debug(0, "Cannot setup PRU_EVTOUT_0.\n");
		use_pruss = false;
		return;
	}
	iooo_debug(2, "PRUSS driver initialized");

	if (use_pruss && prussdrv_exec_program(0, pruBinFile))
	{
		iooo_debug(0, "Non-zero result from prussdrv_pruintc_init\n");
		use_pruss = false;
		return;
	}
	iooo_debug(2, "PRU binary loaded");
	prussdrv_exit();

	//setup clocking lines as outputs
	//ctrl->enableOutput(blank_pin_pin,true);
	ctrl->enableOutput(gsclk_pin_pin, true);

	active = true;
	use_pruss = true;
}

TLC5946PRUSSphy::~TLC5946PRUSSphy()
{
	iooo_debug(2, "TLC5946PRUSSphy::~TLC5946PRUSSphy()");
}

void TLC5946PRUSSphy::setBlank(uint8_t blank)
{
	if (!active /*|| (blank_pin_addr < 0)*/)
		return;
	if (use_pruss)
	{
		iooo_debug(0,
				"TLC5946phy::setBlank(): Blanking with clock generated by PRUSS not supported yet.\n");
	}
	else
	{
		TLC5946phy::setBlank(blank);
	}
}
