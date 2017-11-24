// Author: Jan Wielgus
// Date: 25.10.2017
// 

#include "Sensors.h"
SensorsClass sensors;

void dmpDataReady()
{
	sensors.mpuInterrupt = true;
}



void SensorsClass::init()
{
	Wire.begin();
	Wire.setClock(400000);
	
	mpu.initialize();
	pinMode(INTERRUPT_PIN, INPUT);
	
	devStatus = mpu.dmpInitialize();
	
	// supply your own gyro offsets here, scaled for min sensitivity
	mpu.setXGyroOffset(220);
	mpu.setYGyroOffset(76);
	mpu.setZGyroOffset(-85);
	mpu.setZAccelOffset(1788); // 1688 factory default for my test chip
	
	if (devStatus == 0)
	{
		mpu.setDMPEnabled(true);
		
		attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
		mpuIntStatus = mpu.getIntStatus();
		
		dmpReady = true;
		
		packetSize = mpu.dmpGetFIFOPacketSize();
	}
	else
	{
		// ERROR!
		// 1 = initial memory load failed
		// 2 = DMP configuration updates failed
		// (if it's going to break, usually the code will be 1)
	}
}



void SensorsClass::readAngles()
{
	// if programming failed, don't try to do anything
	if (!dmpReady) return;
	
	// wait for MPU interrupt or extra packet(s) available
	while (!mpuInterrupt && fifoCount < packetSize) {
		// other program behavior stuff here
		
		// if you are really paranoid you can frequently test in between other
		// stuff to see if mpuInterrupt is true, and if so, "break;" from the
		// while() loop to immediately process the MPU data
		
		// EWENTUANIE daæ tu break/return !!!
	}
	
	// reset interrupt flag and get INT_STATUS byte
	mpuInterrupt = false;
	mpuIntStatus = mpu.getIntStatus();
	
	// get current FIFO count
	fifoCount = mpu.getFIFOCount();
	
	// check for overflow (this should never happen unless our code is too inefficient)
	if ((mpuIntStatus & 0x10) || fifoCount == 1024)
	{
		mpu.resetFIFO();
	}
	else if (mpuIntStatus & 0x02)
	{
		// wait for correct available data length, should be a VERY short wait
		while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

		// read a packet from FIFO
		mpu.getFIFOBytes(fifoBuffer, packetSize);
		
		// track FIFO count here in case there is > 1 packet available
		// (this lets us immediately read more without waiting for an interrupt)
		fifoCount -= packetSize;
		
		//////////////////////////////////////////////////////////////////////////
		
		//mpu.dmpGetQuaternion(&q, &fifoBuffer);
		
		int16_t qI[4];
		//if (fifoBuffer == 0) fifoBuffer = mpu.dmpPacketBuffer;
		qI[0] = ((fifoBuffer[0] << 8) | fifoBuffer[1]);
		qI[1] = ((fifoBuffer[4] << 8) | fifoBuffer[5]);
		qI[2] = ((fifoBuffer[8] << 8) | fifoBuffer[9]);
		qI[3] = ((fifoBuffer[12] << 8) | fifoBuffer[13]);
		
		q.w = (float)qI[0] / 16384.0f;
		q.x = (float)qI[1] / 16384.0f;
		q.y = (float)qI[2] / 16384.0f;
		q.z = (float)qI[3] / 16384.0f;
		
		mpu.dmpGetGravity(&gravity, &q);
		mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
		
		
		//////////////////////////////////////////////////////////////////////////
		
		angle.pitch = ypr[1];
		angle.roll  = ypr[2];
		angle.yaw   = ypr[3];
	}
}





