#ifndef iDIGIT_H
#define iDIGIT_H

class Adafruit_NeoPixel;

class Digit
{
    unsigned m_start = 0;
    unsigned m_end = 0;
    bool m_isOn = false;
    unsigned m_red = 0;
    unsigned m_green = 0;
    unsigned m_blue = 0;
    Adafruit_NeoPixel& m_strip;

public:
    Digit(unsigned start, unsigned end, Adafruit_NeoPixel& strip)
        : m_start(start), m_end(end), m_strip(strip) {}
    Digit(unsigned id, Adafruit_NeoPixel& strip)
        : m_start(id), m_end(id), m_strip(strip) {}
    virtual ~Digit() = default;

    bool IsOn() { return m_isOn; }
    void SetColor(unsigned r, unsigned g, unsigned b)
    {
        m_red = r; m_green = g; m_blue = b;
    }

    void TurnOn();
    void TurnOff();

private:
    void SetSpecificPixels(bool on);
};

#endif