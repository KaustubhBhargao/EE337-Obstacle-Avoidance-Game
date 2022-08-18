#include <at89c5131.h>
#include "lcd.h"		//Header file with LCD interfacing functions
#include "serial.c"	//C file with UART interfacing functions
#include "obstacle.c" //C file for obstacle generation

//Global Variables
int dino = 1;              //Detemines current row of the Dinosaur. Takes values 1, 2.
int state = 3;             //Variable to signify change of obstacle. Takes values from 1 to 3
int LFSR = 1;              //Initial non-zero value of the LFSR register = #001B (seed)
int temp_obstacle[6] = 0;  //To store the current obstacle
int row1[16] = 0;          //To store elements of first row of LCD
int row2[16] = 0;          //Seconds row of LCD
int var_T0 = 31;           //For timer zero ISR. 31 will lead to 1 seconds delay (24MHz => Overflow every 32512.5 us)
int score = 0;             //Initialise score
//int check = 0;             //Variable to signigfy occurance of collision. 0=>no collision
int max_score= 0;          //Initialise max score
//code char line1[] = "*****************************\r\n";
code char line2[] = "***Obstacle Avoidance Game***\r\n";
//code char line3[] = "*****************************\r\n";
code char line4[] = "Incorrect Command\r\n";
code char space[] = " ";
code char D[] = "D";
code char dline1[] = "Score: ";
code char dline2[] = "High Score: ";
code char star[] = "*";
code char temp_string[5]={0,0,0,0,'\0'};

/*********************************
	Check for collision with current set of values
	If collision present, call collide(), Return
	If nor present, do nothing, return
	If occured,
	Called when collision occurs
	Clear LCD
	Display Score
	Update Max Score
	DELAY 3s
	Initiate Dinosaur Position
	Start sending obstacles
	exit
*********************************/
void collision_check(void)
{
	if((dino==1 && row1[2]==1) || (dino==2 && row2[2]==1))
	{
		if(score > max_score){max_score = score;}
		lcd_cmd(0x01); //Clear the Display
		msdelay(50);
		lcd_cmd(0x80); //Curson on First row 1st Column
		lcd_write_string(dline1);
		lcd_cmd(0x87); //Cursor on First row 8th column
		int_to_string(score, temp_string);
		lcd_write_string(temp_string);
		
		lcd_cmd(0x0C0);
		lcd_write_string(dline2);
		lcd_cmd(0x0CC); //Cursor on Second row 12th column
		int_to_string(max_score, temp_string);
		lcd_write_string(temp_string);
		
		
		msdelay(3000); 
		lcd_cmd(0x01); //Clear the Display
		dino = 1; //reset dinosaur postion
		state = 3; //rest state of obstacle
		score = 0; //reset score
		row1[16] = 0;          //To store elements of first row of LCD
		row2[16] = 0;          //Seconds row of LCD
	}
}


//Main Function
void main(void)
{
	unsigned char ch=0;
	
	//START TIMER T0 Here
	EA = 1;                    //Enable interrupts
	ET0 = 1;                   //Enable intrrupts from Timer0
	TH0 = 0x00;
	TL0 = 0x00;
	TR0 = 1;                    //Start the timer
	
	//Call initialization functions
	lcd_init();
	uart_init();
	
	//These strings will be printed in terminal software
	//transmit_string(line1);
	transmit_string(line2);
	//transmit_string(line3);
	/*transmit_string("Press q to move UP\r\n");
	transmit_string("Press a to move DOWN\r\n");*/
	
	//Display the Dinosaur
	lcd_cmd(0x82);
	lcd_write_string(D);
	
	while(1)
	{
		//Receive a character
		ch = receive_char();
		
		/********************************
		//The Obstacles keep arriving
		//INITIATE the position of the Dinosaur
		//UPDATE the position based on the character received
		********************************/
		switch(ch)
		{
			case 'q': //moveUP(); //Calling the function
								dino=1;
								lcd_cmd(0x0C2);          //Second row 3rd column
								lcd_write_string(space);   // Clear the initail D
								//msdelay(50);
								lcd_cmd(0x82);           //First row 3rd Column
								lcd_write_string(D);   // Write the new D
								//msdelay(50);
								break;
			case 'a': //moveDOWN(); //Calling the function
								dino=2;
								lcd_cmd(0x82);           //First row 3rd column
								lcd_write_string(space);   // Clear the initail D
								//msdelay(50);
								lcd_cmd(0x0C2);          //Second row 3rd Column
								lcd_write_string(D);   // Write the new D
								//msdelay(50);
								break;
			default: transmit_string(line4);
								break;
		}
		msdelay(100);
	}//While loop ends here
}//Main function ends here


void T0_ISR(void) interrupt 1     //Pass all the required arguments 
{
	TR0 = 0; //Stop the Timer
	TF0 = 0; //Clear overflow flag
	
	if(var_T0!=0)
	{
		var_T0--;
	}
	else //Enter after 1 second
	{
		unsigned int m; //will be used later, can define only at the start
		if(state==3) //Call LFSR_next()
		{
			unsigned int i;
			//temp_obstacle = LFSR_next(&LFSR, temp_obstacle);//Find next obstacle and store it in temp_obstacle
			LFSR_next(&LFSR, temp_obstacle); //Will load the next obstacle Array in temp_obstacle
			for(i=0; i<15; i++)//Update the matrix, Shift Left
			{
				row1[i] = row1[i+1];
				row2[i] = row2[i+1];
			}
			row1[15] = temp_obstacle[0]; //Update the last elements (New entries)
			row2[15] = temp_obstacle[3];
			state = 1;
		}
		else if(state==1)
		{
			unsigned int j;
			for(j=0; j<15; j++)//Update the matrix, Shift Left
			{
				row1[j] = row1[j+1];
				row2[j] = row2[j+1];
			}
			row1[15] = temp_obstacle[1]; //Update the last elements (New entries)
			row2[15] = temp_obstacle[4];
			state++;
		}
		else
		{
			unsigned int k;
			for(k=0; k<15; k++)//Update the matrix, Shift Left
			{
				row1[k] = row1[k+1];
				row2[k] = row2[k+1];
			}
			row1[15] = temp_obstacle[2]; //Update the last elements (New entries)
			row2[15] = temp_obstacle[5];
			state++;
		}
		
		//Update Score Here, i.e. score++
		score++;
		
		//Update the Display here
		//RESOLVE the ISSUE of BLINKING!!!!
		for(m=0; m<16; m++)
		{
			
				lcd_cmd(0x80+m); //Place the cusror in First row (m+1)th column
				if(row1[m]==0)
				{
					lcd_write_string(space); //empty " "
				}
				else
				{
					lcd_write_string(star);
				}
				lcd_cmd(0x0C0+m); //Place the cursor in Second row (m+1)th column
				if(row2[m]==0)
				{
					lcd_write_string(space); //empty " "
				}
				else
				{
					lcd_write_string(star);
				}
		}
		//Dinosaur will be vanished, show it again
		if(dino==1)
		{
			lcd_cmd(0x82); //Cursor in First row 3rd column //D may not be in the firts row
			lcd_write_string(D);
		}
		else
		{
			lcd_cmd(0x0C2); //Cursor in First row 3rd column //D may not be in the firts row
			lcd_write_string(D);
		}
		
		//Check for Collision
		collision_check();
		
		var_T0 = 31; //Restore the value
	}
	//Restart T0 HERE
		TH0 = 0x00;
		TL0 = 0x00;
		TR0 = 1; //Start the timer
}