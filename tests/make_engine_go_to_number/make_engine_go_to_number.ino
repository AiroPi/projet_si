#include <multi_channel_relay.h>

#define CaptPosRef 3
// #define BPGo
#define Relais_BM 4
#define Capteur_Fin_Tour_BM 2

Multi_Channel_Relay pontH;

String input;
int position;

int refPositions[8] = {1, 5, 10, 25, 50, 100, 500, 1000};


void setup() {
    pinMode(CaptPosRef, INPUT);
    pinMode(Relais_BM, OUTPUT);
    pinMode(Capteur_Fin_Tour_BM, INPUT);
    pontH.begin(0x11);

    // Retour en position initiale
    pontH.channelCtrl(0b0000);
    digitalWrite(Relais_BM, LOW); // Front descendant pour alimenter le moteur
    while (digitalRead(Capteur_Fin_Tour_BM) == LOW); // Attente capteur fin de tour
    digitalWrite(Relais_BM, HIGH); // Front montant pour couper le moteur

    // TODO: Determiner la position initale
    position = 0;

    Serial.begin(115200);
    Serial.println("Type something!");
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
            while (digitalRead(CaptPosRef) == HIGH);
            while (digitalRead(CaptPosRef) == LOW);
        }
        pontH.channelCtrl(0b0000);

        digitalWrite(Relais_BM,LOW);                        // Front descendant pour alimenter le moteur
        delay(100);                                         // Attente destinée à laisser la came quitter le capteur
        while(digitalRead(Capteur_Fin_Tour_BM) == LOW);     // Attente activation capteur fin de tour
        digitalWrite(Relais_BM,HIGH); 
    }
}
