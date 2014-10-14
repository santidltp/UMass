/*************************************************************************/
/* File:        vision.c                                                 */
/* Description: User project #2                                          */
/* Date:        5-1-13                                                   */
/*************************************************************************/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
// #include "Xkw/Xkw.h"

#include "roger.h"
#include "simulate.h"
#include "control.h"
#include "modes.h"

void construct_wTb(), matrix_times_vector(), matrix_transpose22(), 
     matXmat2222();

// triangulate the position of the red ball, transform from the base frame to
// to world frame and write into Observation obs
int stereo_observation(roger, obs)
Robot * roger;
Observation * obs;
{
  double ur, ul;
  double wTb[4][4], bTw[4][4], wRb[2][2], bRw[2][2], ref_b[4], ref_w[4];
  double lambdaL, gammaL, gammaR; // stereoJJt[2][2];
  double mat_22[2][2], cov_b[2][2];

  void stereoJJT();

  int compute_average_red_pixel();
  int red_blue_transitions=FALSE;
  int ball_leaving_image=FALSE;
  
  /************************************************************************/
  // PROJECT2: triangulate to localize the red ball in the base frame
  //           convert to world frame, and
  //           write into Observation "obs"
  //
  // build a cascade of filters to identify "valid" stereo observations
  // is there "red" on both image planes?
  // are there any red-blue transitions? i.e the hands are in the way
  // are there any FOV issues? i.e. the ball extends to the edge of the image
  if ( (compute_average_red_pixel(roger, &ur, &ul) == TRUE)
          && (red_blue_transitions == FALSE)
          && (ball_leaving_image == FALSE) ) {

    // get angle from left and right eye frames to the center of the object
    //    gammaL = ...
    //    gammaR = ...

    // lambda_L: catch case where eyes are parallel and same pixels on both
    // eyes (infinite distance)
    //    if ((gammaR - gammaL) == 0.0) lambdaL = 20.0;
    //    else lambdaL = ...

    // calculate x,y coordinate in base frame
    //    ref_b[X] = ...
    //    ref_b[Y] = ...
    //    ref_b[2] = ...
    //    ref_b[3] = ...

    // convert into world frame
    construct_wTb(roger->base_position, wTb);
    wRb[0][0] = wTb[0][0]; wRb[0][1] = wTb[0][1];
    wRb[1][0] = wTb[1][0]; wRb[1][1] = wTb[1][1];

    matrix_times_vector(wTb, ref_b, ref_w);

    obs->pos[X] = ref_w[X];
    obs->pos[Y] = ref_w[Y];

    // compute observation cov (JJT) 
    stereoJJT(roger, ur, ul, cov_b);

    // rotate it into world coordinates ... [cov]_w = wRb [cov]_b wRb^T
    // obs->cov = cov_w = ...

    // and scale it by the SQR of the observation variance
    // obs->cov *= SQR(SIGMA_OBS);

    return(TRUE);
  }
  else return(FALSE);
}

/*************************************************************************/
// compute the (0<=position<128) of the average red pixel on both image planes
// if red is detected on both images:
//       write the index of the mean pixel into ul and ur;
//       return TRUE;
// else return FALSE;
/*************************************************************************/
int compute_average_red_pixel(roger, ur, ul)
Robot* roger;
double *ur, *ul;
{
  int i, nr, nl;
  double r,g,b,I,m,S,H;

  nr = nl = 0;
  *ul = *ur = 0.0;

  /* scan the left and right images and compute the average red pixel */
  //  for (i=0;i<NPIXELS;++i) { ... }
  // 
  // if there's at least one red pixel on both images
  // compute a real valued center of the red segment on
  // both images, ul and ur, and 
  // return(TRUE);
  //
  // else 
  return(FALSE);
}

/*************************************************************************/
// if red is detected in pixel 0 and 127 on either image:
//       return FALSE
// else  return TRUE;
/*************************************************************************/
int check_stereo_FOV(roger)
Robot* roger;
{ 
  return(FALSE);
}

void stereoJJT(roger, ur, ul, JJT)
Robot * roger;
double ur, ul, JJT[2][2]; /* observation covariance only assigns 2x2 */
{ }

