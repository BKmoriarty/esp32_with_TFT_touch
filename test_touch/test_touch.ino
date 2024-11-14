#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup()
{
    Serial.begin(115200);

    // pinMode(14, OUTPUT);

    tft.init();
    tft.setRotation(1);

    // Use this calibration code in setup():
    // uint16_t calData[5] = {280, 3600, 256, 3561, 7};
    // tft.setTouch(calData);

    tft.fillScreen(tft.color24to16(0xFFFFFF));
    tft.fillCircle(100, 100, 60, tft.color24to16(0xC3C3C3));
    tft.fillRect(0, 200, 240, 120, tft.color24to16(0x1ABC9C));
    tft.fillRect(240, 200, 240, 120, tft.color24to16(0xE74C3C));

    tft.setTextSize(6);
    tft.setTextColor(tft.color24to16(0x3B3B3B));
    tft.setTextDatum(ML_DATUM);
    tft.drawString("GPIO14", 200, 100);

    tft.setTextSize(4);
    tft.setTextColor(tft.color24to16(0xFFFFFF));
    tft.setTextDatum(MC_DATUM);
    tft.drawString("ON", 120, 260);
    tft.drawString("OFF", 360, 260);
}

void loop()
{
    uint16_t x, y;
    if (tft.getTouch(&x, &y))
    {
        Serial.printf("Touched at (%d, %d)\n", x, y);
        if ((x > 265 && x < 320) && (y > 0 && y < 240))
        {
            // digitalWrite(14, HIGH);
            tft.fillCircle(100, 100, 60, tft.color24to16(0x27AE60));
        }
        else if ((x > 240 && x < 480) && (y > 200 && y < 320))
        {
            // digitalWrite(14, LOW);
            tft.fillCircle(100, 100, 60, tft.color24to16(0xC3C3C3));
        }
    }
    delay(10);
}