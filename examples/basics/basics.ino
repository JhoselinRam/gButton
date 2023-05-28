#include "gButton.h"

//Creates a button connected to Digital Pin 3
//One terminal of the button must be connected to GND and the other to D3
gButton button(3);

void setup(){
  pinMode(LED_BUILTIN, OUTPUT); //Setup the built-in led to demostrate the buttons activaton
  Serial.begin(115200); //Open the serial monitor to see the output

  button.begin(); //Initialize the button

  Serial.print("Default debounce time in milliseconds \t");
  Serial.println(button.debounceTime());  //You can read the time used in the debounce filter
  
  button.debounceTime(55);  //The time can also be modified using the same method
  
  Serial.print("Modified debounce time in milliseconds \t");
  Serial.println(button.debounceTime());
}

void loop(){

  bool activate;


  /*-------------------------------------------------------------------------
  Uncomment only the functionality that you want to test and comment the rest
  -------------------------------------------------------------------------*/
 
  activate = button.down();         //Activates only when the button is pressed           
  //activate = button.up();         //Activates only when the button is released
  //activate = button.sustained();  //Activates as long you keep the button pressed
  //activate = button.change();     //Activates every time the button goes from being pressed to released and vice versa
  //activate = button.toggle();     //Toggles between true and false every time the button is pressed
  //activate = button.toggleUp();   //Toggles between true and false every time the button is released

  /*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

  //The result activation is shown in the Serial Monitor and with the built-in LED
  if(activate){
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Button activated");
  }
  else{
    digitalWrite(LED_BUILTIN, LOW);
  }
}