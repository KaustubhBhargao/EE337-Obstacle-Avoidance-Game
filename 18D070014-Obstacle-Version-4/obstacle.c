#include <at89c5131.h>

/****************************
	Determines the obstacle based on 
****************************/
void LFSR_next(int *LFSR, int* obstacle_temp)
{
	//already initialised
	int var2 = *LFSR; //Temp variable
	if(*LFSR==1)
	{
		//obstacle_temp = {0,1,1,0,0,0}; 
		*obstacle_temp =0;
		*(obstacle_temp + 1) =1;
		*(obstacle_temp + 2) =1;
		*(obstacle_temp + 3) =0;
		*(obstacle_temp + 4) =0;
		*(obstacle_temp + 5) =0;
		//LFSR=4;
		var2 = 4;
	}
	else if(*LFSR==4)
	{
		//obstacle_temp = {0,0,0,0,1,1};
		*obstacle_temp =0;
		*(obstacle_temp + 1) =0;
		*(obstacle_temp + 2) =0;
		*(obstacle_temp + 3) =0;
		*(obstacle_temp + 4) =1;
		*(obstacle_temp + 5) =1;
		//LFSR=6;
		var2 = 6;
	}
	else if(*LFSR==6)
	{
		//obstacle_temp = {0,1,0,0,0,0};
		*obstacle_temp =0;
		*(obstacle_temp + 1) =1;
		*(obstacle_temp + 2) =0;
		*(obstacle_temp + 3) =0;
		*(obstacle_temp + 4) =0;
		*(obstacle_temp + 5) =0;
		//LFSR=7;
		var2 = 7;
	}
	else if(*LFSR==7)
	{
		//obstacle_temp = {0,0,0,0,0,1};
		*obstacle_temp =0;
		*(obstacle_temp + 1) =0;
		*(obstacle_temp + 2) =0;
		*(obstacle_temp + 3) =0;
		*(obstacle_temp + 4) =0;
		*(obstacle_temp + 5) =1;
		//LFSR=3;
		var2 = 3;
	}
	else if(*LFSR==3)
	{
		//obstacle_temp = {0,0,0,0,1,0};
		*obstacle_temp =0;
		*(obstacle_temp + 1) =0;
		*(obstacle_temp + 2) =0;
		*(obstacle_temp + 3) =0;
		*(obstacle_temp + 4) =1;
		*(obstacle_temp + 5) =0;
		//LFSR=5;
		var2 = 5;
	}
	else if(*LFSR==5)
	{
		//obstacle_temp = {0,0,1,0,0,0};
		*obstacle_temp =0;
		*(obstacle_temp + 1) =0;
		*(obstacle_temp + 2) =1;
		*(obstacle_temp + 3) =0;
		*(obstacle_temp + 4) =0;
		*(obstacle_temp + 5) =0;
		//LFSR=2;
		var2 = 2;
	}
	else
	{
		//obstacle_temp = {0,0,0,0,0,0};
		*obstacle_temp =0;
		*(obstacle_temp + 1) =0;
		*(obstacle_temp + 2) =0;
		*(obstacle_temp + 3) =0;
		*(obstacle_temp + 4) =0;
		*(obstacle_temp + 5) =0;
		//LFSR=1;
		var2 = 1;
	}
	*LFSR = var2;
	//return (obstacle_temp);
}