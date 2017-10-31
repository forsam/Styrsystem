// Define digital output pins pins!!
#define SolenoidOneOut 3   // Stop the flow
#define SolenoidTwoOut 5   // Gearswitch minus
#define SolenoidThreeOut 6 // Gearswitch plus

#define EngineRelay 7   // Engine relay

// Define analog input pins
#define SolenoidOneIn A2
#define SolenoidTwoIn A1
#define SolenoidThreeIn A0
#define PressureSensorIn A3
#define PosSensorIn A4

// Used variables
double StartTime;
double ElapsedTime;

double Frequency = 2;          // Hertz
double Ts = 1/Frequency; // Seconds!  

// ActionTypes and input
int Action = -1;
int Type = -1;

// Sensorvariables
double Position = 0;

void setup() {
  // Start the Serial connection!
  Serial.begin(9600);  

  // Attach all the pins!
  pinMode(PosSensorIn, INPUT);
}

void loop() {
  StartTime = millis();

  // Listen for inputs
  listenFunc();

  // Measurements
  measureFunc();
  // Calculations
  
  // Actions
  action();

  delayFunc();
}

// ------------------------------------------------------- //
// Functions Below!! 

// This function delays the loop according to update frequency!
void delayFunc(){
  ElapsedTime = millis() - StartTime;
  if(ElapsedTime < Ts*1000)
  {
    delay(Ts*1000 - ElapsedTime);
  }
}

// Todo
void listenFunc(){
  if (Serial.available() > 0)
  {
      int ByteReceived = Serial.read();
      if(ByteReceived == '1')
      {
        String msg = "something avalaible: ";
        Serial.print(msg);
        Action = ByteReceived - '0';
        Serial.println(Action);
      }
  }
}

void action(){
}

void measureFunc(){
  posSensorFunc();
  pressureSensorFunc();  
}

void posSensorFunc(){
  int tmp = analogRead(PosSensorIn);
  Position = 135*tmp/1024
  Serial.print("Position: ");
  Serial.print(Position);
  Serial.print(" mm, Bit: ");
  Serial.println(tmp);
}

void pressureSensorFunc(){
  
} 
