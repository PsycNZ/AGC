#include "CAutoPot.h"
#include "math.h"

COffsetPot::COffsetPot(int csPin, int sensorPin, int samples, int targetValue, int tolerance)
  : CAutoPot(csPin, sensorPin, samples), _targetValue(targetValue), _tolerance(tolerance) {
}

void COffsetPot::update() {
    // Get a new, stable sensor reading.
    _readSensor();

    // Calculate the error: how far are we from our target?
    int error = _lastSensorValue - _targetValue;

    // Update the inZone flag. We are "in zone" if the error is within our tolerance.
    inZone = (abs(error) <= _tolerance);

    // If we are outside the tolerance window, apply a correction.
    if (!inZone) {
        // The relationship between the offset pot and sensor reading is typically inverse.
        // Increasing the pot's wiper value might decrease the sensor voltage.
        // We use a gain direction of -1 to account for this.
        // If the relationship is direct, this should be +1.
        _adjustLevel(error, +1);
    }
}
