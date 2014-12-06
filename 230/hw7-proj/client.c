/* inet_str_client.c: Internet stream sockets client */
#include <stdio.h>
#include <sys/types.h>	     /* sockets */
#include <sys/socket.h>	     /* sockets */
#include <netinet/in.h>	     /* internet sockets */
#include <unistd.h>          /* read, write, close */
#include <netdb.h>	         /* gethostbyaddr */
#include <stdlib.h>	         /* exit */
#include <string.h>	         /* strlen */

void perror_exit(char *message);
int MUL (char *message);

void main(int argc, char *argv[]) {
    int             port, sock, i, n;
    char            buf[256];
    struct sockaddr_in server;
    struct sockaddr *serverptr = (struct sockaddr*)&server;
    struct hostent *rem;
    //SpireID
    char SpireID[20] = "28468589"; 
    char BonusQuestion [10]="HW7";
    char probelm[100]="MUL 36 42";//for testing purposes
   

    if (argc != 3) {
    	printf("Please give host name and port number\n");
       	exit(1);}
	/* Create socket */
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    	perror_exit("socket");
	/* Find server address */
    if ((rem = gethostbyname(argv[1])) == NULL) {	
	   herror("gethostbyname"); exit(1);
    }
    port = atoi(argv[2]); /*Convert port number to integer*/
    server.sin_family = AF_INET;       /* Internet domain */
    memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
    server.sin_port = htons(port);         /* Server port */
    /* Initiate connection */
    if (connect(sock, serverptr, sizeof(server)) < 0)
	   perror_exit("connect");


		printf("Connected to Server.\n");//connection stablished
		printf("Sending Spire ID: %s\n", SpireID);//sending my spireID
		bzero(buf,256);//making sure there's nothing in the buffer
		/*Copy from SpireID to buffer*/
		memcpy(buf, SpireID, strlen(SpireID));
		/* Send Spire ID */
		send(sock, buf , strlen(buf),0);
		//clear buffer
		bzero(buf,256);
		/* receive question */
		n= recv(sock, &buf , 256, 0);
		//add null terminator
		buf[n+1] = '\0';

		printf("Received Problem: %s\n", buf);
		i=MUL(probelm);
		printf("%d\n",i );
		snprintf(buf, 256,"%d",i);
		//send solution
		send(sock, buf , strlen(buf),0);
		//clear buffer
		printf("Sending Solution: %s\n", buf);

		//clear buffer
		bzero(buf,256);
		/* receive question */
		n= recv(sock, &buf , 256, 0);
		//add null terminator
		buf[n+1] = '\0';
		printf("%s\n", buf);//succesful or not

		bzero(buf,256);
		memcpy(buf, BonusQuestion, strlen(BonusQuestion));
		printf("Sending Bonus Question: %s\n",buf);
		send(sock, buf , strlen(buf),0);
		//clear buffer
		bzero(buf,256);

		n= recv(sock, &buf , 256, 0);
		//add null terminator
		buf[n+1] = '\0';
		printf("%s\n", buf);//succesful or not    
		//Bonus
		send(sock, buf , strlen(buf),0);
	    close(sock);                 /* Close socket and exit */
}			     
int MUL(char *message){

	char *operator = strtok(message, " ");
	char *cx =  strtok(NULL, " ");
	char *cy =  strtok(NULL, " ");

	int x = atoi(cx);
	int y = atoi(cy);

	return x*y;

}
void perror_exit(char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}
