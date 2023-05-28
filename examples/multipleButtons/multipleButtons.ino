#include "gButton.h"

//Creates three buttons connected to D3, D4 and D5
//One terminal of the button must be connected to GND and the other to the corresponding Arduino pin
gButton button_1(3), button_2(4), button_3(5);

void setup(){
  Serial.begin(115200); //Open the serial monitor to see the output

  gButton::beginAll(); //Use this syntax to initialize all buttons at the same time
}

void loop(){
  gButton::updateAll(); //Similarly, use this syntax to update all buttons at the same time. You can still use the .update() on each button individually

  bool activate_1;
  bool activate_2;
  bool activate_3;

  //You can check each button independently
  activate_1 = button_1.down();       //Button 1 activates when is pressed
  activate_2 = button_2.sustained();  //Button 2 activates is if keep pressed
  activate_3 = button_3.change();     //Button 3 activates every time is pressed or released

  if(activate_1)
    Serial.println("Button 1 was pressed");

  if(activate_2)
    Serial.println("Button 2 is being sustained");

  if(activate_3)
    Serial.println("Button 3 change");
}