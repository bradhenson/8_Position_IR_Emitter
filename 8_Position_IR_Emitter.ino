//*******************************************************************************
// Created: 10/25/2017 
// Author: Bradford Henson
// License: Use at your own risk
//
// The below code uses pin 3 on the Arduino as the output pin for the IR LED
//*******************************************************************************
#define F_CPU 16000000UL

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <util/delay.h>

void IRcarrier(void);
int read_LCD_buttons(int);
void xmitCodeHeader(void);

#define BITtime   560                     // carrier signal standard time
#define btnRIGHT  0                       // Right button (not used currently)
#define btnUP     1                       // Up button (used to enter into setup mode)
#define btnDOWN   2                       // Down button (used to scroll through different routes)
#define btnLEFT   3                       // Left button (not used currently)
#define btnSELECT 4                       // Select button (used to select a route)
#define btnNONE   5                       // No button (not used currently)
#define TIMEOUT 1000                      // Timeout counter time (in cycles, not milliseconds)
#define PIN       3                       // This is the pin the IR LED is connected to
int timeOutCounter = 0;                   // Used through out the setup mode section
int lcd_key     = 0;                      // used by the selectRouteInterface function
int adc_key_in  = 0;                      // used to identify which button has been pressed
int scroll = 2;                           // Starting place for the "-" mark
uint8_t selectedRoute = 1;                // Used in the setup mode section
boolean selectButtonFlag = 0;             // Used in the setup mode section

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

/***************************************************************************
                            SETUP SECTION
***************************************************************************/
void setup()
{
  DDRD = (1 << PIN);                        // Set pin 5 in PortD as an output
  PORTD &= ~(0 << PIN);                      // Set pin 5 in PortD to 0
  lcd.begin(16, 2);                       // start the library
  lcd.setCursor(0,1);                     // Set cursor to position zero on the second row
  lcd.print(F("                "));       // Clears the LCD row
}

/***************************************************************************
                                EVENT LOOP
***************************************************************************/
void loop()                           
{
  lcd.setCursor(0,0);              // Set cursor to position 0 in the first row
  lcd.print(F("  XMIT ROUTE   ")); // Specify that route one is transmitting
  lcd.setCursor(13,0);             // Set cursor to position 13 in the first row
  lcd.print(selectedRoute);        // Specify that route one is transmitting     

/* ***************************************************************************
 *  The following implementation is a brute force PWM. At this stage the CPU
 *  doesn't have anything else to do, so this method is okay. If there were 
 *  other things to do, it would be better to impletment the PWM in hardware
 *  utilizing one of the on chip timers. The advantage of doing it this way, 
 *  is that someone looking at the code can more easily see what is being done.
*******************************************************************************/
switch (selectedRoute)  // This will allow the routes to be cycled through
  {
    case 1:                       // Location A-0 B-0 C-0
      xmitCodeHeader();      
      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(BITtime);         // a LOW is only 1 bit time period
      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(BITtime);         // a LOW is only 1 bit time period   
      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(BITtime);         // a LOW is only 1 bit time period
      IRcarrier(BITtime);         // send a single STOP bit.
      break;
    
    case 2:                       //Location A-0 B-0 C-1
      xmitCodeHeader();      
      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(BITtime);         // a LOW is only 1 bit time period
      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(BITtime);         // a LOW is only 1 bit time period      
      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(3 * BITtime);     // a HIGH is 3 bit time periods      
      IRcarrier(BITtime);         // send a single STOP bit.            
      break;
      
    case 3:                       // Location A-0 B-1 C-0
      xmitCodeHeader();
      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(BITtime);         // a LOW is only 1 bit time period
      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(3 * BITtime);     // a HIGH is 3 bit time periods   
      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(BITtime);         // a LOW is only 1 bit time period      
      IRcarrier(BITtime);         // send a single STOP bit.
      break;
      
    case 4:                       // Location A-0 B-1 C-1
      xmitCodeHeader();
      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(BITtime);         // a LOW is only 1 bit time period
      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(3 * BITtime);     // a HIGH is 3 bit time periods    
      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(3 * BITtime);     // a HIGH is 3 bit time periods            
      IRcarrier(BITtime);         // send a single STOP bit.
      break;
      
    case 5:                       // Location A-1 B-0 C-0
      xmitCodeHeader();

      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(3 * BITtime);     // a HIGH is 3 bit time periods
      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(BITtime);         // a LOW is only 1 bit time period    
      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(BITtime);         // a LOW is only 1 bit time period            
      IRcarrier(BITtime);         // send a single STOP bit.
      break;
      
    case 6:                       // Location A-1 B-0 C-1
      xmitCodeHeader();
      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(3 * BITtime);     // a HIGH is 3 bit time periods
      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(BITtime);         // a LOW is only 1 bit time period    
      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(3 * BITtime);     // a HIGH is 3 bit time periods            
      IRcarrier(BITtime);         // send a single STOP bit.
      break;
      
    case 7:                       // Location A-1 B-1 C-0
      xmitCodeHeader();
      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(3 * BITtime);     // a HIGH is 3 bit time periods
      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(3 * BITtime);     // a HIGH is 3 bit time periods    
      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(BITtime);         // a LOW is only 1 bit time period            
      IRcarrier(BITtime);         // send a single STOP bit.
      break;
      
    case 8:                       // Location A-1 B-1 C-1
      xmitCodeHeader();
      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(3 * BITtime);     // a HIGH is 3 bit time periods
      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(3 * BITtime);     // a HIGH is 3 bit time periods    
      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(3 * BITtime);     // a HIGH is 3 bit time periods            
      IRcarrier(BITtime);         // send a single STOP bit.
      break;                                  
  }
  
  if (read_LCD_buttons() == btnUP) // Enter into setup mode by holding the UP button
  {
   lcd.setCursor(0,0);             // move to the begining of the second line
   lcd.print(F(" ENTERING SETUP "));
   lcd.setCursor(0,1);             // move to the begining of the second line
   lcd.print(F("                "));
   _delay_ms(1800);                // This delay is just to allow people to see the text

  timeOutCounter = 0;              // Set couter to zero
  selectButtonFlag = 0;            // Set flag to zero for setup mode
  
  /* ***************************************************************************
 *  To provide the device with a route selection interface, the SAINSMART LCD 
 *  sheld is used with an Arduino. This provides a decent display output for what
 *  route is being transmitted and shows an activity marker. The buttons on the 
 *  shield are used to enter into a "setup" mode and allow the a user to scroll 
 *  through the available 8 preprogramed routes and select one to transmit.
*******************************************************************************/
  do // The following loop provides the route selection interface during a programming event
  {                    
       if (selectButtonFlag == 1)  //break out of the select route loop
       {
        lcd.setCursor(0,0);
        lcd.print(F("  XMIT ROUTE   ")); // Specify that route one is transmitting
        lcd.setCursor(13,0);
        lcd.print(selectedRoute);
        lcd.setCursor(0,1);              // move to the begining of the second line
        lcd.print(F("                "));
        scroll = 2;
        break;
       }                                     
       else                        // when down button is pressed it will keep cycling
       {                
           switch (selectedRoute)  // This will allow the routes to be cycled through 
           {
                case 1:                       
                    selectRouteInterface(selectedRoute); // Pass selected route 1 to the function
                  break;
                case 2:
                    selectRouteInterface(selectedRoute); // Pass selected route 2 to the function
                  break;
                case 3:
                    selectRouteInterface(selectedRoute); // Pass selected route 3 to the function
                  break;
                case 4:
                    selectRouteInterface(selectedRoute); // Pass selected route 4 to the function
                  break;
                case 5:
                    selectRouteInterface(selectedRoute); // Pass selected route 5 to the function
                  break;
                case 6:
                    selectRouteInterface(selectedRoute); // Pass selected route 6 to the function
                  break;
                case 7:
                    selectRouteInterface(selectedRoute); // Pass selected route 7 to the function
                  break;
                case 8:
                    selectRouteInterface(selectedRoute); // Pass selected route 8 to the function
                  break;
                case 9:
                    if (selectedRoute == 9)
                    {
                      selectedRoute = 1; // By setting the selectedRoute back to 1, the application will cycle back thru
                    }
                  break;                                   
             }
         }        
      timeOutCounter++;                   // Increment the time out counter
     } while (timeOutCounter < TIMEOUT);  // When the counter reaches the defined (at the top) value, break out of loop   
    }  

   _delay_ms(1000);                       //wait 1 second before repeating the sent code

/* ***************************************************************************
 *  The following implementation is an activity mark on the second line of the 
 *  LCD that shows a tic mark every second.
*******************************************************************************/
   if(scroll < 14)
   {
   lcd.setCursor(scroll,1);               // move to the begining of the second line
   lcd.print(F("-"));
   scroll++;
   }
   else
   {
   lcd.setCursor(0,1);                    // move to the begining of the second line
   lcd.print(F("  -             "));      // Starts the "-" back at position 2
   scroll = 3;                            // Starts the "-" at position 3, flows better this way
   }
}

/***************************************************************************
                            FUNCTIONS SECTION
***************************************************************************/

int read_LCD_buttons()
{
  adc_key_in = analogRead(0);            // read the value from the sensor
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons 
  // For V1.1 us this threshold
  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in < 250)  return btnUP;
  if (adc_key_in < 450)  return btnDOWN;
  if (adc_key_in < 650)  return btnLEFT;
  if (adc_key_in < 850)  return btnSELECT;

  return btnNONE;  // when all others fail, return this...
}

void IRcarrier(unsigned int timeinus)
{
  for(int i=0; i < (timeinus / 13); i++)
  {
    PORTD |= (1 << PIN);          // turn on Arduino pin
    _delay_us(13);                //delay for 13us
    PORTD &= ~(1 << PIN);         // turn off Arduino pin
    _delay_us(13);                //delay for 13us
  }
}

 void selectRouteInterface(uint8_t route)
 {                   
   lcd.setCursor(0, 0);                   // Set the initial position of on the LCD
   lcd.print(F("    ROUTE:      "));      // Start on the first line with the word "User "
   lcd.setCursor(11, 0);                  // Set cursor position just after the word user
   lcd.print(route);                      // Display the current selected user 
   lcd.setCursor(0, 1);                   // Set cursor to the start of the second line
   lcd.print(F(" SELECT or DOWN "));      // Prompt user to press the next or enter button
   timeOutCounter = 0;                    // Zero out the time out counter before entering the loop

   while(1)
   {
     lcd_key = read_LCD_buttons();        // read the buttons
     if (lcd_key == btnDOWN)              // if the Down button is pressed, do the following
     {                                    // The pin is compared to 0 because it is being pulled up by the debouce hardware
       selectedRoute = route + 1;         // Set the selectedRoute to the next available one, becuase we want the next route
       _delay_ms(400);                    // Just slows down uC for humans
      break;                              // break out of this while loop
      }
      if (lcd_key == btnSELECT)           //if the select button is pressed, do the following
      {
        selectButtonFlag = 1;             // flags that a route was selected, used to exit the user selection while loop
        selectedRoute = route;            // sets the variable -slectedRoute- to the currently selected route                
        _delay_ms(350);                   // Just slows down uC for humans   
       break;                             // break out of this while loop
       }          
       
       timeOutCounter++;                  // Increment the time out counter
       _delay_ms(10);
       if (timeOutCounter > TIMEOUT)      // When the counter reaches the defined (at the top) value, break out of loop
       {
        lcd.setCursor(0,0);
        lcd.print(F("  XMIT ROUTE   "));  // Specify that route one is transmitting
        lcd.setCursor(13,0);
        lcd.print(selectedRoute);
        lcd.setCursor(0,1);               // move to the begining of the second line
        lcd.print(F("                "));
        scroll = 2;
        break;                             
       }
   }
   return;
 }

 void xmitCodeHeader()
 {
      IRcarrier(9000);            // 9ms of carrier
      _delay_us(4500);            // 4.5ms of silence
      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(BITtime);         // a LOW is only 1 bit time period
      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(BITtime);         // a LOW is only 1 bit time period
      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(BITtime);         // a LOW is only 1 bit time period
      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(BITtime);         // a LOW is only 1 bit time period
      IRcarrier(BITtime);         // turn on the carrier for one bit time
      _delay_us(BITtime);         // a LOW is only 1 bit time period 
 }

