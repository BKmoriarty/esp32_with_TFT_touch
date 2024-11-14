#include <TFT_eSPI.h>

#include "iconUp.h"
#include "iconDown.h"

TFT_eSPI tft = TFT_eSPI();

TFT_eSprite upSprite = TFT_eSprite(&tft);
TFT_eSprite downSprite = TFT_eSprite(&tft);

bool modeAuto = false;

float pHCurrent = 6.86;

float pHSetPoint = 5.8;
float pHRange = 0.3;

float feedTime = 2.0;
float stopTime = 10.0;
bool mixing = false;

float prePHSetPoint = pHSetPoint;
float prePpHRange = pHRange;

float preFeedTime = feedTime;
float preStopTime = stopTime;
bool preMixing = mixing;

void setup()
{
    Serial.begin(115200);
    tft.init();

    tft.setRotation(1);
    tft.setSwapBytes(true);
    //    fill screen
    tft.fillScreen(tft.color24to16(0x000000));

    tft.setTextSize(5);
    tft.setTextColor(tft.color24to16(0xFFFFFF));
    tft.setTextDatum(CC_DATUM);

    tft.drawString("HELLO!", 270, 100);
    tft.drawString("Welcome", 260, 140);

    delay(5000);
}

int currentPage = 1;
bool isShowMainScreen = false;
bool isShowPHsetPoint = false;
bool isShowCalibrate = false;
bool isShowSettingFeed = false;

void loop()
{
    switch (currentPage)
    {
    case 1:
        showMainScreen();
        break;
    case 2:
        showPHsetPoint();
        break;
    case 3:
        showCalibrate();
        break;
    case 4:
        showSettingFeed();
        break;
    default:
        // reset go to home screen
        currentPage = 1;
    }

    delay(50);
}

void changeMode()
{
    tft.fillRect(0, 0, 240, 32, tft.color24to16(0x72FEFD));

    tft.drawFastHLine(45, 20, 150, tft.color24to16(0x000));
    tft.drawFastHLine(45, 19, 150, tft.color24to16(0x000));
    tft.drawFastHLine(45, 18, 150, tft.color24to16(0x000));
    if (modeAuto)
    {
        tft.drawEllipse(60, 19, 10, 10, tft.color24to16(0x000));
        tft.drawEllipse(60, 19, 11, 11, tft.color24to16(0x000));
        tft.fillCircle(60, 19, 10, tft.color24to16(0xafafaf));
        Serial.println("Pause");
        modeAuto = false;
    }
    else
    {
        tft.drawEllipse(180, 19, 10, 10, tft.color24to16(0x000));
        tft.drawEllipse(180, 19, 11, 11, tft.color24to16(0x000));
        tft.fillCircle(180, 19, 10, tft.color24to16(0x3D9CF8));
        Serial.println("Auto");
        modeAuto = true;
    }
}

void showMainScreen()

{
    if (isShowMainScreen)
    {
        Serial.println("showing main screen");
        uint16_t x, y;
        if (tft.getTouch(&x, &y))
        {
            Serial.printf("Touched at (%d, %d)\n", x, y);
            if ((x > 0 && x < 240) && (y > 230 && y < 320))
            {
                changeMode();
            }
            else if ((x > 240 && x < 480) && (y > 120 && y < 320))
            {
                currentPage = 2;
            }
            else if ((x > 0 && x < 240) && (y > 100 && y < 180))
            {
                currentPage = 3;
            }
            else if ((x > 0 && x < 480) && (y > 0 && y < 100))
            {
                currentPage = 4;
            }
        }
    }
    else
    {
        //    fill screen
        tft.fillScreen(tft.color24to16(0x000000));

        // ========================================================================== //
        tft.fillRect(0, 0, 240, 220, tft.color24to16(0x72FEFD));

        tft.drawFastHLine(45, 20, 150, tft.color24to16(0x000));
        tft.drawFastHLine(45, 19, 150, tft.color24to16(0x000));
        tft.drawFastHLine(45, 18, 150, tft.color24to16(0x000));

        tft.drawEllipse(60, 19, 10, 10, tft.color24to16(0x000));
        tft.drawEllipse(60, 19, 11, 11, tft.color24to16(0x000));
        tft.fillCircle(60, 19, 10, tft.color24to16(0xafafaf));

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0x000));
        tft.setTextDatum(TL_DATUM);
        tft.drawString("PAUSE", 32, 40);

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0x000));
        tft.setTextDatum(TL_DATUM);
        tft.drawString("AUTO", 165, 40);

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0x000));
        tft.setTextDatum(TL_DATUM);
        tft.drawString("PH", 20, 80);

        tft.setTextSize(5);
        tft.setTextColor(tft.color24to16(0x000));
        tft.setTextDatum(MC_DATUM);
        tft.drawFloat(pHCurrent, 2, 120, 120);

        tft.fillRoundRect(55, 166, 129, 32, 15, tft.color24to16(0xEEEEEE));

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0x989997));
        tft.setTextDatum(MC_DATUM);
        tft.drawString("CALIBRATE", 120, 185);

        // ========================================================================== //
        tft.fillRect(240, 0, 240, 220, tft.color24to16(0xFFFDA8));

        tft.fillRoundRect(300, 25, 119, 28, 15, tft.color24to16(0x989997));

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0xFFFFFF));
        tft.setTextDatum(MC_DATUM);
        tft.drawString("Setting", 360, 40);

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0x000));
        tft.setTextDatum(TL_DATUM);
        tft.drawString("Setpoint", 260, 70);

        tft.setTextSize(3);
        tft.setTextColor(tft.color24to16(0x000));
        tft.setTextDatum(TR_DATUM);
        tft.drawFloat(pHSetPoint, 1, 460, 80);

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0x000));
        tft.setTextDatum(TL_DATUM);
        tft.drawString("Range", 260, 130);

        tft.setTextSize(3);
        tft.setTextColor(tft.color24to16(0x000));
        tft.setTextDatum(TR_DATUM);
        tft.drawString("+/- " + String(pHRange), 460, 147);

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0x000));
        tft.setTextDatum(TR_DATUM);
        tft.drawString(String((pHSetPoint - pHRange)) + "-" + String((pHSetPoint + pHRange)), 460, 183);

        // ========================================================================== //
        tft.fillRect(0, 220, 480, 100, tft.color24to16(0x3D9CF8));

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0x000));
        tft.setTextDatum(TL_DATUM);
        tft.drawString("Pump Status", 35, 232);

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0x000));
        tft.setTextDatum(TL_DATUM);
        tft.drawString("DOSING", 20, 262);

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0xD9534F));
        tft.setTextDatum(TC_DATUM);
        tft.drawString("OFF", 125, 262);

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0x000));
        tft.setTextDatum(TL_DATUM);
        tft.drawString("CIRCUL", 20, 290);

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0xFFFFFF));
        tft.setTextDatum(TC_DATUM);
        tft.drawString("ON", 125, 290);

        tft.fillRoundRect(150, 265, 110, 28, 15, tft.color24to16(0x989997));

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0xFFFFFF));
        tft.setTextDatum(TC_DATUM);
        tft.drawString("Setting", 205, 271);

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0x000));
        tft.setTextDatum(TC_DATUM);
        tft.drawString("Feed", 300, 229);

        tft.setTextSize(3);
        tft.setTextColor(tft.color24to16(0x000));
        tft.setTextDatum(TC_DATUM);
        tft.drawNumber(feedTime, 300, 260);

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0x000));
        tft.setTextDatum(TC_DATUM);
        tft.drawString("Stop", 420, 229);

        tft.setTextSize(3);
        tft.setTextColor(tft.color24to16(0x000));
        tft.setTextDatum(TC_DATUM);
        tft.drawNumber(stopTime, 420, 260);

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0x000));
        tft.setTextDatum(TL_DATUM);
        tft.drawString("Pump Loop : Sec", 280, 298);

        isShowMainScreen = true;
        isShowPHsetPoint = false;
        isShowCalibrate = false;
        isShowSettingFeed = false;
    }
}

void showPHsetPoint()
{
    if (isShowPHsetPoint)
    {
        Serial.println("showing PH Set Point screen");
        uint16_t x, y;
        if (tft.getTouch(&x, &y))
        {
            Serial.printf("Touched at (%d, %d)\n", x, y);
            // save button
            if ((x > 0 && x < 240) && (y > 0 && y < 60))
            {
                pHSetPoint = prePHSetPoint;
                pHRange = prePpHRange;
                currentPage = 1;
            }
            // cancel button
            else if ((x > 240 && x < 480) && (y > 0 && y < 60))
            {
                prePHSetPoint = pHSetPoint;
                prePpHRange = pHRange;
                currentPage = 1;
            }
            // touch add set point
            else if ((x > 212 && x < 347) && (y > 190 && y < 300))
            {
                prePHSetPoint = prePHSetPoint + 0.1;
            }
            // touch negative set point
            else if ((x > 347 && x < 480) && (y > 190 && y < 300))
            {
                prePHSetPoint = prePHSetPoint - 0.1;
            }
            // touch add range set point
            else if ((x > 212 && x < 347) && (y > 80 && y < 190))
            {
                prePpHRange = prePpHRange + 0.1;
            }
            // touch negative range set point
            else if ((x > 347 && x < 480) && (y > 80 && y < 190))
            {
                prePpHRange = prePpHRange - 0.1;
            }

            tft.fillRoundRect(0, 0, 210, 250, 15, tft.color24to16(0xD7C5F9));

            tft.setTextSize(2);
            tft.setTextColor(tft.color24to16(0x483E59));
            tft.setTextDatum(TC_DATUM);
            tft.drawString("PH Setpoint", 100, 30);
            tft.drawString("Range", 100, 130);
            tft.drawString(String((prePHSetPoint - prePpHRange)) + "-" + String((prePHSetPoint + prePpHRange)), 100, 210);

            tft.setTextSize(4);
            tft.setTextColor(tft.color24to16(0x000));
            tft.setTextDatum(TC_DATUM);
            tft.drawFloat(prePHSetPoint, 1, 100, 63);

            tft.setTextSize(3);
            tft.setTextColor(tft.color24to16(0x000));
            tft.setTextDatum(TC_DATUM);
            tft.drawString("+/- " + String(prePpHRange), 100, 163);
        }
    }
    else
    {

        // =============PH Setpoint=============================================//
        tft.fillScreen(tft.color24to16(0xD7C5F9));

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0x483E59));
        tft.setTextDatum(TC_DATUM);
        tft.drawString("PH Setpoint", 100, 30);
        tft.drawString("Range", 100, 130);
        tft.drawString(String((pHSetPoint - pHRange)) + "-" + String((pHSetPoint + pHRange)), 100, 210);

        tft.setTextSize(4);
        tft.setTextColor(tft.color24to16(0x000));
        tft.setTextDatum(TC_DATUM);
        tft.drawFloat(pHSetPoint, 1, 100, 63);

        tft.setTextSize(3);
        tft.setTextColor(tft.color24to16(0x000));
        tft.setTextDatum(TC_DATUM);
        tft.drawString("+/- " + String(pHRange), 100, 163);

        tft.fillRoundRect(60, 265, 110, 28, 15, tft.color24to16(0x73FDFF));

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0x483E59));
        tft.setTextDatum(TC_DATUM);
        tft.drawString("Save", 118, 271);

        upSprite.createSprite(96, 96);
        upSprite.setSwapBytes(true);
        upSprite.pushImage(0, 0, 96, 96, up);

        upSprite.pushSprite(235, 28, TFT_BLACK);
        upSprite.pushSprite(235, 140, TFT_BLACK);

        downSprite.createSprite(96, 96);
        downSprite.setSwapBytes(true);
        downSprite.pushImage(0, 0, 96, 96, down);

        downSprite.pushSprite(360, 28, TFT_BLACK);
        downSprite.pushSprite(360, 140, TFT_BLACK);

        tft.fillRoundRect(310, 265, 110, 28, 15, tft.color24to16(0xFF8C95));

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0x483E59));
        tft.setTextDatum(TC_DATUM);
        tft.drawString("Cancel", 370, 271);

        isShowMainScreen = false;
        isShowPHsetPoint = true;
        isShowCalibrate = false;
        isShowSettingFeed = false;
    }
}

void showCalibrate()
{

    if (isShowCalibrate)
    {
        Serial.println("Showing Calibrate Screen");
        uint16_t x, y;
        if (tft.getTouch(&x, &y))
        {
            Serial.printf("Touched at (%d, %d)\n", x, y);
            // << back button
            if ((x > 0 && x < 220) && (y > 0 && y < 100))
            {
                currentPage = 1;
            }
        }
    }
    else
    {
        tft.fillScreen(tft.color24to16(0xD7C5F9));

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0x483E59));
        tft.setTextDatum(TC_DATUM);
        tft.drawString("pH Now", 70, 70);

        tft.setTextSize(5);
        tft.setTextColor(tft.color24to16(0x000));
        tft.setTextDatum(TC_DATUM);
        tft.drawFloat(pHCurrent, 2, 110, 140);

        tft.fillRoundRect(50, 246, 120, 40, 15, tft.color24to16(0xFF8C95));

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0xFFFFFF));
        tft.setTextDatum(TC_DATUM);
        tft.drawString("<< BACK", 110, 260);

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0x483E59));
        tft.setTextDatum(TC_DATUM);
        tft.drawString("Choose Calibrate Value", 340, 25);

        tft.fillRoundRect(300, 70, 120, 40, 15, tft.color24to16(0xFFAD68));

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0xFFFFFF));
        tft.setTextDatum(TC_DATUM);
        tft.drawString("pH 4.01", 360, 80);

        tft.fillRoundRect(300, 160, 120, 40, 15, tft.color24to16(0x046334));

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0xFFFFFF));
        tft.setTextDatum(TC_DATUM);
        tft.drawString("pH 6.86", 360, 173);

        tft.fillRoundRect(300, 250, 120, 40, 15, tft.color24to16(0x419AF4));

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0xFFFFFF));
        tft.setTextDatum(TC_DATUM);
        tft.drawString("pH 9.18", 360, 264);

        isShowMainScreen = false;
        isShowPHsetPoint = false;
        isShowCalibrate = true;
        isShowSettingFeed = false;
    }
}

void showSettingFeed()
{
    if (isShowSettingFeed)
    {
        Serial.println("Showing Calibrate Screen");
        uint16_t x, y;
        if (tft.getTouch(&x, &y))
        {
            Serial.printf("Touched at (%d, %d)\n", x, y);
            // << save button
            if ((x > 0 && x < 220) && (y > 0 && y < 60))
            {
                feedTime = preFeedTime;
                stopTime = preStopTime;
                mixing = preMixing;
                currentPage = 1;
            }
            // cancel button
            else if ((x > 356 && x < 480) && (y > 0 && y < 60))
            {
                preFeedTime = feedTime;
                preStopTime = stopTime;
                preMixing = mixing;
                currentPage = 1;
            }
            // add feed time
            else if ((x > 232 && x < 356) && (y > 205 && y < 320))
            {
                preFeedTime = preFeedTime + 1;
            }
            // negative feed time
            else if ((x > 356 && x < 480) && (y > 205 && y < 320))
            {
                preFeedTime = preFeedTime - 1;
            }
            // add stop time
            else if ((x > 232 && x < 356) && (y > 90 && y < 205))
            {
                preStopTime = preStopTime + 1;
            }
            // negative stop time
            else if ((x > 356 && x < 480) && (y > 90 && y < 205))
            {
                preStopTime = preStopTime - 1;
            }
            // switch mixing
            else if ((x > 235 && x < 480) && (y > 60 && y < 110))
            {

                preMixing = !preMixing;
            }

            tft.fillRoundRect(0, 0, 235, 200, 0, tft.color24to16(0xD7C5F9));
            tft.fillRoundRect(235, 213, 190, 48, 0, tft.color24to16(0xD7C5F9));

            tft.setTextSize(2);
            tft.setTextColor(tft.color24to16(0x483E59));
            tft.setTextDatum(TC_DATUM);
            tft.drawString("Feed", 60, 35);
            tft.drawString("Time", 60, 55);

            tft.setTextSize(5);
            tft.setTextColor(tft.color24to16(0x000));
            tft.setTextDatum(TR_DATUM);
            tft.drawFloat(preFeedTime, 1, 210, 45);

            tft.setTextSize(2);
            tft.setTextColor(tft.color24to16(0x483E59));
            tft.setTextDatum(TC_DATUM);
            tft.drawString("Stop", 60, 140);
            tft.drawString("Time", 60, 160);

            tft.setTextSize(5);
            tft.setTextColor(tft.color24to16(0x000));
            tft.setTextDatum(TR_DATUM);
            tft.drawFloat(preStopTime, 1, 210, 145);

            tft.drawFastHLine(270, 242, 160, tft.color24to16(0x000));
            tft.drawFastHLine(270, 243, 160, tft.color24to16(0x000));
            tft.drawFastHLine(270, 244, 160, tft.color24to16(0x000));

            if (preMixing)
            {
                tft.drawEllipse(420, 243, 10, 10, tft.color24to16(0x000));
                tft.drawEllipse(420, 243, 11, 11, tft.color24to16(0x000));
                tft.fillCircle(420, 243, 10, tft.color24to16(0x3D9CF8));
            }
            else
            {
                tft.drawEllipse(290, 243, 10, 10, tft.color24to16(0x000));
                tft.drawEllipse(290, 243, 11, 11, tft.color24to16(0x000));
                tft.fillCircle(290, 243, 10, tft.color24to16(0xafafaf));
            }
        }
    }
    else
    {
        tft.fillScreen(tft.color24to16(0xD7C5F9));

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0x483E59));
        tft.setTextDatum(TC_DATUM);
        tft.drawString("Feed", 60, 35);
        tft.drawString("Time", 60, 55);

        tft.setTextSize(5);
        tft.setTextColor(tft.color24to16(0x000));
        tft.setTextDatum(TR_DATUM);
        tft.drawFloat(feedTime, 1, 210, 45);

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0x483E59));
        tft.setTextDatum(TC_DATUM);
        tft.drawString("Stop", 60, 140);
        tft.drawString("Time", 60, 160);

        tft.setTextSize(5);
        tft.setTextColor(tft.color24to16(0x000));
        tft.setTextDatum(TR_DATUM);
        tft.drawFloat(stopTime, 1, 210, 145);

        upSprite.createSprite(96, 96);
        upSprite.setSwapBytes(true);
        upSprite.pushImage(0, 0, 96, 96, up);

        upSprite.pushSprite(235, 15, TFT_BLACK);
        upSprite.pushSprite(235, 115, TFT_BLACK);

        downSprite.createSprite(96, 96);
        downSprite.setSwapBytes(true);
        downSprite.pushImage(0, 0, 96, 96, down);

        downSprite.pushSprite(360, 15, TFT_BLACK);
        downSprite.pushSprite(360, 115, TFT_BLACK);

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0x483E59));
        tft.setTextDatum(TC_DATUM);
        tft.drawString("CIRCULATE ONLY MIXING", 130, 233);

        tft.drawFastHLine(270, 242, 160, tft.color24to16(0x000));
        tft.drawFastHLine(270, 243, 160, tft.color24to16(0x000));
        tft.drawFastHLine(270, 244, 160, tft.color24to16(0x000));

        tft.drawEllipse(290, 243, 10, 10, tft.color24to16(0x000));
        tft.drawEllipse(290, 243, 11, 11, tft.color24to16(0x000));
        tft.fillCircle(290, 243, 10, tft.color24to16(0xafafaf));

        tft.fillRoundRect(60, 265, 110, 28, 15, tft.color24to16(0x73FDFF));

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0x483E59));
        tft.setTextDatum(TC_DATUM);
        tft.drawString("Save", 118, 271);

        tft.fillRoundRect(310, 265, 110, 28, 15, tft.color24to16(0xFF8C95));

        tft.setTextSize(2);
        tft.setTextColor(tft.color24to16(0x483E59));
        tft.setTextDatum(TC_DATUM);
        tft.drawString("Cancel", 370, 271);

        isShowMainScreen = false;
        isShowPHsetPoint = false;
        isShowCalibrate = false;
        isShowSettingFeed = true;
    }
}