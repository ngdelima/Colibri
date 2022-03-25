#ifndef DRONE_H
#define DRONE_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

// Forward declarations

class Drone;

#include"Commands.h"
#include"Communication.h"
#include"Control.h"

#define PRIORITY_COMMAND_DISPATCHER 	configMAX_PRIORITIES
#define PRIORITY_COMMUNICATION_TX 		PRIORITY_COMMAND_DISPATCHER
#define PRIORITY_COMMUNICATION_RX 		PRIORITY_COMMAND_DISPATCHER

#define COMMAND_QUEUE_SIZE 20

class Drone
{

public:

	Drone()
	: mCommandQueue(NULL)
	, mCommunication(nullptr)
	, mControl(nullptr)
	{}

	void initialize();
	void run();

	void addCommandToQueue(uint8_t* command, int lenght);

private:

	static void commandDispatcher(void* arg);
	void executeCommand(uint8_t* command, int lenght);

	QueueHandle_t mCommandQueue;
	Communication* mCommunication;
	Control* mControl;
};

#endif
