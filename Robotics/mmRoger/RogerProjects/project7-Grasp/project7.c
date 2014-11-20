/*************************************************************************/
/* File:        project7.c                                               */
/* Description: User project #7 - empty project directory for project    */
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

void project7_control(roger, time)
Robot* roger;
double time;
{ }

/************************************************************************/
void project7_reset(roger)
Robot* roger;
{ 
	roger->arm_setpoint[LEFT][0]=2.827433;
	roger->arm_setpoint[LEFT][1]=-2.827433;
	roger->arm_setpoint[RIGHT][0]=-2.827433;
	roger->arm_setpoint[RIGHT][1]=2.827433;

}

// prompt for and read user customized input values
void project7_enter_params() 
{
  printf("Project 4 enter_params called. \n");
}

//function called when the 'visualize' button on the gui is pressed
void project7_visualize(roger)
Robot* roger;
{ }
