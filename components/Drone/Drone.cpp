#include "Drone.h"

void Drone::initialize()
{
	mCommandQueue = xQueueCreate(COMMAND_QUEUE_SIZE, COMMAND_MAX_SIZE);
	if(mCommandQueue == NULL)
	{
		// Error
		return;
	}

	// Initialize Communications module
	mCommunication = new Communication;
	if(mCommunication == nullptr)
	{
		return;
	}
	mCommunication->initialize(this, PRIORITY_COMMUNICATION_TX,PRIORITY_COMMUNICATION_RX);
	//xTaskCreate(commandDispatcher, "command_dispatcher", 1024*2, this, PRIORITY_COMMAND_DISPATCHER, NULL);

	// Initialize Control module4
	mControl = new Control();
	if(mControl == nullptr)
	{
		return;
	}
	mControl->initialize();
}

void Drone::run()
{
	mCommunication->run();
}

void Drone::addCommandToQueue(uint8_t* data, int lenght)
{
	// TODO: if command dispatcher is busy, add to the queue
	//xQueueSend(communication->mCommandQueue, (void *)&command, 10);

	executeCommand(data, lenght);
}

void Drone::executeCommand(uint8_t* command, int lenght)
{
	if(lenght > 0)
	{
		switch(command[0])
		{
			case COMMAND_START_DRONE:
			{
				const char data[13] = "Start drone\n";
				int len = 12;
				mCommunication->send((const char*)data, len);
				break;
			}
			case COMMAND_STOP_DRONE:
			{
				const char data[12] = "Stop drone\n";
				int len = 11;
				mCommunication->send((const char*)data, len);
				break;
			}
			case COMMAND_SET_MOTOR_SPEED:
			{
				if(lenght == 4)
				{
					//const char data[17] = "Set motor speed\n";
					//int len = 16;
					char msg[26];
					sprintf(msg, "Set motor %u speed: %u\n", command[1], command[2]);
					mCommunication->send(msg, 25);
					// Set motor speed
					mControl->setMotorSpeedCmd(command);
				}
				break;
			}
			default:
			{
				// Could not recognize command
				const char data[16] = "Not recognized\n";
				int len = 15;
				mCommunication->send((const char*)data, len);
				break;
			}
		}
	}
}

void Drone::commandDispatcher(void* arg)
{
	if(arg != nullptr)
	{
		/*Drone* drone = (Drone*)arg;
		const TickType_t xDelay = 100 / portTICK_PERIOD_MS;
		for(;;)
		{
			Command command;
			while(xQueueReceive(drone->mCommandQueue, (void *)&command, 0) == pdTRUE)
			{
				//drone->executeCommand(command);
			}
			vTaskDelay(xDelay);
		}*/
	}
}

