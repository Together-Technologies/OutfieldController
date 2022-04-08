/*Interactive Baseball Game 
  Outfield controller
   The controller interfaces with 17 or more outfield illuminated push buttons,
  an illuminated start button, and the score board display monitor
   Opperation
  1. Show a picture on the display screne 
  and slowly flash the Start Button to attract attention  [serial.print"A", read start-button]
  2. When the Start Button is pushed
    a. show game start on the display    [serial.print"B"]
    b. Scan the ports for connected outfield buttons
    c. randomly flash the outfield buttons at a rate specified
    d. after a random period of time between () and ()
    e. stop the random flashing and illuminate 1 outfield button
    f. start recording the time in milliseconds up to 10 seconds.
      [serial.print"C"]
    g. when the player hits an illuminated button
      if the button is the illuminated one
        play a sound "yay"
        stop recording the time and display the time on the screne for a period of time
      if the button is not the illuminated one 
        play a sound "Boo"
        continue recording time
    h. if either the button is pushed or the time runs out
      hold the time on the screne for 2 minutes
      return to step 2. */

const char Description;
//#define DEBUG 1
const uint8_t ButtonInput[] ={24,26,28,30,32,34,36,38,40,42,44,46,48,50,52};
const uint8_t ButtonOutput[] ={25,27,29,31,33,35,37,39,41,43,45,47,49,51,53};
uint8_t ButtonInPlay[15] ={0};
uint8_t OutputInPlay[15] = {0};
int NumOfButtons = 0;
unsigned long startMillis = 0;   
unsigned long playTime = 0;   
char playMode = 'A';    
int StartInput = 22;
int StartOutput = 23;
bool StartLedState = 1;
int StartLedBlinkCount = 0;
int outfieldButtonFlashRate = 90; //  in milliseconds
int introButtonFlashRate = 300;
int randomButton = 0;
int randSelect;
int wrongBall = -1;
int sparkleArray[20]={0};
int flashCount;

int ScanTheOutfield(void){  
  //look for all button inputs that are being pulled high (button connected)
  int ButtonCount = 0;
  for(int i = 0;i <= sizeof(ButtonInput);i++){
    if(digitalRead(ButtonInput[i])==HIGH){
      ButtonInPlay[ButtonCount] = ButtonInput[i];
      OutputInPlay[ButtonCount] = ButtonOutput[i];
      ++ButtonCount;
    }
  }
  return ButtonCount;
}

void setup() {
pinMode(StartOutput, OUTPUT);
Serial.begin(9600);
Serial3.begin(9600);
while (!Serial) {
  ; // wait for serial port to connect. Needed for native USB port only
}
#ifdef DEBUG
  Serial.print("numberof inputs ");
  Serial.println(sizeof(ButtonInput),DEC);
#endif
  // Initailize the IO pins for the Start button
pinMode(StartInput, INPUT_PULLUP);
pinMode(StartOutput, OUTPUT);
  // Initailize the array of pins considered to be inputs in order
for(int i=0;i < sizeof(ButtonInput);i++){
  pinMode(ButtonInput[i],INPUT);
  #ifdef DEBUG
    Serial.print(i,DEC);
    Serial.print(" ");
    Serial.println(ButtonInput[i],DEC);
  #endif
  }
  // Initailize the array of pins considered to be outputs in order
for(int i=0;i <= sizeof(ButtonOutput);i++){
  pinMode(ButtonOutput[i],OUTPUT);
  digitalWrite(ButtonOutput[i],LOW);
  }
NumOfButtons = ScanTheOutfield();    //Look to see how many buttons there are
}


void loop() {
  // int i = 0;
  Serial3.println("A");
  while(playMode == 'A'){  // Blink or turn on the Start button and wait for the button to be pushed
//    for(int i = 0;i < flashCount; i++){
  digitalWrite(StartOutput,StartLedState);
  ++StartLedBlinkCount;
  if(StartLedBlinkCount > 8){  //the flash rate of the start button
    StartLedState = !(StartLedState);
    StartLedBlinkCount = 0;
  }
      randSelect = random(15); //larger num = fewer, smaller num = more ball flashes
      if(randSelect < NumOfButtons){
        sparkleArray[randSelect] = 20;  // the duration of a light
        randomButton = randSelect;
      }
      for(int j = 0;j < NumOfButtons;j++){
        if(sparkleArray[j]){
          digitalWrite(OutputInPlay[j],HIGH);
          sparkleArray[j]--;
        }else{digitalWrite(OutputInPlay[j],LOW);}
      }
      delay(introButtonFlashRate);
//    }
    
//    for(int i = 0;i<1000;i++){
//      delay(2);
//      if(i == 500)digitalWrite(StartOutput,HIGH);
      if(digitalRead(StartInput)==LOW){   // Look to see if someone has mashed the StartButton
        digitalWrite(StartOutput,LOW);
        for(int j = 0;j < NumOfButtons;j++){ // clear the field
          digitalWrite(OutputInPlay[j],LOW); 
        }
        NumOfButtons = ScanTheOutfield();    //Look again to see how many buttons there are
        #ifdef DEBUG
        Serial.println(NumOfButtons,DEC);  //if DEBUG print all the active Buttons and Outputs
        for(int i = 0;i < NumOfButtons;i++){
          Serial.print(ButtonInPlay[i],DEC);
          Serial.print(" ");
        } 
        Serial.println("");
        for(int i = 0;i < NumOfButtons;i++){
          Serial.print(OutputInPlay[i],DEC);
          Serial.print(" ");
        } 
        Serial.println("  B");
        #endif
       playMode = 'B';
        Serial3.println("B");
//        i = 1000;   // break from the for loop
      }
//    } 
    digitalWrite(StartOutput,LOW);  // make sure the start button light is out
  }
  randomSeed(millis()); // Start making up random numbers
  while(playMode == 'B'){  //randomly flash the buttons in play a random number of times
    flashCount = random(90,150);
    for(int i = 0;i < flashCount; i++){
      randSelect = random(30); //larger num = fewer, smaller num = more ball flashes
      if(randSelect < NumOfButtons){
        sparkleArray[randSelect] = 3;  // the duration of a light
        randomButton = randSelect;
      }
      for(int j = 0;j < NumOfButtons;j++){
        if(sparkleArray[j]){
          digitalWrite(OutputInPlay[j],HIGH);
          sparkleArray[j]--;
        }else{
          digitalWrite(OutputInPlay[j],LOW);
        }
      }
      delay(outfieldButtonFlashRate);
    }
    for(int j = 0;j < NumOfButtons;j++){
      digitalWrite(OutputInPlay[j],LOW); 
    }
//    int flashCount = random(15,25);
//    randomButton = 0;
//    for(int i = 0;i < flashCount;i++){
//      randomButton = random(0,NumOfButtons);
//      digitalWrite(OutputInPlay[randomButton],HIGH);
//      delay(outfieldButtonFlashRate);
//      #ifdef DEBUG
//        Serial.print(OutputInPlay[randomButton],DEC);Serial.print(" ");
//      #endif
//      digitalWrite(OutputInPlay[randomButton],LOW);
//    }
    playMode = 'C';
  }
    Serial3.println("C");
    digitalWrite(OutputInPlay[randomButton],HIGH);  // Turn on the last random light
    startMillis = millis(); // record the start milliseconds
    #ifdef DEBUG
      Serial.print("C ");
      Serial.println("Start Time push button 26");
      randomButton = 1;
    #endif
    wrongBall = -1;
  while(playMode == 'C'){  // wait for the last random button to be pushed
    playTime = (millis() - startMillis);
    Serial3.println(playTime,DEC);   // Send the score time
    if(digitalRead(ButtonInPlay[randomButton])== LOW){  // if the right button is pushed show the time
      playTime = (millis() - startMillis);
      Serial3.print("D");
      Serial3.println(playTime,DEC);   // Send the score time
      playMode = 'D';
      #ifdef DEBUG
        Serial.print("D ");
        Serial.println(playTime,DEC);
        randomButton = 1;
      #endif
    }
    for(int i = 0;i < NumOfButtons;i++){ // if the wrong button is pushed continue
      if(i != randomButton){
        if(digitalRead(ButtonInPlay[i])==LOW){
          if(wrongBall != i){
          Serial3.println("X");    // player has caught the wrong ball "Waaaa waaaaa"
          #ifdef DEBUG
          Serial.println(ButtonInPlay[i],DEC);    // player has caught the wrong ball "Waaaa waaaaa"
          #endif
          }
          wrongBall = i; // record the wrog ball so it doesn't play over and over
        }
      }
    } 
    if(millis() > (startMillis + 9900)){    // if timeout send a T, then start over....
      Serial3.println("T");    
       playTime = 9999;  // was (millis() - startMillis);
      Serial3.println(playTime,DEC);   // Send the score time
      playMode = 'D'; 
      Serial3.print("D");
    }
    delay(10);
  }
for(int i=0;i <= sizeof(ButtonOutput);i++){  // clear the field. Turn off all the outputs
  pinMode(ButtonOutput[i],OUTPUT);
  digitalWrite(ButtonOutput[i],LOW);
  }
  while(playMode == 'D'){   // wait 10 sec and start again
    Serial3.print("s");
    digitalWrite(OutputInPlay[randomButton],LOW);  // turn off the light
    delay(10000);
    playMode = 'A';
  }

delay(100);
} 

/*

const long StartBlinkInterval = 2000;      
int StartBlinkState = LOW;             // ledState used to set the LED
 unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= StartBlinkInterval) {
      previousMillis = currentMillis;
      if (ledState == LOW) {
        StatrLedState = HIGH;
        Serial.println("A");  // send playplayMode A to the display
      } else {
        StartLedState = LOW;
      }
      digitalWrite(StartOutput,StartLedState);
    }


*/
