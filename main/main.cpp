#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include"Communication.h"
#include"Drone.h"

extern "C" void app_main(void)
{
	Drone drone;
    drone.initialize();
    drone.run();
}
