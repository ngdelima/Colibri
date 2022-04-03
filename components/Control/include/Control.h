#ifndef CONTROL_H
#define CONTROL_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"

#include"Commands.h"

#define MOTOR_1 0
#define MOTOR_2 1
#define MOTOR_3 2
#define MOTOR_4 3
#define MOTOR_COUNT 4

#define PWM_FREQUENCY 50

#define MOTOR1_PIN 15
#define MOTOR2_PIN 15 // Find other pwm port
#define MOTOR3_PIN 15 // Find other pwm port
#define MOTOR4_PIN 15 // Find other pwm port

class Motor
{
public:

	void initialize(int _motorPin)
	{
		mMotorPin = _motorPin;
	    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, mMotorPin);

	    mcpwm_config_t pwm_config;
	    pwm_config.frequency = PWM_FREQUENCY;
	    pwm_config.cmpr_a = 0.0;
	    pwm_config.counter_mode = MCPWM_UP_COUNTER;
	    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;

	    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);
	    // TODO: Find out why 1000 and move ot to a constant
	    mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_GEN_A, 1000);
	}

	void updateDutyCycle(float dutyCycle)
	{
		mDutyCycle = dutyCycle;
		mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_GEN_A, dutyCycle);
		vTaskDelay(pdMS_TO_TICKS(500));
		// Delay is added in caller function
	}

private:

	float mDutyCycle;
	int mMotorPin;

};


class Control
{
public:

	void initialize()
	{
		mMotors[MOTOR_1].initialize(MOTOR1_PIN);
	    vTaskDelay(pdMS_TO_TICKS(500));
		//mMotors[MOTOR_2].initialize(MOTOR2_PIN);
		//mMotors[MOTOR_3].initialize(MOTOR3_PIN);
		//mMotors[MOTOR_4].initialize(MOTOR4_PIN);
	}

	void setMotorSpeedCmd(uint8_t* cmd)
	{
		if( cmd != nullptr &&
			cmd[COMMAND_ID_POSITION] == COMMAND_SET_MOTOR_SPEED)
		{
			//int motorId = cmd[1];
			int motorId = MOTOR_1;
			float dutyCycle = 5.f;

			mMotors[motorId].updateDutyCycle(dutyCycle);
		}
	}

private:

	Motor mMotors[MOTOR_COUNT];
};

#endif
