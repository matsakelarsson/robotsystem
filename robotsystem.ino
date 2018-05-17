#include <SPI.h>
#include <LiquidCrystal.h>
#include <Servo.h>

#define pin A0

Servo IRServo; Servo LaserServo;

//Constants
const int buttonPin = 4;     

//Variables
int buttonState = 0;
int modeValue=1;
const int ledPin = 2;
const int buzzerPin = 3;
int angle = 100;
int angle1 = 10;


// initialize the library with the number of the sspin
//(or the latch pin of the 74HC595)
LiquidCrystal lcd(9);
 
void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.print("Attack mode");

  // Set button input
  pinMode(buttonPin, INPUT_PULLUP);  
  Serial.begin (9600);
  pinMode (pin, INPUT); 
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  IRServo.attach(8);
  IRServo.write(angle);
  LaserServo.attach(5);
  LaserServo.write(angle1);

}
 
void loop() {
  delay(100);
  if(buttonState == HIGH){
    modeValue = modeValue + 1;
  }
  delay(100);

  
  for(angle = 90; angle < 160; angle++)  
  {                                  
    IRServo.write(angle);               
    delay(18);                   
  } 
  
  // now scan back from 180 to 90 degrees
  for(angle = 160; angle > 90; angle--)    
  {                                
    IRServo.write(angle);           
    delay(18);       
  } 

  
  if(modeValue % 2 == 0){
    lcd.clear();
    lcd.setCursor(0, 0);

    lcd.print("Friendly mode");

    lcd.setCursor(14, 0);
    lcd.print(modeValue);
  }

  else if(modeValue % 2 == 1){
    lcd.clear();
    lcd.setCursor(0, 0);

    lcd.print("Attack mode");

    lcd.setCursor(13, 0);
    lcd.print(modeValue);

    
  }
  
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:

  //Read button state (pressed or not pressed?)
  buttonState = digitalRead(buttonPin);

  uint16_t value = analogRead (pin);
  uint16_t range = get_gp2d12 (value);
  
  Serial.println (value);
  Serial.print (range);
  Serial.println (" mm");
  Serial.println ();
  if(value > 20){
    lcd.print(range);
    lcd.setCursor(4, 1);
    lcd.print("mm");
    if(modeValue % 2 == 0){
      warning_system();
    }
    if(modeValue % 2 == 1){
      laser_system();
      value=0;
    }
    delay (100);
  }
}

void warning_system(){
  int i = 0;
  while (i <= 10){
      tone(buzzerPin, 100);
      digitalWrite(ledPin, HIGH);
      delay(150);

      noTone(buzzerPin);
      digitalWrite(ledPin, LOW);
      delay(100);
      i = i + 1;
  }
}

void laser_system(){
    for(angle1 = 10; angle1 < 100; angle1++)  
  {                                  
    LaserServo.write(angle1);               
    delay(18);                   
  } 
  
  // now scan back from 180 to 90 degrees
  for(angle1 = 100; angle1 > 10; angle1--)    
  {                                
    LaserServo.write(angle1);           
    delay(18);       
  } 
}

uint16_t get_gp2d12 (uint16_t value) 
{
  if (value < 10) value = 10;
  //return ((67870.0 / (value - 3.0)) - 40.0);
  return value;
}
