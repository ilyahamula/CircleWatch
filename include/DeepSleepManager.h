#pragma once

class CircleDial;

class DeepSleepManager
{
    DeepSleepManager();
public:
    static DeepSleepManager& inst();
    void check(CircleDial* dial);

    bool m_isUserBLEConnected;
    bool m_isLighthOn;
    unsigned long m_activeTime;
};