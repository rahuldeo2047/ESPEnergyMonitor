#ifndef DATA_H
#define DATA_H


#define STR(x) #x
#define XSTR(x) STR(x)

#define _VER_ XSTR(VER)

enum gScript_type
{
    hb,
    powerOn,
    reConnected,
    motorStats,
    test,
    unknown
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
