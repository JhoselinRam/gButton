/*
	gButton class

	Author: Jhoselin Adrian Ramirez Montes
	Version: 2.0.0
	Contact: jhoselin.ramirez92@gmail.com

	Description: This class provides an easy way to work with buttons and other momentary
	contact switches. It implements various functionalities like detecting when the button
	is press, released, a toggle and even double or multiple clicks.
	
	It makes no use of the delay() function and implements a debounce filter.
	It can be used with the internal pin pull-up without extra configuration, simplifying
	the wiring or, if preferred, can use an external resistor (10kOhm are suggested).

	All times are in milliseconds.
*/




#include "Arduino.h"
#include "gButton.h"

//-------------- Costructor -------------------

/*
Creates a gButton object.
 
gButton myButton(pinNumber, pullUp);

pinNumber: a positive integer representing the pin in which the button
will be connected to.

pullUp (optional): a boolean indicating if the internal pull up resistor
will be used. 
Default value: true.

Returns: a new gButton object.
*/
gButton::gButton(uint8_t pinNumber, bool pullUp){
    //Initialize all variables
    _pin = pinNumber;
    _debounceTime = BUTTON_DEBOUNCE_TIME;
    _timeOut = BUTTON_TIME_OUT;
    _lastTime = 0;
    _lastDownTime = 0;
    _lastUpTime = 0;
    _state = 0;
    _upCount = 0;
    _downCount = 0;
    _set(BUTTON_USE_PULLUP, pullUp);   

    _addToAddressBook(this);
}                                                   

//---------------------------------------------
//---------------- Begin ----------------------

/*
Set the correct mode on the button pin. 
This method need to be called ones in the setup function.

Returns: no returns
*/
void gButton::begin(){
    if(_get(BUTTON_USE_PULLUP)){
    	pinMode(_pin, INPUT_PULLUP);
        return;
    }
		Serial.println("no Pullup");
    pinMode(_pin, INPUT);
}

//---------------------------------------------
//---------------- Update ---------------------

/*
Update the timers and the button state.

This method need to be called at least ones in the loop function, after any (long) 
delay() or at the top of any user defined loop (for, while, do while) if the button 
object is used inside that loop.

Returns: no returns.
*/
void gButton::update(){

    unsigned long currentTime = millis();
    _set(BUTTON_HAS_CHANGE, false);

    //Debounce filter
    if (currentTime - _lastTime > (unsigned long)_debounceTime){
        //Update the last and current state
        bool read = _get(BUTTON_USE_PULLUP) ? !digitalRead(_pin) : digitalRead(_pin);
        
        _set(BUTTON_LAST_STATE, _get(BUTTON_CURRENT_STATE));
        _set(BUTTON_CURRENT_STATE, read);
        _lastTime = currentTime;                                           

        //If a change occur update the corresponding state
        if (_get(BUTTON_CURRENT_STATE) != _get(BUTTON_LAST_STATE))
            _set(BUTTON_HAS_CHANGE, true); 

        //If the button is pressed toogle the corresponding state
        if(down())
            _set(BUTTON_TOGGLE, !_get(BUTTON_TOGGLE));

        //If the button is released toogle the corresponding state
        if(up())
            _set(BUTTON_TOGGLE_UP, !_get(BUTTON_TOGGLE_UP));
    }

    _downCounter(currentTime); //Used to check if a multiple click occurs
    _upCounter(currentTime);   //Used to check if a multiple up click occurs
}

//---------------------------------------------
//------------- Down Counter ------------------

/*
Private function that is used in update() to keep the _downCount variable.

_downCounter( currentTime )

currentTime: The current time in milliseconds.

Returns: no returns.
*/
void gButton::_downCounter(unsigned long currentTime){
    //If no click is detected and the time runs out, reset the counter.
    if(!down()){
        if(currentTime - _lastDownTime > _timeOut)
            _downCount = 0;
        return;
    }

    //If the click is done in time, increase the counter.
    if(currentTime - _lastDownTime <= _timeOut){
        _downCount++;
        _lastDownTime = currentTime;
        return;
    }

    //Otherwise set the counter to one.
    _downCount = 1;
    _lastDownTime = currentTime;
}

//---------------------------------------------
//-------------- Up Counter -------------------

/*
Private function that is used in update() to keep the _upCount variable.

_upCounter( currentTime )

currentTime: The current time in milliseconds.

Returns: no returns.
*/
void gButton::_upCounter(unsigned long currentTime){
    //If no click is detected and the time runs out, reset the counter.
    if(!up()){
        if(currentTime - _lastUpTime > _timeOut)
            _upCount = 0;
        return;
    }

    //If the click is done in time, increase the counter.
    if(currentTime - _lastUpTime <= _timeOut){
        _upCount++;
        _lastUpTime = currentTime;
        return;
    }

    //Otherwise set the counter to one.
    _upCount = 1;
    _lastUpTime = currentTime;
}

//---------------------------------------------
//----------------- Down ----------------------

/*
Cheeks if the button has been pressed since the last time it was updated

Returns: a boolean.
*/
bool gButton::down(){
    return _get(BUTTON_HAS_CHANGE) && _get(BUTTON_CURRENT_STATE) && !_get(BUTTON_LAST_STATE);
}

//---------------------------------------------
//----------------- Up ------------------------

/*
Cheeks if the button has been released since the last time it was updated.

Returns: a boolean.
*/
bool gButton::up(){
    return _get(BUTTON_HAS_CHANGE) && !_get(BUTTON_CURRENT_STATE) && _get(BUTTON_LAST_STATE);
}

//---------------------------------------------
//--------------- Sustained -------------------

/*
Cheeks if the button is being continually pressed.

Return: a boolean.
*/
bool gButton::sustained(){
    return _get(BUTTON_CURRENT_STATE);
}

//---------------------------------------------
//---------------- Change ---------------------

/*
Cheeks whether the button have change status, passing from HIGH to LOW or vice versa

Return: a boolean.
*/
bool gButton::change(){
    return _get(BUTTON_HAS_CHANGE);
}

//---------------------------------------------
//---------------- Toggle ---------------------

/*
Toggles between true and false every time a down() call would return true

Returns: a boolean.
*/
bool gButton::toggle(){
    return _get(BUTTON_TOGGLE);
}

//---------------------------------------------
//--------------- Toggle Up -------------------

/*
Toggles between true and false every time a up() call would return true

Returns: a boolean.
*/
bool gButton::toggleUp(){
    return _get(BUTTON_TOGGLE_UP);
}

//---------------------------------------------
//-------------- Multi Click ------------------

/*
This method will return true if you press the button 'numberOfClicks' times 
consecutively with 'timeBetweenClicks' milliseconds between each press. 
	  
timeBetweenClicks has a default value of 300ms.

multiClick( numberOfClicks )

numberOfClicks(optional): The number of times that you must press the 
button until the method returns true.
Default value: 2 (This emulates a double click behavior)

Returns: a boolean.
	
Note: If you call this method multiple times between each update(), only the first one or the one with the smallest
'numberOfClicks' argument will be able to return true, the others will always return false.
*/
bool gButton::multiClick(uint8_t numberOfClicks){
    //If the number of clicks is reached.
    if (_downCount == numberOfClicks){
        _downCount = 0; //Restart the counter             
        return true;                         
    }                           
    
    return false;                        
}

//---------------------------------------------
//------------- Multi Click Up ----------------

/*
This method will return true if you release the button 'numberOfClicks' times 
consecutively with 'timeBetweenClicks' milliseconds between each press. 
	  
timeBetweenClicks has a default value of 300ms.

multiClickUp( numberOfClicks )

numberOfClicks(optional): The number of times that you must press the 
button until the method returns true.
Default value: 2 (This emulates a double click behavior)

Returns: a boolean.
	
Note: If you call this method multiple times between each update(), only the first one or the one with the smallest
'numberOfClicks' argument will be able to return true, the others will always return false.
*/
bool gButton::multiClickUp(uint8_t numberOfClicks){
    //If the number of releases is reached.
    if (_upCount == numberOfClicks){
        _upCount = 0; //Restart the counter             
        return true;                         
    }                           
    
    return false;                        
}

//---------------------------------------------
//------------- Debounce Time -----------------

/*
Set the debounce time used in the filter.

debounceTime( time )

time: The time in milliseconds.

Returns: No returns.
*/
void gButton::debounceTime(uint16_t time){	
	_debounceTime = time;
}



/*
Get the debounce time used in the filter.

uint16_t debounceTime()

Returns: The time of the debounce filter in milliseconds
*/
uint16_t gButton::debounceTime(){	
	return _debounceTime;
}

//---------------------------------------------
//--------------- Time Out --------------------

/*
Set the time out used on the multiClick() method.

timeOut( time )

time: The time in milliseconds.

Note: The time-out value must be greater than the debounce time.

Returns: No returns.
*/
void gButton::timeOut(uint16_t time){	
	_timeOut = time;
}



/*
Get the time out used on the multiClick() method.

uint16_t timeOut()

Returns: The time out used on the multiClick() method in milliseconds.
*/
uint16_t gButton::timeOut(){	
	return _timeOut;
}

//---------------------------------------------
//----------------- Set -----------------------

/*
Helpler function that set the state of the button to the especified value
*/
void gButton::_set(uint8_t property, bool value){
    bitWrite(_state, property, value);
}

//---------------------------------------------
//----------------- Get -----------------------

/*
Helpler function that get the state of the button
*/
bool gButton::_get(uint8_t property){
    return bitRead(_state, property);
}



//---------------------------------------------
//--------- Static implementations ------------


//------------- Address Book ------------------

/*
keep a reference to all button objects
*/
gButton** gButton::_addresBook = new gButton*[1]; //Addres book initialized with one space

/*
Amount of objects stored in the address book
*/
uint8_t gButton::_length = 0;

//---------------------------------------------
//---------- Add To Address Book --------------

void gButton::_addToAddressBook(gButton* newButton){
    if(_length == 0){
        _addresBook[0] = newButton;
        _length++;
        return;
    }

    _length++;
    gButton** temporal = _addresBook;

    _addresBook = new gButton*[_length];

    for(uint8_t i=0; i<_length-1; i++)
        _addresBook[i] = temporal[i];
    
    _addresBook[_length - 1] = newButton;

    delete[] temporal;
    temporal = 0;

}

//---------------------------------------------
//-------------- Begin All --------------------

void gButton::beginAll(){
    for(uint8_t i=0; i<_length; i++)
        _addresBook[i]->begin();
}

//---------------------------------------------
//------------- Update All --------------------

void gButton::updateAll(){
    for(uint8_t i=0; i<_length; i++)
        _addresBook[i]->update();
}