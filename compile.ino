unsigned long timer1 = 0;
int incomingByte = 0; // for incoming serial data

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
bool b12 = 0, b12old = 0;
bool b11 =0, b11old = 0;
bool b10 = 0, b10old = 0;
bool b9 = 0, b9old = 0;
bool jumper = 0;
int level[5][2][18] = {
                       {{0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,1},
                        {1,1,0,0,0,1,1,1,1,0,0,0,0,1,0,0,
                        0,0}},
                        
                       {{1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1},
                        {0,0,1,0,0,0,0,1,1,0,0,0,0,1,0,0,
                        0,1}},
                        
                       {{1,1,0,2,0,0,0,0,0,0,0,1,0,0,0,1},
                        {0,0,0,1,1,1,1,1,1,0,0,0,0,1,0,0
                        ,0,1}},
                        
                       {{0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0},
                        {1,1,2,2,0,0,1,0,0,0,1,0,0,0,1,0
                        ,0,0}},
                        
                       {{1,1,0,2,0,0,2,0,0,0,1,0,1,0,0,0},
                        {0,0,0,0,1,2,0,1,1,0,2,0,0,0,1,0
                        ,0,1}}
                        };
int x = 0;
int y = 1;
int xold = 1;
int yold = 1;
int lev = 0;
void setup()
{
  // initialize the LCD
  lcd.init();

  // Turn on the blacklight and print a message.
  lcd.backlight();

  Serial.begin(9600);
  timer1 = millis();
}

void leveldesign(byte levelid){
    if(x != xold || y != yold  || (timer1+1000) <= millis()){
    
    xold = x;
    yold = y; 
    if(lev == 0 && (timer1+1000) <= millis()){
      timer1 = millis();
      if(jumper == 0){
      jumper += 1;
      level[0][0][10] = 0;
      level[0][1][10] = 1; 
      }else {
      jumper -= 1;
      level[0][0][10] = 1;
      level[0][1][10] = 0; 
      } 
      Serial.print(jumper);
    }
    for(byte row = 0; row < 2; row++){
      for(byte col = 0; col < 16; col++){
        lcd.setCursor(col,row);
        if(level[levelid][row][col] == 0){lcd.print(" ");}
        if(level[levelid][row][col] == 1){lcd.print("x");}
        if(level[levelid][row][col] == 2){lcd.print("o");}
      }  
    } 
  } 
}

void loop()
{  
  b12old = b12;
  b11old = b11;
  b10old = b10;
  b9old = b9;
  b12 = digitalRead(12); 
  b11 = digitalRead(11); 
  b10 = digitalRead(10); 
  b9 = digitalRead(9); 
  if(b12 == HIGH && b12old != b12){x++;if(x == 16){x=15;}}
  if(b11 == HIGH && b11old != b11){y--;if(y == -1){y=0;}}
  if(b10 == HIGH && b10old != b10){y++;if(y == 2){y=1;}}
  if(b9 == HIGH && b9old != b9){x--;if(x == -1){x=0;}}
  if(x == 15){
    x = 0;
    lev++;  
  }
  switch(level[lev][y][x]){
    case 1:
      lcd.setCursor(0,0);    
      lcd.print("    YOU DIED    ");
      lcd.setCursor(0,1);
      lcd.print("----------------");
      delay(1000);
      xold = 20;
      yold = 20; 
      x = level[lev][1][16];
      y = level[lev][1][17];
      break;
    case 2:
      if(x!= xold && level[lev][y][x+1] == 1 || y != yold &&  level[lev][y+1][x] == 1 || x!= xold && level[lev][y][x-1] == 1 || y != yold &&  level[lev][y-1][x] == 1){
        x = xold;
        y = yold;
      }else 
      if(x!= xold && level[lev][y][x+1] == 2 || y != yold &&  level[lev][y+1][x] == 2 || x!= xold && level[lev][y][x-1] == 2 || y != yold &&  level[lev][y-1][x] == 2){
        x = xold;
        y = yold;
      }else{
        level[lev][y][x] = 0;
        if(y != yold){
          if(y == 0){level[lev][y+1][x] = 2;}
          if(y == 1){level[lev][y-1][x] = 2;}
        }
        if(x != xold){
          level[lev][y][(x+(x-xold))] = 2;
        }
      }
    default: break;  
  }
  leveldesign(lev);
  lcd.setCursor(x,y);
  lcd.print("1");
}
