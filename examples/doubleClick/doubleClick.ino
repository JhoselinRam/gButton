#include "gButton.h"

//Creates a button connected to Digital Pin 3
//One terminal of the button must be connected to GND and the other to D3
gButton button(3);

void setup(){
  pinMode(LED_BUILTIN, OUTPUT); //Setup the built-in led to demostrate the buttons activaton
  Serial.begin(115200); //Open the serial monitor to see the output

  button.begin(); //Initialize the button

  Serial.print("Default time-out in milliseconds \t");
  Serial.println(button.timeOut());  //You can read the amount of time allowed between activations
  
  button.timeOut(320);  //The time can also be modified using the same method. Its values must be greater than the debounce time.
  
  Serial.print("Modified time-out in milliseconds \t");
  Serial.println(button.timeOut());
}

void loop(){
  /*This method must be called at least ones in the loop function,
  afther a long delay() or inside a user defined loop (for, while, do while)*/
  button.update();

  bool activate;

  /*---------------------------------------------------------------------------
  Uncomment only the functionality that you want to test and comment the other
  ---------------------------------------------------------------------------*/

  activate = button.multiClick();       //Activates when you press the button two consecutive times
  //activate = button.multiClick(3);    //The argument determines the number of times you should press the button to activate it
  //activate = button.multiClickUp();   //Activates when you release the button two consecutive times
  //activate = button.multiClickUp(3);  //The argument determines the number of times you should release the button to activate it

  /*---------------------------------------------------------------------------
  ---------------------------------------------------------------------------*/

  //The result activation is shown in the Serial Monitor and with the built-in LED
  if(activate){
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Multi click detected");
  }
  else{
    digitalWrite(LED_BUILTIN, LOW);
  }
}