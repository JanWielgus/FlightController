// Author: Jan Wielgus
// Date: 25.10.2017
// 

#include "Motors.h"
MotorsClass motors;



void MotorsClass::init()
{
	mTL.attach(TLmotorPin); // Top left
	mTR.attach(TRmotorPin); // Top right
	mBL.attach(BLmotorPin); // Back left
	mBR.attach(BRmotorPin); // Back right
}



void MotorsClass::setOnAllMotors(uint16_t _val)
{
	// Zostawiæ to pierwsze (i przetestowaæ)

	_val = map(_val, 0, 2000, 1000, 2000);
	_val = constrain(_val, 1000, 2000);
	//motor->writeMicroseconds(_val);
	mTL.writeMicroseconds(_val);
	mTR.writeMicroseconds(_val);
	mBL.writeMicroseconds(_val);
	mBR.writeMicroseconds(_val);

	
	//_val = map(_val, 0, 2000, 0, 180);
	//_val - constrain(_val, 0, 180);
	
	//mTL.write(_val);
	//mTR.write(_val);
	//mBL.write(_val);
	//mBR.write(_val);
}



