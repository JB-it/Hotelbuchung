#include <iostream>
#include <string>
#include <vector>
#include <math.h>

using namespace std;

//Enum für den switch
//(Da strings nicht in einem switch statement funktionieren...)
enum Methode {
    Hilfe,
    Status,
    Buchen,
    Auschecken,
    Ende,
    InvalideOption
};

vector<string> splitString(string source, char delimiter);  //Methode, die einen String in eine Liste aus substrings teilt.
void printHelp();                                           //Gibt eine Lister aller Befehle und deren Parameter zurück.
void printStatus(bool* zimmer, vector<string> params);      //Gibt den Status aller Räume oder den Status eines Raums zurück
void bookRoom(bool* zimmer, vector<string> params);         //Bucht einen der Räume
void checkOut(bool* zimmer, vector<string> params);         //Gibt einen gebuchten Raum wieder frei
Methode resolve(string method);                             //löst einen String zu einer switch freundlichen Methode auf

int main()
{
    const int etagen =  6; //Die Etagen des Hotels
    const int anzahl = 10; //Die Anzahl der Zimmer pro Etage

    bool zimmer[etagen * anzahl]; //Ein eindimensionales array, weil einfacher :D
    bool* pZimmer = zimmer;

    //Zu beginn sind alle Zimmer leer
    for (int i = 0; i < etagen*anzahl; i++)
    {
        zimmer[i] = false;
    }

    //Gibt eine Startinformation aus
    cout << "Herzlich wilkommen zum Programm \"Hotelbelegung\"," << endl;
    cout << "einem Projekt von Jonas Bluemer, Niko Drobe und Yannick Angenendt." << endl << endl;
    cout << "Das Hotel besitzt 6 Etagen mit jeweils 10 Zimmern pro Etage." << endl;
    cout << "Alle Angaben zur Etage und Zimmernummer muessen in indexen (ab 0 startend) angegeben werden." << endl;
    cout << "Fuer Supportanfragen wenden Sie sich bitte an einen von uns." << endl << endl;
    cout << "Fuer eine Liste aller befehle geben sie bitte 'hilfe' ein." << endl;

    //Das Programm soll nicht von alleine stoppen
    bool run = true;
    while (run) {
        //Fordert den Benutzer auf, eine Eingabe zu tätigen
        cout << "Bitte geben Sie eine Aktion an: \n>>>";
        string action;
        getline(cin, action);

        //Speichert alle parameter ab
        vector<string> actions = splitString(action, ' ');
        string method = actions.at(0);

        //Führt die eingegebene Methode aus
        switch (resolve(method)) {
            case Hilfe: printHelp();
                break;
            case Status: printStatus(zimmer, actions);
                break;
            case Buchen: bookRoom(zimmer, actions);
                break;
            case Auschecken: checkOut(zimmer, actions);
                break;
            case Ende: run = false;
                break;
            default:
                cout << "Die Eingabe konnte leider nicht ausgewertet werden." << endl;
                cout << "Geben Sie 'hilfe' ein, um eine Lister aller befehle zu erhalten." << endl << endl;
        }
    }
    return 0;
}

//TODO: Kommentieren!
Methode resolve(string method) {
    if (method == "hilfe" || method == "h")         return Hilfe;
    if (method == "status")                         return Status;
    if (method == "buchen")                         return Buchen;
    if (method == "auschecken")                     return Auschecken;
    if (method == "ende")                           return Ende;
    return InvalideOption;
}

vector<string> splitString(string source, char delimiter) {
    source += delimiter;
    string token;
    vector<string> tokens;
    
    for (int i = 0; i < source.length(); i++)
    {
        if (source[i] == delimiter) {
            if (token != "") {
                tokens.push_back(token);
            }
            token = "";
        }
        else {
            token += source[i];
        }

    }

    return tokens;
}

void printHelp() {
    //Druckt alle Befehle in der Konsole aus
    cout << endl;
    cout << "'hilfe': Gibt eine liste aller Befehle aus" << endl;
    cout << "'status [-e -n]': Gibt den Status aller Zimmer aus." << endl;
    cout << "\t -e: Waehlt die Etage aus" << endl;
    cout << "\t -n: Waehlt die Raumnummer aus (Funktioniert nur in Kombination mit '-e')" << endl;
    cout << "'buchen [-e -n]': Bucht den naechsten freien Raum" << endl;
    cout << "\t -e: Waehlt die Etage aus" << endl;
    cout << "\t -n: Waehlt die Raumnummer aus" << endl;
    cout << "\t\t Beide parameter funktionieren nur gemeinsam" << endl;
    cout << "'auschecken -e -n': Macht den Raum mit der Nummer -n in Etage -e wieder frei." << endl;
    cout << "'ende': Beendet das Programm" << endl << endl;
}

void printStatus(bool* zimmer, vector<string> params) {
    //Gibt den Status eines Zimmers, einer Etage oder des gesamten Hotels aus
    int e = -1, n = -1;

    //Sucht nach den Parametern und legt sie gegebenenfalls fest
    //Hört auf zu funktionieren, falls der Wert keine Zahl ist
    //TODO: Fix this!
    for (int i = 1; i < params.size(); i++) {
        if (params.at(i) == "-e") {
            e = stoi(params.at(++i));
        }
        if (e > -1 && params.at(i) == "-n") {
            n = stoi(params.at(++i));
        }
    }

    //Falls beide Werte nicht gesetzt sind
    if (e == -1 && n == -1) {
        //Iteriert über alle Zeilen und gibt sie aus
        for (int et = 5; et >= 0; et--)
        {
            //Druckt eine Zeile
            for (int i = 0; i < 10; i++) {
                cout << "+---";
            }
            cout << "+" << endl;
            for (int rn = 0; rn < 10; rn++)
            {
                cout << "|" << (zimmer[et*10+rn] ? " X " : "   ");
            }
            cout << "|" << endl;
        }
        for (int i = 0; i < 10; i++) {
            cout << "+---";
        }
        cout << "+" << endl;
    }
    //Wenn nur die Etage gesetzt ist
    else if (n == -1 && e < 7) {
        //Gibt eine Zeile aus
        for (int i = 0; i < 10; i++) {
            cout << "+---";
        }
        cout << "+" << endl;

        for (int rn = 0; rn < 10; rn++)
        {
            cout << "|" << (zimmer[e * 10 + rn] ? " X " : "   ");
        }
        cout << "|" << endl;

        for (int i = 0; i < 10; i++) {
            cout << "+---";
        }
        cout << "+" << endl;
    }
    //Wenn Etage und Raumnummer gesetzt sind
    else if (n < 11 && e < 7) {
        //Gibt eine Zelle aus
        cout << "+---+" << endl;
        cout << "|" << (zimmer[e * 10 + n] ? " X " : "   ") << "|" << endl;
        cout << "+---+" << endl;
    }
}

void bookRoom(bool* zimmer, vector<string> params) {
    int e = -1, n = -1;

    //Setzt die Parameter
    for (int i = 1; i < params.size(); i++) {
        if (params.at(i) == "-e") {
            e = stoi(params.at(++i));
        }
        if (e > -1 && params.at(i) == "-n") {
            n = stoi(params.at(++i));
        }
    }

    //Bucht den Raum wenn der Ort des Zimmers festgelegt ist
    if (e > -1 && e < 7 && n > -1 && n < 11) {
        if (zimmer[e * 10 + n]) {
            cout << "Der Raum mit der Nummer " << n << " in Etage " << e << " ist schon belegt." << endl;
            cout << "Bitte wählen Sie einen anderen Raum für Ihre Buchung aus." << endl;
        }
        else {
            zimmer[e * 10 + n] = true;
            cout << "Der Raum mit der Nummer " << n << " in Etage " << e << " wurde gebucht." << endl;
        }
    }
    //Gibt einen bestimmten Fehler aus, je nachdem was falsch gelaufen ist
    else {
        if (e == -1 || n == -1) {
            if (e == -1 && n == -1) {
                bool voll = true;
                for (int i = 0; i < 60; i++) {
                    if (!zimmer[i]) {
                        zimmer[i] = true;
                        voll = false;
                        cout << "Der Raum mit der Nummer " << i % 10 << " in Etage " << floor(i / 10) << " wurde gebucht." << endl;
                        break;
                    }
                }
                if (voll) {
                    cout << "Das Hotel ist im moment ausgebucht. Bitte versuchen Sie es zu einem spaeteren Zeitpunkt erneut." << endl;
                }
            }
            else {
                cout << "Bitte geben Sie sowohl Werte fuer die Etage (-e) als auch fuer die Raumnummer (-n) an" << endl;
            }
        }
        else if (e > 7 || n > 11) {
            cout << "Bitte geben Sie Werte im richtigen Bereich an." << endl;
        }
    }
}

void checkOut(bool* zimmer, vector<string> params) {
    int e = -1, n = -1;

    //Setzt die Parameter
    for (int i = 1; i < params.size(); i++) {
        if (params.at(i) == "-e") {
            e = stoi(params.at(++i));
        }
        if (e > -1 && params.at(i) == "-n") {
            n = stoi(params.at(++i));
        }
    }
    //Setzt den Raum auf frei, sofern alle Parameter gesetzt sind
    if (e > -1 && e < 7 && n > -1 && n < 11) {
        if (!zimmer[e * 10 + n]) {
            cout << "Der Raum mit der Nummer " << n << " in Etage " << e << " ist schon frei." << endl;
            cout << "Bitte wählen Sie einen anderen Raum zum Auschecken aus." << endl;
        }
        else {
            zimmer[e * 10 + n] = false;
            cout << "Der Raum mit der Nummer " << n << " in Etage " << e << " ist nun frei." << endl;
        }
    }
    else {
        cout << "Bitte geben Sie eine valide Etage und eine valide Zimmernummer ein." << endl;
    }
}