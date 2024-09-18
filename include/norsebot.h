#ifndef NORSEBOT_H
#define NORSEBOT_H

// #include <esp_log.h>
#include "norseutils.h"
#include "config/pin.h"
#include "config/constant .h"
// #include "dynamixel.h"
#include "norseprotocol.h"
#include "Dynamixel2Arduino.h"
// #include "norsebotregister.h"

#define WHEEL_FRONT_RIGHT_ID        1
#define WHEEL_FRONT_LEFT_ID         2
#define WHEEL_REAR_LEFT_ID          3
#define WHEEL_REAR_RIGHT_ID         4

// #define MOTOR_DIRECTION_FORWARD     0x00
// #define MOTOR_DIRECTION_BACKWARD    0x01
#define DRIVING_MODE_MANUAL         0x00
#define DRIVING_MODE_AUTO           0x01

#define TAG_PROTOCOL                "Protocol"
#define TAG_DYNAMIXEL               "Dynamixel"
#define TAG_AUTODRIVE               "AutoDrive"

// typedef struct
// {
//     PinName txPin;
//     PinName rxPin;
//     PinName directionPin;
//     uint32_t baudRate;
// } dynamixel_config_t;

// typedef struct
// {
//     PinName txPin;
//     PinName rxPin;
//     uint32_t baudRate;
// } protocol_config_t;

typedef struct
{
    float lengthWheelToCenterX;
    float lengthWheelToCenterY;
    float wheelRadius;
} norsebot_config_t;

typedef struct
{
    uint8_t controlMode;
    float initialPositionFL;
    float initialPositionFR;
    float initialPositionRL;
    float initialPositionRR;
    float presentPositionFL;
    float presentPositionFR;
    float presentPositionRL;
    float presentPositionRR;
    uint16_t currentManualSpeed;
    uint16_t currentManualCommand;
} norsebot_status_t;

class NorseBot
{
    public:
        NorseBot(HardwareSerial& commandPort, HardwareSerial& dynamixelPort, uint8_t directionPin);
        ~NorseBot();

        void init();
        void protocolHandler();
        void updateControl();
        void manualDriveHandler();
        void autoDriveHandler();
        void updatePosition();

        void reset();
        void initNorsebotStatus();

    private:
        void startEngine();
        void stopEngine();

        void stopMoving();
        void moveForward(uint16_t speed);
        void moveBackward(uint16_t speed);
        void moveStraightLeft(uint16_t speed);
        void moveStraightRight(uint16_t speed);
        void moveForwardLeft(uint16_t speed);
        void moveForwardRight(uint16_t speed);
        void moveBackwardLeft(uint16_t speed);
        void moveBackwardRight(uint16_t speed);
        void moveRotateLeft(uint16_t speed);
        void moveRotateRight(uint16_t speed);

        void velocityProfileSquare(float positioningVelocity, float trayMaxVelocity, float targetZone, float distanceX, float distanceY);

    private:
        // Dynamixel* _motor;
        NorseProtocol* _protocol;
        // Thread* _protocolThread;
        HardwareSerial& _commandPort;
        HardwareSerial& _dynamixelPort;
        Dynamixel2Arduino* _motor;

        norse_packet_t _rxPacket;
        norsebot_status_t _norsebotStatus;
        norsebot_config_t _norsebotConfig;

        volatile bool readingThreadRunning = false;
        volatile bool isPacketAvilable = false;

        // Odometry data
        float _odometryPositionX, _odometryPositionY, _odometryPhi;
        float _fwkVelocityX, _fwkVelocityY, _fwkPhi;
        // Target data
        float _targetPositionX, _targetPositionY, _targetPhi;
        float expectedVelocityX, expectedVelocityY, expectedPhi;

        static void protocolThread(void *pvParamter);
        void protocolThreadWorker();
        // void protocolThreadWorker();
        TaskHandle_t taskProtocol;

    private:
        float _IkOrientationOld;
        float _periodS;
    //     uint8_t registerControlMode;
    //     uint8_t registerModeManualCommand;
    //     uint16_t registerModeManualSpeed;
};

#endif