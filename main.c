
/* Airport simulation */

/*Simulation of an Airport.

There is a small busy airport with only one runway. In each unit of time one plane can land or one plane can take off, but not both. Planes arrive ready to land or to take off at random times, so at any given unit of time, the runway may be idle or a plane may be landing or taking off. There may be several planes waiting either to land or to take off. Follow the steps given below to design the program.

1. Create two queues one for the planes landing and the other for planes taking off.
2. Get the maximum number of units for which the simulation program would run.
3. Get the expected number of planes arriving in one unit and number of planes ready to take off in one unit .
3. To display the statistical data concerning the simulation, declare following data members.
a. idletime - to store the number of units the runway was idle
b. landwait - to store total waiting time required for planes landed
c. nland - to store number of planes landed
d. nplanes - to store number of planes processed
e. nrefuse - to store number of planes refused to land on airport
f. ntakeoff - to store number of planes taken off
g. takeoffwait - to store total waiting time taken for take off
Initialize the queue used for the plane landing and for the take off
Get the data for , and from the user.

The process of simulation would run for many units of time, hence run a loop in main( ) that would run from to where would be 1 and would be the maximum number of units the program has to be run.
Generate a random number. Depending on the value of random number generated, perform following tasks.

1. If the random number is less than or equal to 1 then get data for the plane ready to land. Check whether or not the queue for landing of planes is full. If the queue is full then refuse the plane to land. If the queue is not empty then add the data to the queue maintained for planes landing.
2. If the random number generated is zero, then generate a random number again. Check if this number is less than or equal to 1. If it is , then get data for the plane ready to take off. Check whether or not the queue for taking a plane off is full. If the queue is full then refuse the plane to take off otherwise add the data to the queue maintained for planes taking off.
3. It is better to keep a plane waiting on the ground than in the air, hence allow a plane to take off only, if there are no planes waiting to land.
4. After receiving a request from new plane to land or take off, check the queue of planes waiting to land, and only if the landing queue is empty, allow a plane to take off.
5. If the queue for planes landing is not empty then remove the data of plane in the queue else run the procedure to land the plane.
6. Similarly, if the queue for planes taking off is not empty then remove the data of plane in the queue else run the procedure to take off the plane.
7. If both the queues are empty then the runway would be idle.
8. Finally, display the statistical data As given below.

Total number of planes processed
Number of planes landed :
Number of planes taken off :
Number of planes refused use :
Number of planes left ready to land :
Number of planes left ready to take off :
Percentage of time the runway was idle :
Average wait time to land :
Average wait time to take off :*/



#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <windows.h>

#define MAX 3
#define ARRIVE 0
#define DEPART 1

struct plane
{
	int id;
	int tm;
};

struct queue
{
	int count;
	int front;
	int rear;
   	struct plane p[MAX];
};

void initqueue ( struct queue * );
void addqueue ( struct queue *, struct plane );
struct plane delqueue ( struct queue * );
int size ( struct queue );
int empty ( struct queue );
int full ( struct queue );

void initqueue ( struct queue *pq )
{
	pq -> count = 0;
	pq -> front = 0;
	pq -> rear = -1;
}

void addqueue ( struct queue *pq, struct plane item )
{
	if ( pq ->count >= MAX )
	{
		printf ( "\nQueue is full.\n" );
		return;
	}
	( pq -> count )++;

	pq -> rear = ( pq -> rear + 1 )%MAX;
	pq -> p[pq -> rear] = item;
}

struct plane delqueue ( struct queue *pq )
{
	struct plane p1;

	if ( pq -> count <= 0 )
	{
		printf ( "\nQueue is empty.\n" );
		p1.id = 0;
		p1.tm = 0;
	}
	else
	{
		( pq -> count )--;
		p1 = pq -> p[pq -> front];
		pq -> front = ( pq -> front + 1 )%MAX;
	}
	return p1;
}

int size ( struct queue q )
{
	return q.count;
}

int empty ( struct queue q )
{
	return ( q.count <= 0 ) ;
}

int full ( struct queue q )
{
	return ( q.count >= MAX );
}

struct airport
{
	struct queue landing;
	struct queue takeoff;
	struct queue *pl;
	struct queue *pt;
	int idletime;
	int landwait, takeoffwait;
	int nland, nplanes, nrefuse, ntakeoff;
	struct plane pln;
} ;

void initairport ( struct airport * );
void start ( int *, double *, double * );
void newplane ( struct airport *, int, int );
void refuse ( struct airport *, int );
void land ( struct airport *, struct plane, int );
void fly ( struct airport *, struct plane, int );
void idle ( struct airport *, int );
void conclude ( struct airport *, int );
int randomnumber ( double );
void apaddqueue ( struct airport *, char );
struct plane apdelqueue ( struct airport *, char );
int apsize ( struct airport, char );
int apfull ( struct airport, char );
int apempty ( struct airport, char );
void myrandomize ( );

void initairport ( struct airport *ap )
{
    initqueue ( &( ap-> landing ) );
    initqueue ( &( ap -> takeoff ) );

	ap -> pl = &( ap -> landing );
	ap -> pt = &( ap -> takeoff );
	ap -> nplanes = ap -> nland = ap -> ntakeoff = ap -> nrefuse = 0;
	ap -> landwait = ap -> takeoffwait = ap -> idletime = 0;
}

void start ( int *endtime, double *expectarrive, double *expectdepart )
{
	int flag = 0;
	char wish;

	printf ( "\nProgram that simulates an airport with only one runway.\n" );
	printf ( "One plane can land or depart in each unit of time.\n" );
	printf ( "Up to %d planes can be waiting to land or take off at any time.\n", MAX );
	printf ( "How many units of time will the simulation run?" );
	scanf ( "%d", endtime );
	myrandomize( );
	do
	{
		printf ( "\nExpected number of arrivals per unit time? " );
		scanf ( "%lf", expectarrive );
		printf ( "\nExpected number of departures per unit time? " );
		scanf ( "%lf", expectdepart );

		if ( *expectarrive < 0.0 || *expectdepart < 0.0 )
		{
			printf ( "These numbers must be nonnegative.\n" );
			flag = 0;
		}
		else
		{
			if ( *expectarrive + *expectdepart > 1.0 )
			{
				printf ( "The airport will become saturated. Read new numbers? " );
                fflush ( stdin );
				scanf ( "%c", &wish );
				if ( tolower ( wish ) == 'y' )
					flag = 0;
				else
					flag = 1;
			}
			else
				flag = 1;
		}
	} while ( flag == 0 );
}

void newplane ( struct airport *ap, int curtime, int action )
{
	( ap -> nplanes )++;
	ap -> pln.id = ap -> nplanes;
	ap -> pln.tm = curtime;

	switch ( action )
	{
		case ARRIVE :
			printf ( "\n" );
			printf ( "Plane %d ready to land.\n", ap -> nplanes );
			break;

		case DEPART :
			printf ( "\nPlane %d ready to take off.\n", ap -> nplanes );
			break;
	}
}

void refuse ( struct airport *ap, int action )
{
	switch ( action )
	{
		case ARRIVE :

			 printf ( "\tplane  %d directed to another airport.\n", ap -> pln.id );
			 break;

		case DEPART :

			 printf ( "\tplane %d told to try later.\n", ap -> pln.id );
			 break;
	}
	( ap -> nrefuse )++;
}

void land ( struct airport *ap, struct plane pl, int curtime )
{
	int wait;

	wait = curtime - pl.tm;
	printf ( "%d: Plane %d landed ", curtime, pl.id );
	printf ( "in queue %d units \n", wait );
	( ap -> nland ) ++;
	( ap -> landwait ) += wait;
}

void fly ( struct airport *ap, struct plane pl, int curtime )
{
	int wait;

	wait = curtime - pl.tm;
	printf ( "%d: Plane %d took off ", curtime, pl.id );
	printf ( "in queue %d units \n", wait );
	( ap -> ntakeoff )++;
	( ap -> takeoffwait ) += wait;
}

void idle ( struct airport *ap, int curtime )
{
	printf ( "%d: Runway is idle.\n", curtime );
	ap -> idletime++;
}

void conclude ( struct airport *ap, int endtime )
{
	printf ( "\tSimulation has concluded after %d units.\n", endtime );
	printf ( "\tTotal number of planes processed: %d\n", ap -> nplanes );
	printf ( "\tNumber of planes landed: %d\n", ap -> nland );
	printf ( "\tNumber of planes taken off: %d\n", ap -> ntakeoff );
	printf ( "\tNumber of planes refused use: %d\n", ap -> nrefuse );
	printf ( "\tNumber left ready to land: %d\n", apsize ( *ap, 'l' ) );
	printf ( "\tNumber left ready to take off: %d\n", apsize ( *ap, 't' ) );

	if ( endtime > 0 )
		printf ( "\tPercentage of time runway idle: %lf \n", ( ( double ) ap -> idletime / endtime ) * 100.0 );

	if ( ap -> nland > 0 )
		printf ( "\tAverage wait time to land: %lf \n", ( ( double ) ap -> landwait / ap -> nland ) );

	if ( ap -> ntakeoff > 0 )
		printf ( "\tAverage wait time to take off: %lf \n", ( ( double ) ap -> takeoffwait / ap -> ntakeoff ) );
}

int randomnumber ( double expectedvalue )
{
	int n = 0;
	double em;
	double x;

	em = exp ( -expectedvalue );
	x = rand( ) / ( double ) INT_MAX;

	while ( x > em )
	{
		n++;
		x *= rand( ) / ( double ) INT_MAX;
	}
    return n;
}

void apaddqueue ( struct airport *ap, char type )
{
	switch ( tolower( type ) )
	{
		case 'l' :
			  addqueue ( ap -> pl, ap -> pln );
			  break;

		case 't' :
			  addqueue ( ap -> pt, ap -> pln );
			  break;
	}
}

struct plane apdelqueue ( struct airport *ap, char type )
{
	struct plane p1;

	switch ( tolower ( type ) )
	{
		case 'l' :
			  p1 = delqueue ( ap -> pl );
			  break;

		case 't' :
			  p1 = delqueue ( ap -> pl );
			  break;
	}

	return p1;
}

int apsize ( struct airport ap, char type )
{
	switch ( tolower ( type ) )
	{
		case 'l' :
			  return ( size ( *( ap.pl ) ) );

		case 't' :
			  return ( size ( *( ap.pt ) ) );
	}

	return 0;
}

int apfull ( struct airport ap, char type )
{
	switch ( tolower ( type ) )
	{
		case 'l' :
			  return ( full ( *( ap.pl ) ) );

		case 't' :
			  return ( full ( *( ap.pt ) ) );
	}
    return 0;
}

int apempty ( struct airport ap, char type )
{
	switch ( tolower ( type ) )
	{
		case 'l' :
			  return ( empty ( *( ap.pl ) ) );

		case 't' :
			  return ( empty ( *( ap.pt ) ) );
	}

	return 0;
}

void myrandomize( )
{
	srand ( ( unsigned int ) ( time ( NULL )%10000 ) );
}

int main( )
{
	struct airport a;
	int i, pri, curtime, endtime;
	double expectarrive, expectdepart;
	struct plane temp;

    system ( "cls" );

    initairport( &a );

	start ( &endtime, &expectarrive, &expectdepart );

	for ( curtime = 1 ; curtime <= endtime ; curtime++ )
	{
		pri = randomnumber ( expectarrive );

		for ( i = 1 ; i <= pri ; i++ )
		{
			newplane ( &a, curtime, ARRIVE );
			if ( apfull ( a, 'l' ) )
				 refuse ( &a, ARRIVE );
			else
				apaddqueue( &a, 'l' );
		}

		pri = randomnumber ( expectdepart );
		for ( i = 1 ; i <= pri ; i++ )
		{
			newplane ( &a, curtime, DEPART );
			if ( apfull ( a, 't' ) )
			   refuse ( &a, DEPART );
			else
			   apaddqueue ( &a, 't' );
		}

		if (  ! ( apempty ( a, 'l' ) ) )
		{
			temp = apdelqueue ( &a, 'l' );
			land ( &a, temp, curtime );
		}
		else
		{
			if ( ! ( apempty ( a, 't' ) ) )
			{
				temp = apdelqueue ( &a, 't' );
				fly ( &a, temp, curtime );
			}
			else
				idle ( &a, curtime );
		}
	}

	conclude ( &a, endtime );

    return 0;
}
