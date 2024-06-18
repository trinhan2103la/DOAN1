#include<SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <DHT.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x20 for a 16 
 
#define DHTPIN 25     // what pin we're connected to      // D7
#define DHTTYPE DHT11   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);

int   mb=32;


int   nnc=16; // Push button to select manual or automatic mode
int   nnmb=17; // Push button selects to turn the pump on or off

long    nd,da,dem,ndg,dag;
char    data;

int   ttc,ttmb,ttct,ttmbt,quet;

void setup() 
{
  Serial.begin(9600); 
  pinMode(mb,OUTPUT);
  pinMode(nnc,INPUT);
  pinMode(nnmb,INPUT);
  lcd.init();     // khoi tao lcd
  lcd.backlight();
  dht.begin(); 
  dem=0;
  ttc=0;
  ttmb=0;
  digitalWrite(mb,0);
} 

void loop() 
{
  ktra_nnc();
  da = dht.readHumidity();     
  nd = dht.readTemperature(); 
  if(ttc==0) // Automatic mode
  {
    if(da<80)   // Set the desired threshold value    
    {
      digitalWrite(mb,1);
      ttmb=0;
    }
    else            
    {
      digitalWrite(mb,0);
      ttmb=0;
    }
  }
  else // Manual mode
  {
    ktra_nnmb();
    if(ttmb==1)     digitalWrite(mb,1);  // If manual mode and motor is on
    else            digitalWrite(mb,0);  // If manual mode and motor is off
  }

  hienthi();
  gui_gt();  // Send data to LoRa module
  if(Serial.available()>0)
  {
    data = Serial.read(); 
    if(data=='1')
    {
      ttc=1; // Switch to manual mode
      ttmb=0;
    }
    else if(data=='2')
    {
      ttc=0;  // Switch to automatic mode
      ttmb=0;
    }
    else if(data=='3')
    {
      if(ttc==1)        ttmb=1;
    }
    else if(data=='4')
    {
      if(ttc==1)        ttmb=0;
    }
  }

  dem++;
  if(dem==50)
  {
    ndg=0;
  }
  else if(dem==100)
  {
    dem=0;
    ttct=10;
  }
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

void gui_gt()
{
  if((ndg!=nd)||(dag!=da))
  {
    ndg=nd;
    dag=da; 
    Serial.print('t');
    Serial.write(nd/10%10+48);      //+0x30 chuyen tu thap phan sang ma ascii
    Serial.write(nd%10+48);
    Serial.write(da/10%10+48); 
    Serial.write(da%10+48); 
    Serial.print('.'); 
    delay(100);
  }

  if((ttct!=ttc)||(ttmbt!=ttmb))
  {
    ttct=ttc;
    ttmbt=ttmb;
    Serial.print("k");
    if(ttc==1)      Serial.print("1");
    else            Serial.print("0");
    if(ttmb==1)     Serial.print("1");
    else            Serial.print("0");
    Serial.print(".");
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
      ttmb=0;
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
        ttmb++;
        if(ttmb>1)        ttmb=0;
        gui_gt();
      while(digitalRead(nnmb)==0);
    }
  }
}