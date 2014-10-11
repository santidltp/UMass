/*************************************************************************/
/* File: project2.c                                                      */
/* Description: Inverse Kinematic function for Arms                      */
/*                 inv_arm_kinematics() - writes arm cspace setpoints    */
/*                 (stand alone - outside control cycle)                 */
/*              Stereo Triangulate code (in control cycle)               */
/*                 stereo_observation() - (vision.c) computes Cartesian  */
/*                 observations (mean, cov), visualize procedure draws   */
/*                 observation to canvas                                 */
/* Date:        9-1-2014                                                 */
/*************************************************************************/
#include <math.h>
#include <stdio.h>
#include "roger.h"
#include "simulate.h"
#include "control.h"
#include "modes.h"

void construct_wTb(), inverse_homogeneous_xform(), matrix_times_vector();

/**********************************************************************/
/*** PROJECT #2 - INVERSE KINEMATICS: MAP (X,Y) TO (THETA1, THETA2) ***/
/**********************************************************************/
int inv_arm_kinematics(roger, limb, x, y)
Robot * roger;
int limb;
double x, y;
{
  double wTb[4][4], bTw[4][4], ref_b[4], ref_w[4];

  double r2, c2, s2_plus, s2_minus, theta2_plus, theta2_minus;
  double k1, k2_plus, k2_minus, alpha_plus, alpha_minus;
  double theta1_plus, theta1_minus;

  // transform world frame (x,y)_w input into base coordinates (x,y)_b

  // add the arm offset for the approariate arm to get it to shoulder frame
  if (limb==LEFT) ref_b[Y] -= ARM_OFFSET;
  else ref_b[Y] += ARM_OFFSET;

  // implement the inverse kinematic algorithm

  // pick one of the two solutions and write it into the setpoint of the arm

  // return TRUE if there was a solution, FALSE if the location is out of reach
  return(FALSE);
}

/**********************************************************************/
/*** PROJECT #2 - triangulate on the red ball                       ***/
/**********************************************************************/

void draw_observation(); // in simulator file xrobot.c

Observation obs;

/*************************************************************************/
/* project development interface - called by GUI                         */
/* project2_control() is executed automatically when                     */
/* control mode = PROJECT2; input mode = BALL INPUTS                     */
/*************************************************************************/
void project2_control(roger, time)
Robot * roger;
double time;
{
  int stereo_observation();

  printf("STEREO OBSERVATION state=%d\n", stereo_observation(roger, &obs));

  // check if ball is in view
  // write Observation "obs" = mean and cov in world coordinates
  //  if (stereo_observation(roger, &obs)) {          // in vision.c
  //     printf("PROJECT 2: stereo_observation()- x=%6.4lf y=%6.4lf\n",
  //        obs.pos[X], obs.pos[Y]);
  //     printf("                  %lf %lf\n", obs.cov[0][0], obs.cov[0][1]);
  //     printf("                  %lf %lf\n\n",obs.cov[1][0],obs.cov[1][1]);
  //  }
  //  else printf("no valid stereo observation!\n");
}

void project2_reset(roger)
Robot* roger;
{ }

void project2_enter_params() 
{
  printf("Project 2 enter_params called. \n");
}

void project2_visualize(roger)
Robot* roger;
{ 
  void draw_observation();
  draw_observation(obs); /* defined in xrobot.c */
}


