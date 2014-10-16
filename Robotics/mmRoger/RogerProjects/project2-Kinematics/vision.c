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
  double d=0;
  //void stereoJJT();

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
        gammaL = roger->eye_theta[LEFT] + atan2((ul-63.5),64.0);
        gammaR = roger->eye_theta[RIGHT]+ atan2((ur-63.5),64.0);

    // lambda_L: catch case where eyes are parallel and same pixels on both
    // eyes (infinite distance)
       if ((gammaR - gammaL) == 0.0) lambdaL = 20.0;
       else lambdaL = (2*BASELINE*cos(gammaR))/(sin(gammaR-gammaL));
    // calculate x,y coordinate in base frame
       ref_b[X] = lambdaL * cos(gammaL);
       ref_b[Y] = BASELINE + (lambdaL * sin(gammaL));
       ref_b[2] = 0;
       ref_b[3] = 1;

    // convert into world frame
    construct_wTb(roger->base_position, wTb);
    wRb[0][0] = wTb[0][0]; wRb[0][1] = wTb[0][1];
    wRb[1][0] = wTb[1][0]; wRb[1][1] = wTb[1][1];

    matrix_times_vector(wTb, ref_b, ref_w);

    obs->pos[X] = ref_w[X];
    obs->pos[Y] = ref_w[Y];



    d=(2* BASELINE)/(sin(gammaR-gammaL)*sin(gammaR-gammaL));
    double JJ[2][2];
    double JJT[2][2];
    double JW[2][2];
    double JWT[2][2];
    JJ[0][0]= cos(gammaR)*cos(gammaR)*d;
    JJ[0][1]= - cos(gammaR)*cos(gammaR)*d;
    JJ[1][0]= sin(gammaR) *cos(gammaR) * d;
    JJ[1][1]= - sin(gammaR) *cos(gammaR) * d;
    matXmat2222(wRb,JJ,JW);
    matrix_transpose22(JW,JWT);
    matXmat2222(JW,JWT,JJT);

    // compute observation cov (JJT)
  //  stereoJJT(roger, ur, ul, cov_b);


    // rotate it into world coordinates ... [cov]_w = wRb [cov]_b wRb^T
    obs->cov[0][0] = 8*JJT[0][0];
    obs->cov[0][1] = 8*JJT[0][1];
    obs->cov[1][0] = 8*JJT[1][0];
    obs->cov[1][1] = 8*JJT[1][1];
    // and scale it by the SQR of the observation variance
    obs->cov[0][0] *= sqrt(0.000001);
    obs->cov[0][1] *= sqrt(0.000001);
    obs->cov[1][0] *= sqrt(0.000001);
    obs->cov[1][1] *= sqrt(0.000001);

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
  double minValue,maxValue;
  double max, min;
  int check =0,dcheck=1;
  int checktwo=0,dchecktwo=1;
  nr = nl = 0;
  *ul = *ur = 0.0;


  /* scan the left and right images and compute the average red pixel */
    for (i=0;i<NPIXELS;++i) {
    if(roger->image[LEFT][i][RED_CHANNEL]  == 255/*red*/){
      if(dcheck){
      minValue =i;
      dcheck=0;
      check=1;
      }
    }
    if((roger->image[LEFT][i][RED_CHANNEL]  != 255) && check){
      maxValue=i-1;
      check=0;
      *ul= (maxValue+minValue)/2;
    }


    if(roger->image[RIGHT][i][RED_CHANNEL] == 255/*red*/){
      if(dchecktwo){
       min =i;
       dchecktwo=0;
       checktwo=1;
       }
     }
     if((roger->image[RIGHT][i][RED_CHANNEL]  != 255) && checktwo){
       max=i-1;
       checktwo=0;
       *ur=(max+min)/2;
     }

    //
    // printf("%f\t\t%f\n",*ul,*ur);
  }
  //
  // if there's at least one red pixel on both images
  // compute a real valued center of the red segment on
  // both images, ul and ur, and
  // return(TRUE);
  //
  // else
  if(*ul>0||*ur>0)return (TRUE);
  else return(FALSE);
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

// void stereoJJT(roger, ur, ul, JJT)
// Robot * roger;
// double ur, ul, JJT[2][2]; /* observation covariance only assigns 2x2 */
// { }
