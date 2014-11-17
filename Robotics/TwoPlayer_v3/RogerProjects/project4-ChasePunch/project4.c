/*************************************************************************/
/* File:        project4.c                                               */
/* Description: User project #4 - empty project directory for project    */
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

 	
Observation obs;
void project4_control(roger, time)
Robot* roger;
double time;
{ 
ChasePunch(roger,time);

}

/************************************************************************/
void project4_reset(roger)
Robot* roger;
{ }

// prompt for and read user customized input values
void project4_enter_params() 
{
  printf("Project 4 enter_params called. \n");
}

//function called when the 'visualize' button on the gui is pressed
void project4_visualize(roger)
Robot* roger;
{ }


void Chase(roger,time)
Robot* roger;
double time;
 {

 	 	if(stereo_observation(roger, &obs)){	
	 		if( isBallOnMySide(roger)){
		 	// 	roger->base_setpoint[X]=obs.pos[X];
				// roger->base_setpoint[Y]=obs.pos[Y];
				printf("On My Side\n");
			}
			else{
				printf("Not On My Side\n");
				roger->base_setpoint[X]=roger->base_position[X];
				roger->base_setpoint[Y]=roger->base_position[Y];
			}
		}
// printf("ball:  %f\t roger: %f \t roger set: %f\n",obs.pos[X],roger->base_position[X], roger->base_setpoint[X] );

 }

 int Punch(roger)
 Robot* roger;
 {
 	//return arm movement if is within the range.
 	return inv_arm_kinematics(roger, RIGHT, obs.pos[X], obs.pos[Y]) || inv_arm_kinematics(roger, LEFT, obs.pos[X], obs.pos[Y]);
 }

ChasePunch(roger,time)
Robot *roger;
double time;
{

 //   printf("(%f, %f)\n", time,  theta_error);
//time
	// printf("%d\t%d\n",roger->base_position[X] );
	// If the ball was found and is being tracked, then chase it.
	if(SEARCHTRACK(roger,time)==CONVERGED && !(Punch(roger))){
		Chase(roger,time);
	}
	else{
		Punch(roger);
	}
	//printf("%d\n",SEARCH() );
	/*if()
	 Punch(roger);*/
} 
int isBallOnMySide(roger)
Robot *roger;
{
	printf("base: %f\t Ball: %f\t",roger->base_position[X],obs.pos[X] );
		//check if the body of roger and the ball are in the same side.
		if(roger->base_position[X] < 0 && obs.pos[X] < 0 )
			return TRUE;
		else if(roger->base_position[X] > 0 && obs.pos[X] > 0 )
			return TRUE;
		else
			return FALSE;
}