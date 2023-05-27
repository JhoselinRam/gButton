/*
||	gButton class
||
||	Author: Jhoselin Adrian Ramirez Montes
||	Version:1.0.0
||	Contact: jhoselin.ramirez92@gmail.com
||
||	This class is part of the Hardware Enclosure group, make no use of the delay
||	function and can be used with the general beginAll and updateAll functions, see
||	hardwareEnclosure.cpp
||
||	Description: This class provides an easy way to work with buttons and other momentary
||	contact switches. It has a debounce filter and can be used with internal pullup or
||	external pulldown or pullup resistor (10kOhm are suggested).
||
||	All times are in milliseconds.
||
||	The Hardware Enclosure is a group of libraries dedicated to simplify at the maximum
||	possible the use of various devices like buttons, leds, etc providing high level 
||	function easy to understand and implement in your projects.
||
||
||	Update History
||	--------------------------------------------------------------------------------------
||	Version 1.0.0 dd/mm/yyyy: First Release.
||	--------------------------------------------------------------------------------------
*/

#ifndef G_BUTTON_H_
#define G_BUTTON_H_

#include<Arduino.h>

//------- Constant definitions --------

//Button State
#define BUTTON_LAST_STATE     0 
#define BUTTON_ACTUAL_STATE   1
#define BUTTON_HAS_CHANGE     2
#define BUTTON_DOWN           3
#define BUTTON_UP             4
#define BUTTON_HAS_MUTI_CLICK 5
#define BUTTON_USE_PULLUP     6

//Default Configurations
#define BUTTON_DEBOUNCE_TIME  50
#define BUTTON_TIME_OUT       300

//-------------------------------------

class gButton{
	public:
		gButton(uint8_t pinNumber, bool pullUp = true);
		void begin();
		void update();
		bool down();	
		bool up();
		bool sustain();
		bool change();
		bool toggle();
		bool toggleUp();
		bool multiClick(uint8_t = 2);
		bool multiClickUp(uint8_t = 2);
		bool multiToggle(uint8_t = 2);
		bool multiToggleUp(uint8_t = 2);
		void debounceTime(uint8_t debounceTime);
		uint8_t debounceTime();
		void timeOut(uint16_t timeOut);
		uint16_t timeOut();
	private:
		uint8_t _pin;                 //Pin on the Arduino board in which the button will be connected to
		uint8_t _debounceTime;        //Time that takes the button to bounce
		uint8_t _state;               //State of the button
		uint8_t _UpCount;             //Counts the multiple clicks
		uint8_t _DownCount;           //Counts the multiple releases
		unsigned long _lastTime;      //Last time since the button was updated
		unsigned long _lastPressTime; //Last time since the button was pressed
		uint16_t _timeOut;            //Time between each click
		
		void _clickCheck(unsigned long); //Utility function, used in the update method

        static gButton* _addresBook;  //Array that stores a reference to all buttons 
};

#endif