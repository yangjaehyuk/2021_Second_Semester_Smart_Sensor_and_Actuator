#pragma config(Sensor, S1, ts, sensorEV3_Touch)
#pragma config(Sensor, S2, gs, sensorEV3_Gyro, modeEV3Gyro_RateAndAngle)
#pragma config(Sensor, S3, cs, sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S4, ss, sensorEV3_Ultrasonic)
#pragma config(Motor,  motorB, lm, tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC, rm, tmotorEV3_Large, PIDControl, driveRight, encoder)
#define Black 1
#define Blue 2
#define Green 3
#define Yellow 4
#define Red 5
#define White 6
#define Brown 7
int i, color, finish_line = -1, speed = 5, start_val = 0, count= 0, lastcolor, realcolor, dist, renew, enc_degree, initval;

void go(int speed) //go function
{
   setMotorSpeed(lm,speed);
   setMotorSpeed(rm,speed);
}

void back(int speed) //back function
{
	setMotorSpeed(lm, -speed);
	setMotorSpeed(rm, -speed);
}

int convert(int dist) //calculate degree
{
	return (int)(360.0*dist/17.58);
}

void reverseclock_pointturn(int speed) //reverseclock_pointturn function
{
	setMotorSpeed(lm, -speed);
	setMotorSpeed(rm, speed);
}

void clock_pointturn(int speed) //clock_pointturn function
{
	setMotorSpeed(lm, speed);
	setMotorSpeed(rm, -speed);
}

task main()
{
   while(getTouchValue(ts) == 0){}
   while(getTouchValue(ts) == 1){}
   finish_line = -1;

   while(1)
   {
      color = getColorName(cs);
      realcolor = color;
      clearTimer(T1);
      float t;
      renew = 0;
      enc_degree=convert(5.0);
      initval=getGyroDegrees(gs);
      while(1)
      {
         realcolor = getColorName(cs);
         if (color != realcolor){
            renew = 1;
            break;
         }
         t = time1[T1];
         if(t>250) break;
      }
      dist = getUSDistance(ss);
      if(dist < 5){
      	playSound(soundBeepBeep);
      	sleep(500);
      	resetMotorEncoder(lm);
        while(getMotorEncoder(lm)<enc_degree)
        {
          back(speed/2);
          if(-1*getMotorEncoder(lm)==enc_degree)
             break;
         }
         resetGyro(gs);
         clock_pointturn(speed/2);
         waitUntil(getGyroDegrees(gs)>90);
         clock_pointturn(0);
         resetMotorEncoder(lm);
         while(getMotorEncoder(lm)<count*enc_degree/2)
         {

           go(speed/2);
         }
         resetGyro(gs);
         clock_pointturn(speed/2);
         waitUntil(getGyroDegrees(gs)>90);
         clock_pointturn(0);
         go(speed);
         }


      go(speed);
      if(!renew){
         if(finish_line == -1 && (color == Black || color == Green || color == Yellow || color == Brown))
         {
            clearTimer(T2);
            finish_line = color;
            setLEDColor(ledGreen);
            start_val += 1;
            lastcolor = color;
         }
         if(lastcolor != finish_line && color == finish_line)
         {
            t=time1[T2];
            displayBigTextLine(1, "record=%.2f sec", t/1000);
            go(0);
            sleep(500);
            break;
         }
         if(lastcolor != Black && color == Black)
         {
            count += 1;
            lastcolor = Black;
         }
         if(lastcolor != Red && color == Red)
         {
           go(0);
           resetGyro(gs);
           reverseclock_pointturn(speed/2);
           waitUntil(abs(getGyroDegrees(gs))>70);
           reverseclock_pointturn(0);
           sleep(500);
           resetMotorEncoder(lm);
           while(getMotorEncoder(lm)<convert(count*10))
         {
           go(speed/2);
         }
           resetMotorEncoder(lm);
           resetGyro(gs);
           reverseclock_pointturn(speed/2);
           waitUntil(abs(getGyroDegrees(gs))>70);
           reverseclock_pointturn(0);
           sleep(500);
           lastcolor = color;
         }
         if(lastcolor != Blue && color == Blue)
         {
            if(count == 0){
               playSound(soundBeepBeep);
               sleep(700);
            }
            else
            {
               for(i = 0; i < count; i++)
               {
                  playSound(soundBeepBeep);
                  sleep(700);
               }
            }
            lastcolor = color;
         }

         if(color == White)
         {
            lastcolor = White;
         }
      }
   }
}
