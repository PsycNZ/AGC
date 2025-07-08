#include "CAutoPot.h"
#include "math.h"

CGainPot::CGainPot(int csPin, int sensorPin, int samples)
  : CAutoPot(csPin, sensorPin, samples) {}

void CGainPot::update() {
    // Get a new, stable sensor reading.
    _readSensor();

    // Determine if the signal is in a danger zone (clipping or near rails).
    bool isClippingLow = _lastSensorValue < CLIP_LOW;
    bool isClippingHigh = _lastSensorValue > CLIP_HIGH;

    // --- Priority 1: Prevent Clipping ---
    // If the signal is too high or too low, we MUST reduce the gain.
    if (isClippingLow || isClippingHigh) {
        // We are "out of zone" if clipping is occurring.
        inZone = false;

        // Calculate the error relative to the nearest clip point.
        int error = isClippingHigh ? (_lastSensorValue - CLIP_HIGH) : (CLIP_LOW - _lastSensorValue);
        
        // For gain, we want to decrease the pot level to decrease amplification.
        // The relationship is direct, so we use a gain direction of -1 to subtract from the level.
        _adjustLevel(error, -1);

    }
    // --- Priority 2: Maximize Swing (Increase Gain) ---
    // If we are not in danger, check if we are in a safe zone to boost the signal.
    else if (_lastSensorValue > SAFE_LOW && _lastSensorValue < SAFE_HIGH) {
        // We are "in zone" because the signal is stable and not clipping.
        inZone = true;
        
        // To gently increase gain, we can treat the center of the safe zone as a target.
        // The error will be small, leading to a very small, gradual increase in gain.
        // This prevents the gain from running away.
        int midSafeZone = (SAFE_HIGH + SAFE_LOW) / 2;
        int error = abs(_lastSensorValue - midSafeZone);

        // We want to increase the pot level to increase amplification.
        // The relationship is direct, so we use a gain direction of +1 to add to the level.
        _adjustLevel(error, +1);
    }
    // --- Otherwise, do nothing ---
    // If the signal is between the clip and safe zones, it's best to hold the gain steady.
    else {
        inZone = true; // Still considered "in zone" as we are not clipping.
    }
}
