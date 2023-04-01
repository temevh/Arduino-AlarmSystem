#include <Keypad.h>

//this is for the keypad(number of rows and columns)
const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 3; //three columns

//the led pins
const int gLed = 11;
const int rLed = 10;

//this is the movement sensor pins
const int trigPin = 12;
const int echoPin = 13;

//there was no more space in io pins so we had to put buzzer to an analog pin
const int buzzer = A5;

//millis counts the time that has gone from the start of the program
long time = millis();
long time1 = 0;
long overtime = 0;

float normal_distance;
long duration;
float distance;

//keypad buttons can be changed here
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

//keypad buttons initialization
byte pin_rows[ROW_NUM] = {9, 8, 7, 6};
byte pin_column[COLUMN_NUM] = {5, 4, 3};

//keypad button mapping
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

//setting up password
const String password = "1234";
String input_password;

void setup(){
  Serial.begin(9600);
  input_password.reserve(32);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication

  normal_distance = distance_calculator(); //calculates the distance given by the echo
}

float distance_calculator(){
    // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance in cm
  distance = duration * 0.034 / 2;
  return distance;
}

bool password_checker(){
  //gets the time that has passed since the start of the program
  time1 = millis();
  while(true){
    time = millis();
    overtime = time - time1;   // time passed since someone has triggered the alarm
    char key = keypad.getKey();
    if (overtime >= 10000){     // if the time since triggering the alarm system is greater than ten seconds, the alert starts ringing
        alert();
        break;
    }

    if (key){
      Serial.println(key);
      if(key == '*') {
        input_password = ""; // clear input password
        } 
      else if(key == '#') {   //submit password
        if(password == input_password) {
          Serial.println("password is correct");
          digitalWrite(11, HIGH);
          break;
        } 
        else {
          Serial.println("password is incorrect");
          alert();
          break;
        }
        input_password = "";
      }else {
        input_password += key;
      }

      }//if(key)
  }//while
}//function

void alert(){
  while(true){
    char key2 = keypad.getKey();
    //Serial.println("KEY IS "+ key);
    if (key2){
    Serial.println(key2);
    }

    //an option to shut down the buzzer cause it is annoying(only for testing purposes)
    if (key2 == '5'){
      digitalWrite(10, LOW);
      digitalWrite(11, HIGH);
      noTone(buzzer);
      delay(1000000000);

    //the real buzzering starts
    } else{
      tone(buzzer, 500);
      digitalWrite(10, HIGH);
      delay(200);
      tone(buzzer, 1000);
      digitalWrite(10, LOW);
    }
  }
}

void loop(){
  int new_distance = distance_calculator();
  //if distance change is bigger than 10 centimeters then the alarm sets off
  if (abs(new_distance - normal_distance) > 10 ){
    Serial.println("ERI PITUUS");
    bool checked = password_checker();
    delay(30000);
    if (!checked){
      alert();
    }
    
  }

}