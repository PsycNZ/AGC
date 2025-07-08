#ifndef _CAUTOPOT_H
#define _CAUTOPOT_H

class CAutoPot {
public:
    bool inZone = false;

    CAutoPot(int csPin, int sensorPin, int samplesToAverage);
    virtual ~CAutoPot();

    void begin(int initialLevel = 127);
    void reset(int level);
    virtual void update() = 0;
    void setAggression(float aggression);
    int getLevel();
    int getSensorValue();

protected:
    void _readSensor();
    void _adjustLevel(int error, int gainDirection);
    
    /**
     * @brief Sets the potentiometer to a new level, handling constraints and writing to the hardware if changed.
     * @param newLevel The desired new level.
     */
    void _setLevel(int newLevel);

    /**
     * @brief Adjusts the potentiometer by a single step.
     * @param direction The direction to step (+1 or -1).
     */
    void _stepLevel(int direction);


    // Member variables for hardware and state
    int _csPin;
    int _sensorPin;
    int _samplesToAverage;
    int _currentLevel = 127;
    int _lastSensorValue = 0;
    float _aggression = 0.01f;

private:
    void _writeToPot(int value);
};

class COffsetPot : public CAutoPot {
public:
    /**
     * @brief Constructor for COffsetPot.
     * @param csPin The chip select pin for the digital potentiometer.
     * @param sensorPin The analog pin used to read the sensor value.
     * @param samples The number of sensor readings to average.
     * @param targetValue The desired DC level for the sensor (e.g., 511 for mid-scale on a 10-bit ADC).
     * @param tolerance The acceptable deviation from the target before correction is applied.
     */
    COffsetPot(int csPin, int sensorPin, int samples, int targetValue, int tolerance);

    /**
     * @brief Overridden update method to implement the offset control logic.
     */
    void update() override;

private:
    int _targetValue;
    int _tolerance;
};

class CGainPot : public CAutoPot {
public:
    /**
     * @brief Constructor for CGainPot.
     * @param csPin The chip select pin for the digital potentiometer.
     * @param sensorPin The analog pin used to read the sensor value.
     * @param samples The number of sensor readings to average.
     */
    CGainPot(int csPin, int sensorPin, int samples);

    /**
     * @brief Overridden update method to implement the gain control logic.
     */
    void update() override;

private:
    // Define the operating zones for the gain controller for a 10-bit ADC (0-1023)
    const int CLIP_LOW = 50;    // Below this is considered clipping/dangerously low.
    const int CLIP_HIGH = 970;  // Above this is considered clipping/dangerously high.
    const int SAFE_LOW = 200;   // Lower bound of the 'safe' zone for increasing gain.
    const int SAFE_HIGH = 820;  // Upper bound of the 'safe' zone for increasing gain.
};

#endif // _CAUTOPOT_H
