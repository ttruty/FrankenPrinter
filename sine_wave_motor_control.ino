
#define enA 9
#define in1 6
#define in2 7
#define INTERVAL 1000   // time between reads in millis
#define RUNTIME 120000   // time to run the motor in millis
#define STOPTIME 30000   // atop time in millis
//#define LEN_SINE_ARRAY 64  
//uint16_t sinWave[LEN_SINE_ARRAY];

int val; // sine reading
int dir; // direction to run motor

int sampleCount = 0;
bool isReverse = false;
//int test = 90;
//len of sine array for testing
int testArray[] = {
  //640, 500, 385, 325, 275, 235, 210, 190, 170, 155, 145, 133, 125, 115, 108, 102, 96, 90
  //88, 86, 84, 82, 80, 78, 76, 74, 72, 70, 68, 66, 64, 62, 60, 58, 56, 54, 52, 50
  //8, 11, 13, 20, 32, 42, 58, 62, 64, 72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 98, 111, 128, 148, 156
  //8, 42, 80,
  24, 25, 26, 27, 28, 29, 30, 31,32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44
}; 
int testCount = 21;

int sensorPin = 0;    // The potentiometer is connected to
                      // analog pin 0

void setup()
{
  pinMode(enA, OUTPUT); //pulse width modulation pin
  pinMode(in1, OUTPUT); // high is left
  pinMode(in2, OUTPUT); // high is right
  
  // Set initial rotation direction
  digitalWrite(in1, LOW); 
  digitalWrite(in2, HIGH);
  Serial.begin(9600);
}

void reverseMotor(int pwnOutput){
  /* 
   *  reserse the motor direction
   *  uses the pwm output from the sine function
   */
    if (dir == 0){ 
      //Serial.println("LEFT <<<<<<<<<<<<<<<<<<<<<<<<<");
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      dir = 1;
      }
    else if (dir == 1) {
      //Serial.println("RIGHT >>>>>>>>>>>>>>>>>>>>>>>>");
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      dir = 0;
    }
    analogWrite(enA, pwnOutput);
    isReverse = true;
}

void runMotor(int pwnOutput) {
  /* 
   *  use whatever the currently direction is
   *  set the pwm and run motor at that voltage
   */
  analogWrite(enA, pwnOutput);
}

void stopMotor(int waitPeriod) {
  /* 
   *  Move the motor to the left end of carriage then
   *  stop the motor for a set amount of currentTime 
   */
  analogWrite(enA, 180);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  delay(1000);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  delay(400);
  dir = 1;
  analogWrite(enA, 0);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  delay(waitPeriod);
}

void loop(){
  static uint16_t phase; // use to potentiometer
  int sensorValue; // use to potentiometer
  
  int test;
//  sensorValue = analogRead(sensorPin);  // for use for testing wih potentiometer
//  phase = (map(sensorValue, 0, 1023, 1, 2046)); // for use for testing wih potentiometer
  for (int thisTest = 0; thisTest < testCount; thisTest++)
    {
      //Serial.println("Test number:  ", thisTest) 
      unsigned long currentTime = millis();
      test = testArray[thisTest];
      while(millis() <  currentTime + RUNTIME)
      {
        //if(test <= 1000) // this is about the max for len of rail
          uint16_t sinWave[test]; // set the sin wave array to the len test
          
          for (int16_t i = 0; i < (test); i++)
          {
            int pwmOutput; // pulse width modulation to control motor movement/speed
            
            float angle = TWO_PI * i / test; // division of sine wave according to current test iter
            int16_t val = sin(angle) * (1024 - 1); // calculate sine wave
            val += 1024;
            sinWave[i] = val; 
            pwmOutput = map(val, 0, 2047, 125, 255); // mapping higher than 0 to ensure motor is still moving as need to meet a voltage thresh to move,
            Serial.println(pwmOutput);               
            //Serial.println(val);
            
            if(pwmOutput != 125){
              runMotor(pwmOutput); // if not the min of wave run the motor
              isReverse = false;
            }
            else{   
              if (isReverse == false){
                reverseMotor(pwmOutput); // if min reverse the motor
              }   
            }
            sampleCount++; 
          }
        }
//    else
//    {
//      while(true)
//      {
//        //no nothing if test is higher than set max,
//      }
//    }
    float hertz = ((float(sampleCount)/float(test))/ float(RUNTIME/1000)) / 2.0;  //IS THIS THE CORRECT WAY TO GET HERTZ OUT OF TEST???
    Serial.println();
    Serial.println("Test Speed");
    Serial.println(test);
    Serial.println("Hz:");
    Serial.println(hertz);
    Serial.println();
    sampleCount = 0;
    //test++;
    stopMotor(STOPTIME); // stop motor for set second
  }
  while(true)
  {
    //no nothing if test is higher than set max,
  }
}
