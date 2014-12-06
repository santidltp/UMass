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

double setTimer=0;
double ballx=0;
double bally=0;
Observation ball;
void project4_control(roger, time)
Robot* roger;
double time;
{ 

Game(roger,time);

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
 	 	if(stereo_observation(roger, &ball)){	
		 		roger->base_setpoint[X]=ball.pos[X];
				roger->base_setpoint[Y]=ball.pos[Y];

		}
		else{
			SEARCHTRACK();
		}
 }

 int Punch(roger)
 Robot* roger;
 {
 	//return arm movement if is within the range.
 if(stereo_observation(roger,&ball)){
 	//if(ball.pos[Y]<-1.84 || ball.pos[Y]> 1.84)return FALSE;
 	if(ball.pos[Y]  < roger->base_position[Y])
 		return inv_arm_kinematics(roger, RIGHT, ball.pos[X], ball.pos[Y]) ;
 	else
		return inv_arm_kinematics(roger, LEFT, ball.pos[X], ball.pos[Y]) ;
 }	

}
	


void baseRetreat(Robot *roger){
	
	roger->base_setpoint[X]=-3.5;
				roger->base_setpoint[Y]=0;
				roger->base_setpoint[THETA]=0 ;
			

}

double predictBall_inYAxis(Robot *roger,double time){

double angle=0, ballVx=0, ballVy=0, y=0, xSpace=0, ySpace=0;

/*ballVx = BALLSPEED*Math.cos(bounceAngle);
ballVy = BALLSPEED*-Math.sin(bounceAngle);
*/
		xSpace = (ball.pos[X] - ballx);
		ySpace = (ball.pos[Y] - bally);
		angle  = atan2(xSpace,ySpace);
		ballVx = xSpace/0.01;
		ballVy = ySpace/0.01;
		// setTimer =time;
		y = tan(angle)*xSpace;
		if (y > 1.73)		{
			y= y-1.73 * (-1);
		}else if(y<-1.73){
			y = y-(-1.73) * (-1);
		}
	printf("angle: %f\ty: %f\n",angle, y );

		return y;

	
	return 0;
}

 void NoChase(Robot *roger,double time){
 	double finaly=predictBall_inYAxis(roger,time);

	roger->base_setpoint[Y] = finaly;
	roger->base_setpoint[X] = roger->base_position[X];
	// printf("finaly: %f\t time: %f\n",finaly, time );
}



/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
///////////////////////////PONG//////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////



void Game(roger,time)
Robot *roger;
double time;
{

	
			// if(time >= (setTimer+0.1)){
			// 	setTimer = time;
			// 	// NoChase(roger,time);
			// 	ballx=ball.pos[X];
			// 	bally=ball.pos[Y];

			// }
			// predictBall_inYAxis(roger,time);

		SEARCHTRACK(roger,time);
		

	// If the ball was found and is being tracked, then chase it.
	if( !Punch(roger) &&  isBallOnMySide(roger) ){
		Chase(roger,time);
		// trapBall(ball,roger);
	}
	else if(  !isBallOnMySide(roger)) {

			// printf("settimer: %f\n",setTimer );
	
			baseRetreat(roger);
			armsRetreat(roger);
			
				// roger->base_position[Y] = -0.86;
				 // armsTrapBallOpen(roger);
		}
	else if(Punch(roger)){
		// baseRetreat(roger);

		//nothing
	}



} 




/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
///////////////////////////Game//////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////


int isBallOnMySide(roger)
Robot *roger;
{
	// printf("base: %f\t Ball: %f\t",roger->base_position[X],obs.pos[X] );
		//check if the body of roger and the ball are in the same side.
		if(stereo_observation(roger, &ball)){	
			if(roger->base_position[X] < 0 && ball.pos[X] < -0.2)
				return TRUE;
			else if(roger->base_position[X] > 0 && ball.pos[X] > 0.2 )
				return TRUE;
			else
				return FALSE;
	    }
}
void stop_Wheels(roger)
Robot *roger;
{
	roger->base_setpoint[X]=roger->base_position[X];
	roger->base_setpoint[Y]=roger->base_position[Y];
}


void armsRetreat(roger)
Robot *roger;
{
	roger->arm_setpoint[LEFT][0] = 1.439897;
	roger->arm_setpoint[LEFT][1] =-2.617994;
	roger->arm_setpoint[RIGHT][0]=-1.439897;
	roger->arm_setpoint[RIGHT][1]= 2.617994;

}


void grabAndPunch(roger)
Robot *roger;
{
	roger->arm_setpoint[LEFT][0]=1.439897;
	roger->arm_setpoint[LEFT][1]=-2.617994;
	roger->arm_setpoint[RIGHT][0]=-1.439897;
	roger->arm_setpoint[RIGHT][1]=2.617994;

}

void protectRight(roger)
Robot *roger;
{
	roger->arm_setpoint[LEFT][0]= -0.555461;
	roger->arm_setpoint[LEFT][1]=-1.670964;
	roger->arm_setpoint[RIGHT][0]=-2.174667;
	roger->arm_setpoint[RIGHT][1]=1.654695;
}

void armsTrapBallClose(roger)
Robot *roger;
{
	roger->arm_setpoint[LEFT][0]	=   1.310810;
	roger->arm_setpoint[LEFT][1]	=  -2.400305;
	roger->arm_setpoint[RIGHT][0]	=  -1.182136;
	roger->arm_setpoint[RIGHT][1]	=	2.228120;
}

void armsTrapBallOpen(roger)
Robot *roger;
{
	roger->arm_setpoint[LEFT][0]	=   1.248399;
	roger->arm_setpoint[LEFT][1]	=  -2.094164;
	roger->arm_setpoint[RIGHT][0]	=  -1.526108;
	roger->arm_setpoint[RIGHT][1]	=	2.143611;
}
//graber-> left: 1.310810	-2.400305 right: -1.182136	2.228120
//graber open-> left: 1.248399	-2.094164 right: -1.526108	2.143611

int stereo_observationWithRespect(roger, obs)
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
// printf("heleeeeeeeeeeeeeeeeeeeeeeeeee%d\n",compute_average_red_pixel(roger, &ur, &ul));
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

          obs->pos[X] = ref_b[X];
           obs->pos[Y] = ref_b[Y];
       return TRUE;
   }
   return FALSE;
   }

 


// void trapBall(Observation ball, Robot *roger){
// 		if(stereo_observationWithRespect(roger, &ball)){
// 		//ballSpeed(ball,time);
// 	//	isBallTrapped(roger,ball,time);
// 			if(ball.pos[X] < 0.4){
// 				armsTrapBallClose(roger);
// 			// 	if(isBallTrapped(roger,ball,time))
// 			// 		armsTrapBallOpen(roger);
// 			 }
// 			else{
// 					armsTrapBallOpen(roger);			
// 			}
// 		}
// 		else{
// 			armsTrapBallOpen(roger);
// 		}
// }


/*
Ovservations:
* Touching the wall due to base
* Some Moon Walking
* Defending:
*	 When the ball is behind roger
*	 When Boucing the wall
* 
Attacking
* Ball prediction
* Aiming
* Grabbing ball
*/