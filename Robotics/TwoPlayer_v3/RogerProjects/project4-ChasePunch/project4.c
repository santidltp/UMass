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
  // printf("Project 4 enter_params called. \n");
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
 	 		// printf("%f\n",obs.pos[Y]);
	 		if( isBallOnMySide(roger)){
		 		roger->base_setpoint[X]=obs.pos[X];
				roger->base_setpoint[Y]=obs.pos[Y];
				// printf("On My Side\n");
			}
			else{
				// printf("Not On My Side\n");
				roger->base_setpoint[X]=-3.8;
				roger->base_setpoint[Y]=0;
				roger->base_setpoint[THETA]=0 ;
			}
		}
// printf("setpoint  %f\n", roger->base_setpoint[X] );

 }

 int Punch(roger)
 Robot* roger;
 {
 	//return arm movement if is within the range.
 	if(obs.pos[Y]<-1.84 || obs.pos[Y]> 1.84)return FALSE;
 	if(obs.pos[Y]  < 0)
 		return inv_arm_kinematics(roger, RIGHT, obs.pos[X], obs.pos[Y]) ;
 	else
		return inv_arm_kinematics(roger, LEFT, obs.pos[X], obs.pos[Y]) ;

}


ChasePunch(roger,time)
Robot *roger;
double time;
{
double setTimer=0;
 //   printf("(%f, %f)\n", time,  theta_error);
//time
	// printf("%d\t%d\n",roger->base_position[X] );
	// If the ball was found and is being tracked, then chase it.
	if(SEARCHTRACK(roger,time)==CONVERGED && !Punch(roger)  ){
		Chase(roger,time);
		printf("(%f,%f)\t(%f,%f)\t(%f,%f)\n",time, roger->base_position[THETA],time, roger->base_position[X],time,roger->base_position[Y]);
	}
	else{

		Punch(roger);
		setTimer=time;

			//stop_Wheels(roger);


		}
		//keep a short punch to avoid arms wiggiling around.
		if(time == (setTimer + 0.1))
		arms_Retreat(roger);

		
	// printf("arma %f\n", roger->arm_theta[RIGHT][1] );


	//printf("%d\n",SEARCH() );
	/*if()
	 Punch(roger);*/
		// printf("base setpoint %f \n" , roger->base_setpoint[THETA]);

} 
int isBallOnMySide(roger)
Robot *roger;
{
	// printf("base: %f\t Ball: %f\t",roger->base_position[X],obs.pos[X] );
		//check if the body of roger and the ball are in the same side.
		if(roger->base_position[X] < 0 && obs.pos[X] < -0.5)
			return TRUE;
		else if(roger->base_position[X] > 0 && obs.pos[X] > 0.5 )
			return TRUE;
		else{
			arms_Retreat(roger);
			return FALSE;
		}
}
void stop_Wheels(roger)
Robot *roger;
{
	roger->base_setpoint[X]=roger->base_position[X];
	roger->base_setpoint[Y]=roger->base_position[Y];
}
void arms_Retreat(roger)
Robot *roger;
{
	roger->arm_setpoint[LEFT][0]=1.439897;
	roger->arm_setpoint[LEFT][1]=-2.617994;
	roger->arm_setpoint[RIGHT][0]=-1.439897;
	roger->arm_setpoint[RIGHT][1]=2.617994;

		/*roger->arm_setpoint[LEFT][0]=2.827433;
	roger->arm_setpoint[LEFT][1]=-2.827433;
	roger->arm_setpoint[RIGHT][0]=-2.827433;
	roger->arm_setpoint[RIGHT][1]=2.827433;*/
}

void grabAndPunch(roger)
Robot *roger;
{
	roger->arm_setpoint[LEFT][0]=1.439897;
	roger->arm_setpoint[LEFT][1]=-2.617994;
	roger->arm_setpoint[RIGHT][0]=-1.439897;
	roger->arm_setpoint[RIGHT][1]=2.617994;



}


/*
Ovservations:
* Touching the wall due to base
* Defending when the ball is behind roger
* Some Moon Walking



*/