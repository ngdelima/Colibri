#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

// Forward declarations

class Communication;

#include"Drone.h"

#define CONFIG_EXAMPLE_TASK_STACK_SIZE 2048
#define TX_PIN (UART_PIN_NO_CHANGE)
#define RX_PIN (UART_PIN_NO_CHANGE)
#define RTS_PIN (UART_PIN_NO_CHANGE)
#define CTS_PIN (UART_PIN_NO_CHANGE)

#define UART_PORT_NUM      (UART_NUM_0)
#define UART_BAUD_RATE     (115200)
#define TASK_STACK_SIZE    (CONFIG_EXAMPLE_TASK_STACK_SIZE)

#define BUF_SIZE (1024)
static const int RX_BUF_SIZE = 1024;
static const int TX_BUF_SIZE = 1024;

typedef int comms_error;
#define COMMS_OK 0
#define COMMS_FAIL -1
#define COMMS_ERROR_NOT_READY 1

enum class COMMS_STATE
{
	COMMS_STATE_READY = 0,
	COMMS_STATE_NOT_READY,
	COMMS_STATE_RUNNING,
};

class Communication
{

public:

	Communication()
	: mDrone(nullptr)
	, mTxPriority(0)
	, mRxPriority(0)
	, mState(COMMS_STATE::COMMS_STATE_NOT_READY)
	{}

	comms_error initialize(Drone* _drone, int txPriority, int rxPriority);
	comms_error run();
	static void echo_task(void *arg);
	void send(const char *data, int lenght);

private:

	static void txTask(void *arg);
	static void rxTask(void *arg);

	Drone* mDrone;
	int mTxPriority;
	int mRxPriority;
	COMMS_STATE mState;
};



#endif
