#include <LiquidCrystal.h>

// LCD pin configuration
const int lcd_d4 = 4;
const int lcd_d5 = 3;
const int lcd_d6 = 2;
const int lcd_d7 = 1;
const int lcd_en = 5;
const int lcd_rw = 6;
const int lcd_rs = 7;

void setup()
{
  // Construct with pin configuration
  LiquidCrystal lcd( 
                lcd_rs,
                lcd_rw,
                lcd_en,
                lcd_d4,
                lcd_d5,
                lcd_d6,
                lcd_d7
                );
                
  // Specify dimensions of New Haven display
  lcd.begin( 16, 2 );
  
  // Print messages to both lines
  lcd.setCursor( 0, 0 );
  lcd.print( "hello, world!" );
  lcd.setCursor( 0, 1 );          // Set cursor to next line
  lcd.print( "i am mongol" );
}

void loop()
{
  
}
