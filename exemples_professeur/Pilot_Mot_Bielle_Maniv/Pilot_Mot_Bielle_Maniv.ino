#define Relais_BM 4
#define Capteur_Fin_Tour_BM 2
#define BP 8

// Le relais passe a ON sur un front montant sur la PIN Relais_BM
// Le relais passe a OFF sur un front descendant sur la PIN Relais_BM

void setup()
{
    pinMode(Relais_BM,OUTPUT);
    pinMode(Capteur_Fin_Tour_BM,INPUT);
    // Retour en position initiale de la bielle manivelle
    digitalWrite(Relais_BM,LOW);                        // Front descendant pour alimenter le moteur
    while(digitalRead(Capteur_Fin_Tour_BM) == LOW);     // Attente capteur fin de tour
    digitalWrite(Relais_BM,HIGH);                       // Front montant pour couper le moteur
    
    pinMode(BP,INPUT);
}
void loop()
{
    while(digitalRead(BP) == LOW);                      // Attente pression sur BP
    
    digitalWrite(Relais_BM,LOW);                        // Front descendant pour alimenter le moteur
    delay(100);                                         // Attente destinée à laisser la came quitter le capteur
    while(digitalRead(Capteur_Fin_Tour_BM) == LOW);     // Attente activation capteur fin de tour
    digitalWrite(Relais_BM,HIGH);                       // Front montant pour couper le moteur
}

