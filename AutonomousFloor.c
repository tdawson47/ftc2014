#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Sensor, S2,     IRSeeker,       sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S3,     light,          sensorLightActive)
#pragma config(Sensor, S4,     sonar,          sensorSONAR)
#pragma config(Motor,  motorA,          gripperMotor,  tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     rightMotor,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     leftMotor,     tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     armMotor,      tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     motorG,        tmotorTetrix, openLoop, encoder)

#pragma debuggerWindows("debugStream");

void driveForward(int timeRunning){
	long startTime = nSysTime;
	long currentTime = startTime;
	while(currentTime < startTime+timeRunning){
		motor[leftMotor] = 50;
		motor[rightMotor] = 50;
		currentTime = nSysTime;
	}
}

void driveFull(int timeRunning){
	long startTime = nSysTime;
	long currentTime = startTime;
	while(currentTime < startTime+timeRunning){
		motor[leftMotor] = 100;
		motor[rightMotor] = 100;
		currentTime = nSysTime;
	}
}

void driveWithin(int distanceInCm){
	int sonarVal = SensorValue(sonar);
	while(sonarVal > distanceInCm){
		motor[leftMotor] = 50;
		motor[rightMotor] = 50;
		sonarVal = SensorValue(sonar);
		//writeDebugStreamLine("the sonar value is %d", sonarVal);
	}
}

void irTurnBy(int turnBy){
	int irVal = (SensorValue(IRSeeker));
	int initialIrVal = irVal;
	while(irVal != initialIrVal+turnBy){
		motor[rightMotor] = 50;
		motor[leftMotor] = -50;
		irVal = SensorValue(IRSeeker);
	}
}

void irTurnTo(char turnDir, int turnTo){
	int irVal = (SensorValue(IRSeeker));
	if(turnDir == 'l'){
		while(irVal != turnTo){
			motor[rightMotor] = 50;
			motor[leftMotor] = -50;
			irVal = SensorValue(IRSeeker);
		}
	}
	else if(turnDir == 'r'){
		while(irVal != turnTo){
			motor[rightMotor] = -50;
			motor[leftMotor] = 50;
			irVal = SensorValue(IRSeeker);
		}
	}
}

void turnFor(char direction, int timeInMs){
	long startTime = nSysTime;
	long currentTime = startTime;
	if(direction == 'r'){
		while(currentTime < startTime+timeInMs){
			motor[leftMotor] = 50;
			motor[rightMotor] = -50;
			currentTime = nSysTime;
		}
	}
	else if(direction == 'l'){
		while(currentTime < startTime+timeInMs){
			motor[leftMotor] = -50;
			motor[rightMotor] = 50;
			currentTime = nSysTime;
		}
	}
}

void autonomous()
{
	//takes a start time for the ir mission
	long irTime = nSysTime;
	writeDebugStreamLine("the start time for IR is %d", irTime);

	int IRV = SensorValue(IRSeeker);
	writeDebugStreamLine("the ir is %d", IRV);

	driveForward(3750);
	IRV = SensorValue(IRSeeker);
	writeDebugStreamLine("the ir is %d", IRV);

	//put IRSeeker in middle
	if(SensorValue(IRSeeker) == 3){//4 irl
		//red facing
		irTurnTo('r', 2);
		driveForward(2000);
	}
	else if(SensorValue(IRSeeker) == 4){//5 irl
		//in the middle
		irTurnTo('r', 1);
		driveForward(1400);
		turnFor('l', 1200);
		wait1Msec(100);
		//irTurnTo('l', 2);
		driveForward(3000);
	}
	else if(SensorValue(IRSeeker) == 6){//6 irl
		//blue facing
		irTurnTo('r', 2);
		driveForward(3000);
		irTurnTo('l', 3);
		turnFor('r', 200);
		driveFull(2000);
	}

	//find the time taken for the ir in sec
	long irEndTime = nSysTime;
	long irTaken = (irEndTime - irTime) / 1000;
	writeDebugStreamLine("the IR time taken is %d", irTaken);
}
