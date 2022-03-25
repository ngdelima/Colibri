#include "Communication.h"

comms_error Communication::initialize(Drone* _drone, int txPriority, int rxPriority)
{
	mDrone = _drone;

	mTxPriority = txPriority;
	mRxPriority = rxPriority;

	uart_config_t uart_config;
	{
		uart_config.baud_rate = UART_BAUD_RATE;
		uart_config.data_bits = UART_DATA_8_BITS;
		uart_config.parity    = UART_PARITY_DISABLE;
		uart_config.stop_bits = UART_STOP_BITS_1;
		uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
		uart_config.source_clk = UART_SCLK_APB;
		uart_config.rx_flow_ctrl_thresh  = 64;
	}

	esp_err_t install_error = uart_driver_install(UART_PORT_NUM, RX_BUF_SIZE, TX_BUF_SIZE, 0, NULL, 0);
	esp_err_t config_error = uart_param_config(UART_PORT_NUM, &uart_config);
	esp_err_t set_pin_error = uart_set_pin(UART_PORT_NUM, TX_PIN, RX_PIN, RTS_PIN, CTS_PIN);


	if(install_error == ESP_OK &&
		config_error == ESP_OK &&
		set_pin_error ==  ESP_OK &&
		mDrone != nullptr)
	{
		mState = COMMS_STATE::COMMS_STATE_READY;
		return COMMS_OK;
	}
	return COMMS_FAIL;
}

void Communication::send(const char *data, int lenght)
{
	// TODO: Check if is currently writing and add to queue if so
	uart_write_bytes(UART_PORT_NUM, data, lenght);
}

void Communication::echo_task(void *arg)
{
	if(arg != nullptr)
	{
		Drone* drone = (Drone*)arg;

		// Configure a temporary buffer for the incoming data
		uint8_t *data = (uint8_t *) malloc(BUF_SIZE);

		while(true)
		{
			// Read data from the UART
			int lenght = uart_read_bytes(UART_PORT_NUM, data, BUF_SIZE, 20 / portTICK_RATE_MS);

			// TODO: remove following code and put it in the rxTask
			drone->addCommandToQueue(data, lenght);
			//char response[5] = "ACK\n";
			// Write data back to the UART
			//uart_write_bytes(UART_PORT_NUM, (const char *) response, 4);
			//uart_write_bytes(UART_PORT_NUM, (const char *)data, lenght);
		}
	}
}

comms_error Communication::run()
{
	if (mState == COMMS_STATE::COMMS_STATE_READY)
	{
		xTaskCreate(echo_task, "uart_echo_task", TASK_STACK_SIZE, mDrone, 10, NULL);
	    //xTaskCreate(rxTask, "uart_rx_task", 1024*2, this, mRxPriority, NULL);
	    //xTaskCreate(txTask, "uart_tx_task", 1024*2, this, mTxPriority, NULL);

		mState = COMMS_STATE::COMMS_STATE_RUNNING;
		return COMMS_OK;
	}
	else
	{
		return COMMS_ERROR_NOT_READY;
	}
}

void Communication::txTask(void *arg)
{
	char c[] = "0\n\0";
    while (1)
    {
        uart_write_bytes(UART_PORT_NUM, &c, 3);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        c[0]++;
    }
}

void Communication::rxTask(void *arg)
{
	if(arg != nullptr)
	{
		Communication* communication = (Communication*)arg;
		//Command* command = (Command*)malloc(sizeof(Command));
		//Command command;
		while (1)
		{
			//const int rxBytes = uart_read_bytes(UART_PORT_NUM, &command, sizeof(Command), 1000 / portTICK_RATE_MS);
			//if (rxBytes == COMMAND_MAX_SIZE)
			{
				//xQueueSend(communication->mCommandQueue, (void *)&command, 10);
			}
		}
	}
	else
	{
		// Need to implement a mechanism to inform this error (maybe just send this as parameter)
	}
}
