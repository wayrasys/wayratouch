/*
 Name:		WayraTouch.cpp
 Created:	31/07/2017 19:24:46
 Author:	Wayra Sys - Gustavo
 Editor:	http://www.visualmicro.com
*/

#include "WayraTouch.h"

WayraTouch::WayraTouch(int imput, int output)
{
	_imput = imput;
	_output = output;

	_autosave = false;
	_memonyInit = 0;
	_menonyState = 1;
	_state = 0;
	*p_state = 0;
	_showDebugState = 0;
	_lastState = LOW;

	pinMode(_imput, INPUT);
	//pinMode(_output, OUTPUT);
}

void WayraTouch::Init() {
	print("Inicializando...", true);
	if (_autosave) {
		if (EEPROM.read(_memonyInit) == true) {
			print("Inicializado", true);
			_state = EEPROM.read(_menonyState);
		}
		else {
			print("Iniciualizando", true);
			_lastState = LOW;
			_state = LOW;
			EEPROM.write(_memonyInit, 1);
			EEPROM.write(_menonyState, LOW);
		}
	}
	print("Estado",true);
}

void WayraTouch::setAutoSave(bool autosave) {
	_autosave = autosave;
}

void WayraTouch::setAddress(int menoryposition) {
	_memonyInit = menoryposition;
	_menonyState = _memonyInit++;
}

int WayraTouch::getAddress() {
	return _memonyInit;
}

void WayraTouch::setImput(int imput) {
	_imput = imput;
}

int WayraTouch::getImput() {
	return _imput;
}

void WayraTouch::setOutPut(int output) {
	_output = output;
}

int WayraTouch::getOutPut() {
	return _output;
}

void WayraTouch::setDebug(bool show) {
	_showDebugState = show;
}

//void WayraTouch::Listen() {
void WayraTouch::Listen(void *handleListen(bool)){
	if (digitalRead(_imput) == HIGH) {
		if (_lastState == LOW) {
			_lastState = HIGH;
			if (getState() == LOW) {
				setState(HIGH);
				handleListen(HIGH);
			}
			else {
				setState(LOW);
				handleListen(LOW);
			}
		}
	}else {
		_lastState = LOW;
	}
}

void WayraTouch::setState(bool state) {
	_state = state;
	*p_state = state;
	if (state == HIGH) {
		print("PRENDIDO", true);
	}else {
		print("APAGADO", true);
	}
	if (_autosave) {
		EEPROM.write(_menonyState, state);
	}
}

void WayraTouch::setGlobalState(bool *state) {
	*p_state = *state;
}

bool WayraTouch::getState() {
	return _state;
}

void WayraTouch::print(char* msg, bool newline) {
	if (_showDebugState) {
		Serial.print("WayraTouch::");
		if (newline) {
			Serial.println(msg);
		}
		else {
			Serial.print(msg);
		}
	}
}