#include<SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

char data,chay,l;
char    dl[16]="";

int   nnc=16;
int   nnmb=17;
int   ttc,ttmb,ttct,ttmbt,quet;

long  nd,da;
int   gui;

void setup()
{
    Serial.begin(9600);
    pinMode(nnc,INPUT);
    pinMode(nnmb,INPUT);
    lcd.init();     // khoi tao lcd
    lcd.backlight();
    chay=0; 
    quet=0;
}
void loop()
{
  if(Serial.available() > 0)
  {
    data = Serial.read(); 
    if(data=='.')
    {
      if(dl[0]=='t')        // nhiet do vuon
      {
        nd=(dl[1]-0x30)*10+(dl[2]-0x30);
        da=(dl[3]-0x30)*10+(dl[4]-0x30);
        Serial.print("ok"); 
      }
      else if(dl[0]=='k')        // nhiet do vuon
      {
        ttc=(dl[1]-0x30);
        ttmb=(dl[2]-0x30);
        Serial.print("tt"); 
      }
      for(l=0;l<16;l++)
      {
        dl[l]=0x20;
      }
      chay=0;
    }
    else
    {
      dl[chay]=data;
      chay++;
    }
  }
  ktra_nnc();
  ktra_nnmb();
  gui_gt();
  quet++;
  delay(1);
  if(quet==100)
  {
    ttct=10;
  }
  else if(quet==200)
  {
    ttmbt=10;
    quet=0;
  }
  hienthi();
}

void hienthi()
{
  lcd.setCursor(0,0);
  lcd.print("ND:");
  lcd.print(nd);      //+0x30 chuyen tu thap phan sang ma ascii
  lcd.write(0xdf);
  lcd.print("C   ");
  
  lcd.setCursor(10,0);
  lcd.print("DA:");
  lcd.print(da);
  lcd.print("%");

  if(ttc==1)
  {
    lcd.setCursor(0,1);
    lcd.print("T/CONG");
  }
  else
  {
    lcd.setCursor(0,1);
    lcd.print("T/DONG");
  }

  if(ttmb==1)
  {
    lcd.setCursor(10,1);
    lcd.print("MB:BAT");
  }
  else
  {
    lcd.setCursor(10,1);
    lcd.print("MB:TAT");
  }
}

void ktra_nnc()
{
  if(digitalRead(nnc)==0)
  {
    delay(20);
    if(digitalRead(nnc)==0)
    {
      ttc++;
      if(ttc>1)        ttc=0;
      gui_gt();
      while(digitalRead(nnc)==0);
    }
  }
}
void ktra_nnmb()
{
  if(digitalRead(nnmb)==0)
  {
    delay(20);
    if(digitalRead(nnmb)==0)
    {
      if(ttc==1)
      {
        ttmb++;
        if(ttmb>1)        ttmb=0;
      }
      gui_gt();
      while(digitalRead(nnmb)==0);
    }
  }
}

void gui_gt()
{
  if((ttct!=ttc))
  {
    ttct=ttc;
    if(ttc==1)      Serial.print("1");
    else            Serial.print("2");
  }

  if((ttmbt!=ttmb))
  {
    ttmbt=ttmb;
    if(ttmb==1)      Serial.print("3");
    else             Serial.print("4");
  }
}