int incomingByte = 0; // for incoming serial data

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

String level[4][4] = {{"   x      xx   x",
                       "xx   xxxx    x  ","0","0"},
                       
                      {"x    x    xx   x",
                       "  x    xx    x  ","0","1"},
                       
                      {"xx o       x   x",
                       "   xxxxxx    x  ","0","1"},
                       
                      {"   x    x   x   ",
                       "xxoo  x   x   x ","0","0"}};
                       
int Damage[4][2][16] = {
                       {{0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,1},
                        {1,1,0,0,0,1,1,1,1,0,0,0,0,1,0,0}},
                        
                       {{1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1},
                        {0,0,1,0,0,0,0,1,1,0,0,0,0,1,0,0}},
                        
                       {{1,1,0,2,0,0,0,0,0,0,0,1,0,0,0,1},
                        {0,0,0,1,1,1,1,1,1,0,0,0,0,1,0,0}},
                        
                       {{0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0},
                        {1,1,2,2,0,0,1,0,0,0,1,0,0,0,1,0}},
                        };
int x = 0;
int y = 1;
int xold = 1;
int yold = 1;
int lev = 0;
struct kiste{
        byte x = 0;
        byte y = 0;
       };

void setup()
{
  // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();

  Serial.begin(9600);
  
}

void leveldesign(byte row){
  if(x != xold || y != yold){
    xold = x;
    yold = y; 
    lcd.setCursor(0,0);
    lcd.print(level[row][0]);
    lcd.setCursor(0,1);
    lcd.print(level[row][1]);
  }
  if(Damage[row][y][x] == true){
    lcd.setCursor(0,0);    
    lcd.print("    Verloren     ");
    lcd.setCursor(0,1);
    lcd.print("-----------------");
    delay(1000);
    xold = 20;
    yold = 20; 
    x = level[row][2].toInt();
    y = level[row][3].toInt();
  }  
}

void loop()
{  
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
  
    switch (incomingByte){
      case 48: //forwards
       y--;
       if(y == -1){y=0;}
      break;
      case 49: //backwards
       y++;
       if(y == 2){y=1;}
      break;
      case 50: //right
       x++;
       
       if(x == 16){x=15;}
      break;
      case 51: //left
       x--;
       if(x == -1){x=0;}
      break;
    }
  }
  if(x == 15){
    x = 0;
    lev++;  
  }
  
  leveldesign(lev);
  lcd.setCursor(x,y);
  lcd.print("1");
}
