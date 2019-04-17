  
int incomingByte = 0; // for incoming serial data

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
bool b12 = 0, b12old = 0;
bool b11 =0, b11old = 0;
bool b10 = 0, b10old = 0;
bool b9 = 0, b9old = 0;

int Damage[5][2][18] = {
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
byte kistenid = 0;
int oldlevelid = 2;
struct kiste{
        byte x = 0;
        byte y = 0;
       } kist[5];

struct kiste Kisten[5];
void setup()
{
  // initialize the LCD
  lcd.init();

  // Turn on the blacklight and print a message.
  lcd.backlight();

  Serial.begin(9600);
  
}

void leveldesign(byte levelid){
    if(levelid != oldlevelid){
      oldlevelid = levelid;
      for(byte neu = 0; neu < 5; neu++){Kisten[neu].x=0;Kisten[neu].y=0;kistenid = 0;}
      for(byte row = 0; row < 2; row++){
        for(byte col = 0; col < 16; col++){
          if(Damage[levelid][row][col] == 2){Kisten[kistenid].x=col; Kisten[kistenid].y = row; kistenid++;}
        }  
      }      
    }
    if(x != xold || y != yold){
    xold = x;
    yold = y; 
    for(byte row = 0; row < 2; row++){
      for(byte col = 0; col < 16; col++){
        lcd.setCursor(col,row);
        if(Damage[levelid][row][col] == 0){lcd.print(" ");}
        if(Damage[levelid][row][col] == 1){lcd.print("x");}
        if(Damage[levelid][row][col] == 2){lcd.print("o");}
      }  
    } 
  }
  if(Damage[levelid][y][x] == true){
    lcd.setCursor(0,0);    
    lcd.print("    Verloren     ");
    lcd.setCursor(0,1);
    lcd.print("-----------------");
    delay(1000);
    xold = 20;
    yold = 20; 
    x = Damage[levelid][1][16];
    y = Damage[levelid][1][17];
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
  for(byte ooo = 0; ooo < 5; ooo++){
    if(Kisten[ooo].x == x && Kisten[ooo].y == y){
      Damage[lev][Kisten[ooo].y][Kisten[ooo].x] = 0;
      if(x != xold){Kisten[ooo].x += (x-xold);
      Damage[lev][Kisten[ooo].y][Kisten[ooo].x] = 2;}else 
      
      if(y != yold){ 
      if((Kisten[ooo].y+1) == 2){Kisten[ooo].y = 0;Damage[lev][Kisten[ooo].y][Kisten[ooo].x] = 2;}
      if((Kisten[ooo].y-1) == -1){Kisten[ooo].y = 1;Damage[lev][Kisten[ooo].y][Kisten[ooo].x] = 2;}
      
      }
    }  
  }
  leveldesign(lev);
  lcd.setCursor(x,y);
  lcd.print("1");
}
