#include <multi_channel_relay.h>

#define CaptPosRef 3
#define BPGo

Multi_Channel_Relay pontH;

String input;
int position;

int refPositions[8] = {1, 5, 10, 25, 50, 100, 500, 1000};


void setup() {
    Serial.begin(115200);
    Serial.println("Type something!");

    // TODO: Determiner la position initale
    position = 0;

    pinMode(CaptPosRef, INPUT);
    pontH.begin(0x11);
    pontH.channelCtrl(0b0000);
}

void loop() {
    if (Serial.available())
    {
        input = Serial.readStringUntil('\n');
        Serial.print("You typed: ");
        Serial.println(input);

        int positionGoal;
        for (int i = 0; i < sizeof(refPositions); i++)
        {
            if (input.toInt() == refPositions[i])
            {
                positionGoal = i;
                break;
            }
        }


        int diff = positionGoal - position;
        int moveGoal;
        // TODO: inverser le sens plus tard
        if (diff == 0) {
            Serial.println("On bouge pas");
            moveGoal = 0;
        } else if (diff < -4) {
            pontH.channelCtrl(0b1001);
            Serial.print("On bouge dans le sens horraire de : ");
            moveGoal = 8 - abs(diff);
            Serial.println(8-abs(diff));
        } else if (diff < 0) {
            pontH.channelCtrl(0b0110);
            Serial.print("On bouge dans le sens anti-horraire de : ");
            moveGoal = abs(diff);
            Serial.println(abs(diff));
        } else if (diff < 4) {
            pontH.channelCtrl(0b1001);
            Serial.print("On bouge dans le sens horraire de : ");
            moveGoal = diff;
            Serial.println(diff);
        } else {
            pontH.channelCtrl(0b0110);
            Serial.print("On bouge dans le sens anti-horraire de : ");
            moveGoal = 8 - diff;
            Serial.println(8-diff);
        }
        position = positionGoal;

        for (int i = 0; i < moveGoal; i++) {
            while (digitalRead(CaptPosRef) == HIGH){}
            while (digitalRead(CaptPosRef) == LOW){}
        }
        pontH.channelCtrl(0b0000);
    }
}
