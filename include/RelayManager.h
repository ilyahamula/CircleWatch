#pragma once

class RelayManager
{
    RelayManager();
public:
    static RelayManager& inst();

    void OffAll() const;
    void OnAll() const;

    void OnDial() const;
    void OffDial() const;

    void OnLight() const;
    void OffLight() const;
};