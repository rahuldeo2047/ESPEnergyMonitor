#ifndef DATA_H
#define DATA_H

enum gScript_type
{
    hb,
    powerOn,
    reConnected,
    motorStats,
    test
};

enum gScript_motor_status
{
    ON,
    OFF,
    UNKNOWN
};

typedef gScript_type gScript_type;
typedef gScript_motor_status gScript_motor_status;

#endif // DATA_H
