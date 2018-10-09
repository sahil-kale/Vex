#pragma config(Sensor, in1,    linetrackerl,   sensorLineFollower)
#pragma config(Sensor, in2,    linetrackerc,   sensorLineFollower)
#pragma config(Sensor, in3,    linetrackerr,   sensorLineFollower)
#pragma config(Motor,  port2,           rightMotor,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           leftMotor,     tmotorVex393_MC29, openLoop, driveLeft)
#pragma config(Motor,  port4,           clawAngleMotor, tmotorVex393_MC29, openLoop, driveRight)
#pragma config(Motor,  port5,           grabMotor,     tmotorVex393_MC29, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

void teleopFunc()
{
	bool clawUpSet = true;

		motor[leftMotor] = vexRT[Ch3];
		motor[rightMotor] = vexRT[Ch2];


		if(vexRT[Btn5U]==1)
		{
			motor[clawAngleMotor] = 127;
		}
		else if(vexRT[Btn5D]==1)
		{
			motor[clawAngleMotor] = -127;
		}
		else
		{
			if(clawUpSet == true)
			{
				motor[clawAngleMotor] = 17;
			}
			else
			{
				motor[clawAngleMotor] = 0;
			}
		}

		if(vexRT[Btn8L]==1)
		{
			if(clawUpSet)
			{
				clawUpSet = false;
			}
			else
			{
				clawUpSet = true;
			}
		}

		if(vexRT[Btn6U]==1)
		{
			motor[grabMotor] = 127;
		}
		else if(vexRT[Btn6D]==1)
		{
			motor[grabMotor] = -127;
		}
		else
		{
			motor[grabMotor] = 0;
		}
}

void lineTrackerFunc()
{
	float speed = 30; //Speed to travel during the line following sequence
	float scale = -0.1; //Used to determine speed of the other motor side, determined with a scale from the speed float
	float thresholdL = 2800; //Calibration values for sensors. These WILL change as dependent on lighting, surface, etc.
	float thresholdC = 2850; //A higher value indicates a darker surface
	float thresholdR = 2885; //max value is 4095

	/*
	if(SensorValue[linetrackerr] < thresholdR && SensorValue[linetrackerl] < thresholdL && SensorValue[linetrackerc] < thresholdC)
	{
	//just some code to re-align. This can safely be ignored.
		motor[leftMotor] = speed;
		motor[rightMotor] = -speed;
	}
	else*/ if(SensorValue[linetrackerr] <= thresholdR || SensorValue[linetrackerl] <= thresholdL) //If these sensors see white
	{

		if(SensorValue[linetrackerr] <= thresholdR) //turn to the left since the right sensor is no longer on the line
		{
			motor[leftMotor] = speed * scale;
			motor[rightMotor] = speed;
		}
		else if(SensorValue[linetrackerl] <= thresholdL)
		{
			motor[leftMotor] = speed;
			motor[rightMotor] = speed * scale;
		}

	}

	if(SensorValue[linetrackerc] >= thresholdC) //If all are on track, proceed normally.
	{
		motor[leftMotor] = speed;
		motor[rightMotor] = speed;
	}
}

void timerLine(float time) //simple timer function, takes seconds
{
	time = time * 1000; //converts from seconds to milliseconds
	float timeElapsed = 0;

	while(timeElapsed <= time) //while loop for code
	{
		lineTrackerFunc(); //calls line function
		timeElapsed += 1; //adds 1 to timeElapsed
		wait1Msec(1); //waits 1MS before executing again. Once time elapsed > time, the function will stop.
	}
}


task main()
{

	while(true) //teleop code
	{
		teleopFunc();
		if(vexRT[Btn7R]==1)
		{
			timerLine(15); //<------------------------------------------------------------------------------------
			//Function is called for autonomous with time being allocated to 15.
		}

	}
}
