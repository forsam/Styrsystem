// Define digital output pins pins!!
#define Solenoid1Out 3   // Stop the flow
#define Solenoid2Out 5   // Gearswitch minus
#define Solenoid3Out 6 // Gearswitch plus

#define EngineRelay 7   // Engine relay

// Define analog input pins
#define Solenoid1In A2
#define Solenoid2In A1
#define Solenoid3In A0
#define PressureSensorIn A3
#define PosSensorIn A4

// Used variables
double StartTime; //used for frequency control
double ElapsedTime; //Frequency control

double Frequency = 200;          // Hertz
double Ts = 1/Frequency; // Seconds!  

// Sensorvariables
double Position = 0; //[mm] Positon will be updated with sensordata
double Position1 = 0; //[mm] Piston is fully retracted
double Position2 = 10; // [mm] Neutral
double Position3 = 20; // [mm] Piston is fully extracted
double RefPosition = 0; // [mm] Is updated whith command
double Pressure = 0; // [Bar] Is updated with sensordata
long LowPressure = 45; //[Bar] Pump starts at this pressure
long HighPressure = 60; //[Bar] Pump stops at this pressure

//boolean variables for control
boolean MotorOn = false; //Determines if the motorrelay should be on/off


void setup() {
  // Start the Serial connection!
  Serial.begin(115200);
  
  // Attach all the Input pins!
  pinMode(PosSensorIn, INPUT);
  pinMode(PressureSensorIn, INPUT);
  pinMode(Solenoid1In, INPUT);
  pinMode(Solenoid2In, INPUT);
  pinMode(Solenoid3In, INPUT);

  // Attach all the Output pins
  pinMode(Solenoid1Out,OUTPUT);
  pinMode(Solenoid2Out,OUTPUT);
  pinMode(Solenoid3Out,OUTPUT);
  pinMode(EngineRelay,OUTPUT);
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

// This function delays the loop to achieve a stable update frequency
void delayFunc(){
  ElapsedTime = millis() - StartTime;
  if(ElapsedTime < Ts*1000)
  {
    delay(Ts*1000 - ElapsedTime);
  }
}

// Todo (Takes input from user)
void listenFunc(){
  String ReadString = "";
  while (Serial.available())
  {
    if (Serial.available() > 0)
    {
      char C = Serial.read();
      ReadString += C;
    }
  }
  if (ReadString[0] == 'C' && ReadString.length() == 2)
  {
    if (ReadString[1] == '1')
    {
      RefPosition = Position1;
    }
    else if (ReadString[1] == '2')
    {
      RefPosition = Position2;
    }
    else if (ReadString[1] == '3')
    {
      RefPosition = Position3;
    }
    Serial.print("Refposition: ");
    Serial.println(RefPosition);
  }
  else if (ReadString[0] == 'P' && ReadString.length() == 4)
  {
    String Value = ReadString.substring(2);
    long NumberValue = Value.toInt();
    if (NumberValue != 0 && NumberValue > 29 && NumberValue < 100)
    {
      if (ReadString[1] == 'L')
      {
        if (NumberValue < HighPressure)
        {
          LowPressure = NumberValue;
          Serial.print("LowPressure: ");
          Serial.println(LowPressure);
        }
        else
        {
          Serial.print("LowPressure must be lower than HighPressure: ");
          Serial.println(HighPressure);
        }
      }
      else if (ReadString[1] == 'H')
      {
        if (NumberValue > LowPressure)
        {
          HighPressure = NumberValue;
          Serial.print("HighPressure: ");
          Serial.println(HighPressure);
        }
        else
        {
          Serial.print("HighPressure must be higher than LowPressure: ");
          Serial.println(LowPressure);
        }
      }
    }
  }
  
}

//Todo (executes commands based on input and controllers)
void action(){
  engineControl();
}

//Turns on/off the relay to control the pump based on pressure sensor data.
void engineControl()
{
  if (MotorOn)
  {
    if (Pressure >= HighPressure)
    {
      MotorOn = false;
      digitalWrite(EngineRelay, LOW);
    }
  }
  else
  {
    if (Pressure <= LowPressure)
    {
      MotorOn = true;
      digitalWrite(EngineRelay, HIGH);
    }
  }
  
}

//Reads and stores inputs from sensors
void measureFunc(){
  posSensorFunc();
  pressureSensorFunc();  
}

//Reads the positonssensor and translates it into a position
void posSensorFunc(){
  int tmp = analogRead(PosSensorIn);
  Position = tmp/8.0;
  /*
  Serial.print("Position: ");
  Serial.print(Position);
  Serial.print(" mm, Bit: ");
  Serial.println(tmp);
  */
}

void pressureSensorFunc(){
  int tmp analogRead(PressureSensorIn);
  Pressure = tmp;
} 
