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

void project6_control(roger, time)
Robot* roger;
double time;
{ 
if(time==0.0){
	roger->arm_setpoint[LEFT][0]=1.439897;
	roger->arm_setpoint[LEFT][1]=-2.617994;
	roger->arm_setpoint[RIGHT][0]=-1.439897;
	roger->arm_setpoint[RIGHT][1]=2.617994;
}

 if(time > 0.200000 && time < 0.400000){
printf("heel\n");
	roger->arm_setpoint[LEFT][0]=2.827433;
	roger->arm_setpoint[LEFT][1]=-2.827433;
	roger->arm_setpoint[RIGHT][0]=-2.827433;
	roger->arm_setpoint[RIGHT][1]=2.827433;
}

 if(time > 0.400000 && time < 0.600000){
printf("heel\n");
	roger->arm_setpoint[LEFT][0]=0.75;
	roger->arm_setpoint[LEFT][1]=-2;
	roger->arm_setpoint[RIGHT][0]=-0.75;
	roger->arm_setpoint[RIGHT][1]=2;
}

	printf("%f\n",time);

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
