#pragma once

class RelayManager
{
    RelayManager();
    RelayManager(const RelayManager&) = delete;
    RelayManager& operator=(const RelayManager&) = delete;
public:
    static RelayManager& inst();

    void OffAll() const;
    void OnAll() const;

    void OnDial() const;
    void OffDial() const;

    void OnLight() const;
    void OffLight() const;
};