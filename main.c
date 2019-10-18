#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <main.h>
#include "leds.h"
#include "serial_comm.h"

static THD_WORKING_AREA(comm_thd_wa, 512);
static THD_FUNCTION(comm_thd, arg) {
    (void) arg;
    chRegSetThreadName(__FUNCTION__);
    uint8_t recvBuff = 0;

    while(1) {
    	chSequentialStreamRead(&SD3, &recvBuff, 1); // Blocking read
    	//chnReadTimeout(&SD3, &recvBuff, 1, MS2ST(10)); // Non-blocking read (timeout 10 ms)
    	set_body_led(2);
    	chSequentialStreamWrite(&SD3, &recvBuff, 1); // Blocking write
    	//chnWriteTimeout(&SD3, &recvBuff, 1, MS2ST(10)); // Non-blocking write (timeout 10 ms)
    }
}



int main(void)
{

    halInit();
    chSysInit();
    mpu_init();

    serial_start();

    chThdCreateStatic(comm_thd_wa, sizeof(comm_thd_wa), NORMALPRIO, comm_thd, NULL);

    /* Infinite loop. */
    while (1) {
    	//waits 1 second
        chThdSleepMilliseconds(1000);
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
