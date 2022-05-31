#include "DHT.h"
#define DHTPIN 12 
#define DHTTYPE DHT11
#include <LiquidCrystal_I2C.h>

// clk pin
#define encoder0PinA 2
// dt pin
#define encoder0PinB 3
// btn pin
#define encoder0Btn 4

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27,16,2);

int control_state=0;
int HeaterRelayPin = 8;
int FanRelayPin = 9;
int min_set_temp=15;
int max_set_temp=50;
int set_temperature = 20;
float current_temperature = dht.readTemperature();
bool control_temperature = false;


bool increase_temperature=false;
bool decrease_temperature=false;

void doEncoder()
{
  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB))
  {
    if(set_temperature >min_set_temp){
      decrease_temperature=true;

    }
  }
  else
  {
 
      if(set_temperature <max_set_temp){
        increase_temperature=true;
    
    }
  }

}

void setup()
{

pinMode(HeaterRelayPin, OUTPUT);
pinMode(FanRelayPin, OUTPUT);

digitalWrite(HeaterRelayPin, HIGH);
digitalWrite(FanRelayPin, HIGH);

pinMode(encoder0PinA, INPUT_PULLUP);
pinMode(encoder0PinB, INPUT_PULLUP);
pinMode(encoder0Btn, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(encoder0PinA), doEncoder, CHANGE);

  // Setup Serial Monitor
  Serial.begin(9600);


  
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
 
  
  lcd.setCursor(0,0);
  lcd.print("I&M Project");
  lcd.setCursor(0,1);
  lcd.print("Ctrl by button clk");





  Serial.println(F("DHTxx test!"));

  dht.begin();

  delay(1000);
}

void refresh_lcd(){
  
lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set Temp:");
  lcd.print(set_temperature);
   if (control_temperature){
  lcd.print(" Ctrl");
   }
  
  lcd.setCursor(0,1);
  lcd.print("Temp:");
  lcd.print(current_temperature);
  lcd.print(F("C"));

  if (control_state == 1){
    lcd.print(F(" Heat"));
  }else if(control_state == 2){
  lcd.print(F(" Fan"));
  }else{
    lcd.print(F(" OFF"));
  }
  
  

}



void loop()
{


    current_temperature = dht.readTemperature();



refresh_lcd();

  // Put in a slight delay to help debounce the reading

      int encoder_btn = digitalRead(encoder0Btn);

    if(not encoder_btn){
      if (control_temperature){
        control_temperature=false;
      }else{
      control_temperature=true;
      }
    }

if (control_temperature){


if(set_temperature-current_temperature > 2){
  // turn on heater
        digitalWrite(HeaterRelayPin, LOW);
digitalWrite(FanRelayPin, HIGH);
control_state=1;
  
}else if(current_temperature-set_temperature > 2){
  // turn on fan
        digitalWrite(HeaterRelayPin, HIGH);
digitalWrite(FanRelayPin, LOW);
control_state=2;
}else{
      digitalWrite(HeaterRelayPin, HIGH);
digitalWrite(FanRelayPin, HIGH);
control_state=0;
}

  
  }else{
    digitalWrite(HeaterRelayPin, HIGH);
digitalWrite(FanRelayPin, HIGH);
control_state=0;
  }

if(increase_temperature){
   increase_temperature=false;
           set_temperature++;
}
         

        if(decrease_temperature){
             decrease_temperature=false;
 set_temperature--;

        }

    
  delay(100);

}
