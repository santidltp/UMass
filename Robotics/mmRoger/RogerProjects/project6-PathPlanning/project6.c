/*************************************************************************/
/* File:        project6.c                                               */
/* Description: User project #6 - empty project directory for project    */
/*              developement                                             */
/* Date:        03-30-2013                                               */
/*************************************************************************/
#include <math.h>
#include <stdio.h>
#include "Xkw/Xkw.h"
#include "roger.h"
#include "simulate.h"
#include "control.h"
#include "modes.h"

double Xinitial = 0;
	double Xfinal   = 0;
	double initialTime=0;
	double finalTime=0;
	double speed=0;
	double balltime=0;

void project6_control(roger, time)
Robot* roger;
double time;
{ 
game(roger, time);

}

/************************************************************************/
void project6_reset(roger)
Robot* roger;
{ }

// prompt for and read user customized input values
void project6_enter_params() 
{
  printf("Project 4 enter_params called. \n");
}

//function called when the 'visualize' button on the gui is pressed
void project6_visualize(roger)
Robot* roger;
{ }


void game(roger, time)
Robot *roger;
double time;
{

Observation ball;
// TRACK();


// 	if(stereo_observationWithRespect(roger, &ball)){
// 		//ballSpeed(ball,time);
// 	//	isBallTrapped(roger,ball,time);
// 		if(ball.pos[X] < 0.4){
// 			armsTrapBallClose(roger);
// 		// 	if(isBallTrapped(roger,ball,time))
// 		// 		armsTrapBallOpen(roger);
// 		// }
// 		else{
// 				armsTrapBallOpen(roger);			
// 			}
    





// }
// else{
// 	armsTrapBallOpen(roger);

// }





}
void armsTrapBallOpen(roger)
Robot *roger;
{
	roger->arm_setpoint[LEFT][0]	=   1.526108;
	roger->arm_setpoint[LEFT][1]	=  -2.143611;
	roger->arm_setpoint[RIGHT][0]	=  -1.526108;
	roger->arm_setpoint[RIGHT][1]	=	2.143611;
}
void armsTrapBallClose(roger)
Robot *roger;
{
	roger->arm_setpoint[LEFT][0]	=   1.310810;
	roger->arm_setpoint[LEFT][1]	=  -2.400305;
	roger->arm_setpoint[RIGHT][0]	=  -1.182136;
	roger->arm_setpoint[RIGHT][1]	=	2.228120;
}





void ballSpeed(Observation ball, double time){
if(initialTime == 0){
 	initialTime=time;
 	Xinitial = ball.pos[X];
 	// printf("initial time:%f\n", initialTime);
 }
 else if(time >= (initialTime+ 0.01)){
 	Xfinal = ball.pos[X];
 	finalTime=time;
 	speed = (Xinitial-Xfinal)/(initialTime-finalTime);
 	initialTime =0;
 	printf("speeeeeeed: %f\t time: %f\n",speed,time ); 
 	// return speed;
 }

// return 0;
}

int isBallTrapped(Robot *roger, Observation ball, double time){


	if(ball.pos[X] < 0.26){
		
		return 1;
	}

	return 0;
}