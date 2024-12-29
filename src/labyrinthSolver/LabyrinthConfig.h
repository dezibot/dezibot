#ifndef LABYRINTH_CONFIG_H
#define LABYRINTH_CONFIG_H

#include <cstdint>

class LabyrinthConfig {
public:
    uint16_t getBaseSpeed() const;
    uint16_t getMaxSpeed() const;
    uint16_t getRotateSpeed() const;
    double getRedScale() const;
    double getGreenScale() const;
    int getRotateDuration() const;
    int getMoveStraightDuration() const;
    double getWhiteTolerance() const;

    void setBaseSpeed(uint16_t speed);
    void setMaxSpeed(uint16_t speed);
    void setRotateSpeed(uint16_t speed);
    void setRedScale(double scale);
    void setGreenScale(double scale);
    void setRotateDuration(int duration);
    void setMoveStraightDuration(int duration);
    void setWhiteTolerance(double tolerance);

private:
    uint16_t baseSpeed = 3900;
    uint16_t maxSpeed = 8191;
    uint16_t rotateSpeed = 2500;
    double redScale = 1.00;
    double greenScale = 0.98;
    int rotateDuration = 2000;
    int moveStraightDuration = 1500;
    double whiteTolerance = 0.7;
};

#endif // LABYRINTH_CONFIG_H