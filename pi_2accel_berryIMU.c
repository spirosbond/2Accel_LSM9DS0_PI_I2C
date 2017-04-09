/*
	This program  reads the angles from the accelerometer and gyroscope
	on a BerryIMU connected to a Raspberry Pi.
	http://ozzmaker.com/


    Copyright (C) 2014  Mark Williams

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Library General Public License for more details.
    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
    MA 02111-1307, USA
*/



#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include "sensor.c"


#define DT 0.001         // [s/loop] loop period. 1ms
#define AA 0.97         // complementary filter constant

#define A_GAIN 0.0573      // [deg/LSB]
#define G_GAIN 0.070     // [deg/s/LSB]
#define RAD_TO_DEG 57.29578
#define M_PI 3.14159265358979323846

#define ACC_ADDR_1 0x1e
#define ACC_ADDR_2 0x1d
#define MAG_ADDR_1 0x1e
#define MAG_ADDR_2 0x1d
#define GYR_ADDR_1 0x6a
#define GYR_ADDR_2 0x6b

FILE * fp;

void  INThandler(int sig)
{
        signal(sig, SIG_IGN);
	fclose(fp);
        exit(0);
}

int mymillis()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec) * 1000 + (tv.tv_usec)/1000;
}

int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;
    return (diff<0);
}

int main(int argc, char *argv[])
{

	float rate_gyr_y = 0.0;   // [deg/s]
	float rate_gyr_x = 0.0;    // [deg/s]
	float rate_gyr_z = 0.0;     // [deg/s]

	int  acc1Raw[3];
	int  acc2Raw[3];
	int  mag1Raw[3];
	int  mag2Raw[3];
	int  gyr1Raw[3];
	int  gyr2Raw[3];



	float gyroXangle = 0.0;
	float gyroYangle = 0.0;
	float gyroZangle = 0.0;
	float AccYangle = 0.0;
	float AccXangle = 0.0;
	float CFangleX = 0.0;
	float CFangleY = 0.0;

	int startInt  = mymillis();
	struct  timeval tvBegin, tvEnd,tvDiff;


        signal(SIGINT, INThandler);

	enableIMU(ACC_ADDR_1, MAG_ADDR_1, GYR_ADDR_1);
	enableIMU(ACC_ADDR_2, MAG_ADDR_2, GYR_ADDR_2);

	gettimeofday(&tvBegin, NULL);

	fp = fopen (strcat(argv[1],".csv"), "w+");

	while(1)
	{
		startInt = mymillis();


		//read ACC and GYR data
		readACC(acc1Raw,ACC_ADDR_1);
		readACC(acc2Raw,ACC_ADDR_2);
		fprintf (fp, "Acc1Xraw,%d,Acc1Yraw,%d,Acc1Zraw,%d,Acc2Xraw,%d,Acc2Yraw,%d,Acc2Zraw,%d,",acc1Raw[0],acc1Raw[1],acc1Raw[2],acc2Raw[0],acc2Raw[1],acc2Raw[2]);

	        while(mymillis() - startInt < (DT*1000))
	        {
	            usleep(100);
	        }

		fprintf(fp, "Loop Time,%d\n", mymillis()- startInt);
	}

	fclose(fp);
}

