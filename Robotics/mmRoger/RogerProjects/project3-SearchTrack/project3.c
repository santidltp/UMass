/*************************************************************************/
/* File:        project3.c                                               */
/* Description: User project #3 - SEARCHTRACK                            */
/* Date:        3-06-2014                                                */
/*************************************************************************/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "roger.h"
#include "simulate.h"
#include "control.h"
#include "modes.h"

int sample_gaze_direction(), compute_average_red_pixel();
int stereo_observation();
void draw_observation();

extern Observation obs;

/* RETURN STATUS FOR ALL ACTIONS:                                */
/* 0-"UNKNOWN", 1-"NO_REFERENCE", 2-"TRANSIENT", 3-"CONVERGED" */
/*************************************************************************/
/* SEARCH controller -                                                   */
/* explore samples from the SEARCH distribution for the target red ball  */
/*************************************************************************/
int SEARCH(roger, time)
Robot * roger;
double time;
{
  static double search_heading;
  double heading_error_base;

  static int return_state = UNKNOWN;

  // SAMPLE A NEW REFERENCE HEADING
  if (return_state != TRANSIENT) {
    // sample a new gaze heading from the SEARCH distribution
    if (sample_gaze_direction(&search_heading) == FALSE) {
      return_state = NO_REFERENCE;
    }
    else return_state = TRANSIENT;
  }

  else {
    /***********************************************************************/
    /* PROJECT3 PART I - complete the code to explore sampled              */
    /*    search_heading using base and eyes                               */
    /***********************************************************************/
    // heading_error_base = ...
    // while (heading_error_base > M_PI) heading_error_base -= 2.0 * M_PI;  
    // while (heading_error_base < -M_PI) heading_error_base += 2.0 * M_PI;
    
    /* define new setpoints for the base and the eyes                      */
    // roger->base_setpoint[THETA] = ...
    // roger->eyes_setpoint[LEFT] = ...
    // roger->eyes_setpoint[RIGHT] = ...

    // check for CONVERGE
    if ( fabs(heading_error_base) < 0.01) return_state = CONVERGED;

    /***********************************************************************/
    /* PROJECT3 PART I - END                                               */
    /***********************************************************************/
  }
  //  printf("      returning from search\n"); fflush(stdout);
  return(return_state);
}

/*************************************************************************/
// primitive TRACK controller - TRACK the red ball with base and eyes
/*************************************************************************/
int TRACK(roger, time)
Robot* roger;
double time;
{
  double ul, ur, error_eye[2], error_base;

  static int return_state = UNKNOWN;

  // control eyes independently and triangulate is ball is visible in both eyes
  if (compute_average_red_pixel(roger, &ur, &ul) == TRUE) {
    /***********************************************************************/
    /* PROJECT3 PART II - complete the code to TRACK the red ball          */
    /*    that using base and eyes                                         */
    /***********************************************************************/
    error_eye[LEFT] = atan2((ul-63.5), 64.0);
    error_eye[RIGHT] = atan2((ur-63.5), 64.0);

    // define eye setpoints
    // roger->eyes_setpoint[LEFT] = ...
    // roger->eyes_setpoint[RIGHT] = ...
    
    // define base setpoints
    // error_base = ...
    // roger->base_setpoint[THETA] = ...

    // check for CONVERGE
    if ((fabs(error_eye[LEFT]) < 0.1) && (fabs(error_eye[RIGHT]) < 0.1) &&
	(fabs(error_base) < 0.1)) {
      return_state = CONVERGED;
    }
    else { return_state = TRANSIENT; }

    stereo_observation(roger, &obs); // in project2-Kinematics/vision.c
    /***********************************************************************/
    /* PROJECT3 PART II - END                                              */
    /***********************************************************************/
  }
  else {
    // No ball in view -> no reference
    return_state = NO_REFERENCE;
  }
  return(return_state);
}

/*************************************************************************/
// SEARCH/TRACK the red ball using primitive SEARCH and TRACK controllers
/*************************************************************************/
int SEARCHTRACK(roger, time)
Robot *roger;
double time;
{
  static int return_state = UNKNOWN;
  static int internal_state[2] = { UNKNOWN, UNKNOWN };//[0]->SEARCH [1]->TRACK
  int i, state;

  //  printf("   in searchtrack\n");

  /**********************************************************************/
  /* PROJECT3 PART III - the FSA for SEARCHTRACK                        */
  /*    internal_state=[ 0:SEARCH 1:TRACK ]                             */ 
  /**********************************************************************/
  state = internal_state[1]*4 + internal_state[0];
  switch (state) {
    // the example shows an aggregate "X" that produces the same actions
    //    for all 4 SEARCH return values
    //                                       TRACK         SEARCH
  case 0:                              //  UNKNOWN   -  UNKNOWN
  case 1:                              //  UNKNOWN   - NO_REFERENCE
  case 2:                              //  UNKNOWN   -  TRANSIENT
  case 3:                              //  UNKNOWN   -  CONVERGED
    // return_state = ...
    // set return values by hand or execute SEARCH()/TRACK() to return them
    // internal_state[0] = ...
    // internal_state[1] = ...
    break;
  case 4:                              // NO_REFERENCE -  UNKNOWN
  case 5:                              // NO_REFERENCE - NO_REFERENCE
  case 6:                              // NO_REFERENCE -  TRANSIENT
  case 7:                              // NO_REFERENCE -  CONVERGED
    // return_state = ...
    // set return values by hand or execute SEARCH()/TRACK() to return them
    // internal_state[0] = ...
    // internal_state[1] = ...
    break;

  case 8:                              //  TRANSIENT   -  UNKNOWN
  case 9:                              //  TRANSIENT   - NO_REFERENCE
  case 10:                             //  TRANSIENT   -  TRANSIENT
  case 11:                             //  TRANSIENT   -  CONVERGED
    // return_state = ...
    // set return values by hand or execute SEARCH()/TRACK() to return them
    // internal_state[0] = ...
    // internal_state[1] = ...
    break;
  case 12:                             //  CONVERGED   -  UNKNOWN
  case 13:                             //  CONVERGED   - NO_REFERENCE
  case 14:                             //  CONVERGED   -  TRANSIENT
  case 15:                             //  CONVERGED   -  CONVERGED
    // return_state = ...
    // set return values by hand or execute SEARCH()/TRACK() to return them
    // internal_state[0] = ...
    // internal_state[1] = ...
    break;
  default:
    break;
  }
  /***********************************************************************/
  /* PROJECT3 PART III - END                                             */
  /***********************************************************************/
  return(return_state);
}

/*************************************************************************/
/* project development interface - called by GUI                         */
/*************************************************************************/
void project3_control(roger,time)
Robot * roger;
double time;
{
  static int state = UNKNOWN;

  printf("SEARCHTRACK state=%d\n", SEARCHTRACK(roger, time));
}

/*************************************************************************/
void project3_reset(roger)
Robot* roger;
{ }

void project3_visualize(roger)
Robot* roger;
{
  void draw_observations();
  draw_observation(obs);
}

/*************************************************************************/
// prompt for and read user customized input values
/*************************************************************************/
void project3_enter_params() 
{
  printf("Project 3 enter_params called. \n");
}
