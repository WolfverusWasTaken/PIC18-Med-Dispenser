/*
 * File:   Test.c
 * Author: alexi
 * PIC18
 * Created on 8 July, 2020, 10:26 AM
 */


#include <xc.h>
#include "keypad.h"
#include "delays.h"
#include "lcd.h"

//universal values(will be the same in all functions)
int hour, minute, second;
int hourF, minuteF, hourE, minuteE, hourD, minuteD, hourA, minuteA;
int snooze_F = 0, snooze_E = 0, snooze_D = 0;

int fArr[4];

unsigned char ComOpt;


void SetupTime(int setuptime[]);

char int_2_char (unsigned char int1)
{
	char char1;
	switch (int1)
	{
		case 0 : char1 = '0' ; break;
		case 1 : char1 = '1' ; break;
		case 2 : char1 = '2' ; break;
		case 3 : char1 = '3' ; break;
		case 4 : char1 = '4' ; break;
		case 5 : char1 = '5' ; break;
		case 6 : char1 = '6' ; break;
		case 7 : char1 = '7' ; break;
		case 8 : char1 = '8' ; break;
		case 9 : char1 = '9' ; break;
		default : char1 = '?' ;
	}
	return(char1);
}

unsigned char char_2_int (char char1)
{
	unsigned char int1;
	switch (char1)
	{
		case '0' : int1 = 0 ; break;
		case '1' : int1 = 1 ; break;
		case '2' : int1 = 2 ; break;
		case '3' : int1 = 3 ; break;
		case '4' : int1 = 4 ; break;
		case '5' : int1 = 5 ; break;
		case '6' : int1 = 6 ; break;
		case '7' : int1 = 7 ; break;
		case '8' : int1 = 8 ; break;
		case '9' : int1 = 9 ; break;
		default : int1 = 0 ;
	}
	return(int1);
}

void interrupt ISR_Timer0_Int() //Current time display and arithmetic. run code.
{
	int hour10, hour1, minute10, minute1, second10, second1;
    int setup[6];
	//SetupTime(setup);  //Fill in the function that setup the time.
    
    
	if (INTCONbits.TMR0IF == 1)
	{
		
        TMR0H = 0x48;
		TMR0L = 0xE5;

		second = second + 1;
		if ( second > 59 )
		{
			second = 0;
			minute = minute + 1;
			if ( minute > 59 )
			{
				minute = 0;
				hour = hour + 1;
				if ( hour > 23 )
					hour = 0;
			}
		}

		hour10 = hour / 10;
		hour1 = hour - hour10 * 10;

		minute10 = minute / 10;
		minute1 = minute - minute10 * 10;

		second10 = second / 10;
		second1 = second - second10 * 10;

		lcd_write_cmd(0x80);
		lcd_write_data(int_2_char (hour10));
		lcd_write_data(int_2_char (hour1));
		lcd_write_data(':');
		lcd_write_data(int_2_char (minute10));
		lcd_write_data(int_2_char (minute1));
		lcd_write_data(':');
		lcd_write_data(int_2_char (second10));
		lcd_write_data(int_2_char (second1));
        
        
                            
		INTCONbits.TMR0IF = 0;
	}
}

void SetupTime(int setuptime[])
{
	
    unsigned char msgindex, outchar, ctemp;
	int hour10, hour1, minute10, minute1, second10, second1;
	char Message[ ] = "Set time hhmmss:    ";

	lcd_write_cmd(0x80);

	for (msgindex = 0; msgindex < 20; msgindex++)
	{
		outchar = Message[msgindex];
		lcd_write_data(outchar);
	}

	lcd_write_cmd(0xC0); // Move cursor to line 2 position 1
    
    for(int i = 0; i <6 ;i++){
        
        ctemp=getkey(); // waits and get an ascii key number when pressed
        lcd_write_data(ctemp); //display on LCD
        setuptime[i] = char_2_int (ctemp); //stores hhmmss value in an array
        
    }

	hour = setuptime[0] * 10 + setuptime[1];
	minute = setuptime[2] * 10 + setuptime[3];
	second = setuptime[4] * 10 + setuptime[5];
    
	delay_ms(500);
	lcd_write_cmd(0x01);
}

void SetupAlarmTime()
{
	unsigned char msgindex, outchar, ctemp;
	unsigned char hour10, hour1, minute10, minute1;    
    char Message[ ] = "Choose Com F,E or D:";
    

	lcd_write_cmd(0x80);

	for (msgindex = 0; msgindex < 20; msgindex++)
	{
		outchar = Message[msgindex];
		lcd_write_data(outchar);
	}

	lcd_write_cmd(0xC0); // Move cursor to line 2 position 1


    
	ctemp=getkey(); // waits and get an ascii key number when pressed
	lcd_write_data(ctemp); //display on LCD
	ComOpt = ctemp;

    
	delay_ms(500);
	lcd_write_cmd(0x01);
}

void CompartFtime()
{
    unsigned char msgindex, outchar, ctemp;
    char Message[ ] = "Set Com F time hhmm:";

	lcd_write_cmd(0x80);

	for (msgindex = 0; msgindex < 20; msgindex++)
	{
		outchar = Message[msgindex];
		lcd_write_data(outchar);
	}
    
    lcd_write_cmd(0xC0); // Move cursor to line 2 position 1
    
    for(int i = 0; i <4 ;i++){
        
        ctemp=getkey(); // waits and get an ascii key number when pressed
        lcd_write_data(ctemp); //display on LCD
        fArr[i] = char_2_int (ctemp); //stores hhmmss value in an array
        
    }

	hourF = fArr[0] * 10 + fArr[1];
	minuteF = fArr[2] * 10 + fArr[3];
    
    delay_ms(500);
	lcd_write_cmd(0x01);
}

void CompartEtime()
{
	unsigned char msgindex, outchar, ctemp;
    char Message[ ] = "Set Com E time hhmm:";
    int eArr[4];

	lcd_write_cmd(0x80);

	for (msgindex = 0; msgindex < 20; msgindex++)
	{
		outchar = Message[msgindex];
		lcd_write_data(outchar);
	}
    
    lcd_write_cmd(0xC0); // Move cursor to line 2 position 1
    
    for(int i = 0; i <4 ;i++){
        
        ctemp=getkey(); // waits and get an ascii key number when pressed
        lcd_write_data(ctemp); //display on LCD
        eArr[i] = char_2_int (ctemp); //stores hhmmss value in an array
        
    }

	hourE = eArr[0] * 10 + eArr[1];
	minuteE = eArr[2] * 10 + eArr[3];
    
    delay_ms(500);
	lcd_write_cmd(0x01);
}

void CompartDtime()
{
	unsigned char msgindex, outchar, ctemp;
    char Message[ ] = "Set Com D time hhmm:";
    int dArr[4];

	lcd_write_cmd(0x80);

	for (msgindex = 0; msgindex < 20; msgindex++)
	{
		outchar = Message[msgindex];
		lcd_write_data(outchar);
	}
    
    lcd_write_cmd(0xC0); // Move cursor to line 2 position 1
    
    for(int i = 0; i <4 ;i++){
        
        ctemp=getkey(); // waits and get an ascii key number when pressed
        lcd_write_data(ctemp); //display on LCD
        dArr[i] = char_2_int (ctemp); //stores hhmmss value in an array
        
    }

	hourD = dArr[0] * 10 + dArr[1];
	minuteD = dArr[2] * 10 + dArr[3];
    
    delay_ms(500);
	lcd_write_cmd(0x01);
}

void displayDlarmOn()
{
	unsigned char msgindex, outchar, ctemp;
    unsigned char msgindex, outchar;
	char Message[ ] = "Alarm On            ";

	
	for (msgindex = 0; msgindex < 20; msgindex++)
	{
		outchar = Message[msgindex];
		lcd_write_data(outchar);
	}
    
    lcd_write_cmd(0xC0);
}

void displayAlarmOff()
{
	unsigned char msgindex, outchar;
	char Message[ ] = "Alarm Off           ";

	lcd_write_cmd(0xC0);
	for (msgindex = 0; msgindex < 20; msgindex++)
	{
		outchar = Message[msgindex];
		lcd_write_data(outchar);
	}
}

void main(void)   //------------ Main Program  ---------------------------------------------------------------
{
    unsigned char displayUpdated;
    unsigned char msgindex, outchar, ctemp;
	displayUpdated = 1;
	ADCON1 = 0x0F;
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.VCFG0 = 0; 
    
    TRISA = 0b00000000;
    TRISC = 0b00000111;
    
    PORTAbits.RA0 = 0;
    PORTAbits.RA1 = 0;
    PORTAbits.RA2 = 0;
    PORTAbits.RA3 = 0;
    
	lcd_init();
    int setup[6];
    
    
    
	SetupTime(setup);  //Fill in the function that setup the time.
	
	ctemp = '0';//sets ctemp to any value other than 'A'
    while(ctemp != 'A'){   //if A not pressed go back to choosing time
        SetupAlarmTime();  //Fill in the function that setup the alarm time.
        switch(ComOpt){
            
            case 'F': CompartFtime(); break;
        
            case 'E': CompartEtime(); break;
        
            case 'D': CompartDtime(); break;
        }
        
        
        
        char Message[ ] = "Start timer? Press A";
        
        lcd_write_cmd(0x80); // Move cursor to line 1 position 1
        for (msgindex = 0; msgindex < 20; msgindex++)
        {
            outchar = Message[msgindex];
            lcd_write_data(outchar);
        }
        
        char Message1[ ] = "Add timer? Press any";
        lcd_write_cmd(0xC0); // Move cursor to line 2 position 1
        for (msgindex = 0; msgindex < 20; msgindex++)
        {
            outchar = Message1[msgindex];
            lcd_write_data(outchar);
        }
        
        ctemp=getkey(); // waits and get an ascii key number when pressed
        lcd_write_cmd(0x01);
        
    }
	
    //TRISDbits.RD0 = 0;
    T0CON = 0b10000111; // Timer on, 16-bit, Fosc/4, pre-scaler 256
    INTCONbits.GIE = 1;
    INTCONbits.TMR0IE = 1;
    
    
    TMR0H = 0x6D; // always write to TMR0H before TMR0L
    TMR0L = 0x84;
    //hourF = fArr[0] * 10 + fArr[1];
	//minuteF = fArr[2] * 10 + fArr[3];

	while(1)
	{
		
        if((hour == hourF && minute == minuteF) && snooze_F == 0){
            PORTAbits.RA3 = 1; //on buzzer
            PORTAbits.RA0 = 1; //on led
            //PORTDbits.RD2 = 1;
        }
        if(PORTCbits.RC0 == 1){
            snooze_F = 1;
            PORTAbits.RA3 = 0; //on buzzer
            PORTAbits.RA0 = 0; //on led
        }
        
        if((hour == hourE && minute == minuteE) && snooze_E == 0){
            PORTAbits.RA3 = 1; //on buzzer
            PORTAbits.RA1 = 1; //on led
            //PORTDbits.RD2 = 1;
        }
        if(PORTCbits.RC1 == 1){
            snooze_E = 1;
            PORTAbits.RA3 = 0; //on buzzer
            PORTAbits.RA1 = 0; //on led
        }
        
        if((hour == hourD && minute == minuteD) && snooze_D == 0){
            PORTAbits.RA3 = 1; //on buzzer
            PORTAbits.RA2 = 1; //on led
            //PORTDbits.RD2 = 1;
        }
        if(PORTCbits.RC2 == 1){
            snooze_D = 1;
            PORTAbits.RA3 = 0; //on buzzer
            PORTAbits.RA2 = 0; //on led
        }
        
	}
}