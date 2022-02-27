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
#define DEBUG 1
const uint8_t ButtonInput[] ={4,6,8,10,12,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54};
const uint8_t ButtonOutput[] ={3,5,7,9,11,13,17,19,21,23,25,27,29,31,33,35,37,39,41,43,45,47,49,51,53,55};
uint8_t ButtonInPlay[25] ={0};
uint8_t OutputInPlay[25] = {0};
int NumOfButtons = 0;
unsigned long startMillis = 0;   
unsigned long playTime = 0;   
char playMode = 'A';    
int StartInput = 2;
int StartOutput = 13;
int StartLedState = 0;
int outfieldButtonFlashRate = 500; //  in milliseconds
int randomButton = 0;
int wrongBall = -1;

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
  pinMode(ButtonInput[i],INPUT_PULLUP);
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
}

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

void loop() {
  // int i = 0;
  Serial3.println("A");
  while(playMode == 'A'){  // Blink the Start button slowly and wait for the button to be pushed
    for(int i = 0;i<1000;i++){
      delay(2);
      if(i == 500)digitalWrite(StartOutput,HIGH);
      if(digitalRead(StartInput)==LOW){   // Look to see if someone has mashed the StartButton
        digitalWrite(StartOutput,LOW);
        NumOfButtons = ScanTheOutfield();    //Look to see how many buttons there are
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
        Serial.println("");
        #endif
       playMode = 'B';
        Serial3.println("B");
        i = 1000;   // break from the for loop
      }
    } 
    digitalWrite(StartOutput,LOW);  // make sure the start button light is out
  }
  randomSeed(millis()); // Start making up random numbers
  while(playMode == 'B'){  //randomly flash the buttons in play a random number of times
    int flashCount = random(15,25);
    randomButton = 0;
    for(int i = 0;i < flashCount;i++){
      randomButton = random(0,NumOfButtons);
      digitalWrite(OutputInPlay[randomButton],HIGH);
      delay(outfieldButtonFlashRate);
      #ifdef DEBUG
        Serial.print(OutputInPlay[randomButton],DEC);Serial.print(" ");
      #endif
      digitalWrite(OutputInPlay[randomButton],LOW);
    }
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
    if(millis() > (startMillis + 9990)){    // if timeout send a Y, then start over....
      Serial3.println("T");    
       playTime = (millis() - startMillis);
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
