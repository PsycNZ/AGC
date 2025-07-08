#include "CAutoPot.h"
#include "Arduino.h"
#include "SPI.h"

CAutoPot::CAutoPot(int csPin, int sensorPin, int samplesToAverage) {
    _csPin = csPin;
    _sensorPin = sensorPin;
    _samplesToAverage = samplesToAverage > 0 ? samplesToAverage : 1;
}

CAutoPot::~CAutoPot() {}

void CAutoPot::begin(int initialLevel) {
    pinMode(_csPin, OUTPUT);
    digitalWrite(_csPin, HIGH);
    reset(initialLevel);
}

void CAutoPot::reset(int level) {
    _setLevel(level);
}

int CAutoPot::getLevel() {
    return _currentLevel;
}

int CAutoPot::getSensorValue() {
    return _lastSensorValue;
}

void CAutoPot::setAggression(float aggression) {
    _aggression = aggression;
}

void CAutoPot::_readSensor() {
    long totalValue = 0;
    for (int i = 0; i < _samplesToAverage; i++) {
        totalValue += analogRead(_sensorPin);
        delayMicroseconds(50);
    }
    _lastSensorValue = totalValue / _samplesToAverage;
}

void CAutoPot::_adjustLevel(int error, int gainDirection) {
    float adjustment = (float)error * _aggression * (float)gainDirection;
    if (abs(adjustment) >= 1.0f) {
        _setLevel(_currentLevel + (int)adjustment);
    }
}

void CAutoPot::_stepLevel(int direction) {
    _setLevel(_currentLevel + direction);
}

void CAutoPot::_setLevel(int newLevel) {
    int previousLevel = _currentLevel;
    _currentLevel = constrain(newLevel, 1, 254);
    if (previousLevel != _currentLevel) {
        _writeToPot(_currentLevel);
    }
}

void CAutoPot::_writeToPot(int value) {
    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
    digitalWrite(_csPin, LOW);
    SPI.transfer(0x00);
    SPI.transfer(value);
    digitalWrite(_csPin, HIGH);
    SPI.endTransaction();
}
