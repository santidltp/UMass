/*************************************************************************/
/* File:        project6.c                                               */
/* Description: User project #6                                          */
/* Date:        01-29-2011                                               */
/*************************************************************************/
#include <math.h>
#include <stdio.h>
#include "Xkw/Xkw.h"

#include "roger.h"
#include "simulate.h"
#include "control.h"
#include "modes.h"

Observation obs;
void project6_control(roger, time)
Robot* roger;
double time;
{
Pong(roger,time);
}

void project6_reset(roger)
Robot* roger;
{ }

// prompt for and read user customized input values
void project6_enter_params()
{
  printf("Project 6 enter_params called. \n");
}

/* do not alter */
//function called when the 'visualize' button on the gui is pressed
void project6_visualize(roger)
Robot* roger;
{ }
RETREAT(roger,time)
Robot* roger;
double time;
{
	//PUNCH(roger);
	

}

 void Pong(roger, time)
Robot *roger;
double time;
{

	if(SEARCHTRACK(roger,time)==CONVERGED && !Punch(roger)  ){
		Chase(roger,time);
	}
	else{
		Punch(roger);
			//stop_Wheels(roger);


		}

	
	
}


/*void arms_Retreat(roger)
Robot *roger;
{
	roger->arm_setpoint[LEFT][0]=1.439897;
	roger->arm_setpoint[LEFT][1]=-2.617994;
	roger->arm_setpoint[RIGHT][0]=-1.439897;
	roger->arm_setpoint[RIGHT][1]=2.617994;
}
int isBallOnMySide(roger)
Robot *roger;
{
	// printf("base: %f\t Ball: %f\t",roger->base_position[X],obs.pos[X] );
		//check if the body of roger and the ball are in the same side.
		if(roger->base_position[X] < 0 && obs.pos[X] < -1.0 )
			return TRUE;
		else if(roger->base_position[X] > 0 && obs.pos[X] > 1.0 )
			return TRUE;
		else{
			arms_Retreat(roger);
			return FALSE;
		}
}
/* int Punch(roger)
 Robot* roger;
 {
 	//return arm movement if is within the range.
 	return inv_arm_kinematics(roger, RIGHT, obs.pos[X], obs.pos[Y]) && inv_arm_kinematics(roger, LEFT, obs.pos[X], obs.pos[Y]);
 }
 /*void Chase(roger,time)
Robot* roger;
double time;
 {

 	 	if(stereo_observation(roger, &obs)){	
	 		if( isBallOnMySide(roger)){
		 		roger->base_setpoint[X]=obs.pos[X];
				roger->base_setpoint[Y]=obs.pos[Y];
				printf("On My Side\n");
			}
			else{
				printf("Not On My Side\n");
				roger->base_setpoint[X]=roger->base_position[X];
				roger->base_setpoint[Y]=roger->base_position[Y];
			}
		}
printf("setpoint  %f\n", roger->base_setpoint[X] );

 }*/