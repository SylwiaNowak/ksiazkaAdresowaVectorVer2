#include <iostream>
#include <vector>
#include <windows.h> // funkcja system("cls")
#include <cstdlib> // funkcja exit(0)
#include <fstream> //biblioteka odpowiedzialna za wspolprace z plikami
#include <sstream>
#include <algorithm>
#include <string>

using namespace std;

string nazwaPlikuTxt = "ksiazka_adresowa.txt";

struct Adresat {
    int idAdresata = 0;
    string imie = "", nazwisko = "", nrTelefonu = "", email = "", adres = "";
};

char wczytajZnak();
//string wczytajLinie();
//string zamienPierwszaLitereNaDuzaAPozostaleNaMale(string wyraz);
//string konwersjaZIntNaString(int zmienna);
//Adresat pobierzDaneAdresata(string daneAdresataZapisaneWLinii);
void wczytajAdresatowZPliku(vector<Adresat> &adresaci);
void wprowadzDaneNowegoAdresata(vector<Adresat> &adresaci);
void wyszukajAdresataWedlugImie(vector<Adresat> &adresaci);
void wyszukajAdresataWedlugNazwisko(vector<Adresat> &adresaci);
void wyswietlKsiazkeAdresowa(vector<Adresat> &adresaci);
//void zapiszAdresatowDoPlikuTekstowego(vector<Adresat> &adresaci);
void usunAdresata(vector<Adresat> &adresaci);
void edytujAdresata(vector<Adresat> &adresaci);
void zakonczProgram();

int main() {
    vector<Adresat> adresaci;
    char wybor;

    wczytajAdresatowZPliku(adresaci);

    while(true) {
        system("cls");
        cout << "KSIAZKA ADRESOWA" << endl << endl;
        cout << "1. Dodaj adresata" << endl;
        cout << "2. Wyszukaj po imieniu" << endl;
        cout << "3. Wyszukaj po nazwisku" << endl;
        cout << "4. Wyswietl wszystkich adresatow" << endl;
        cout << "5. Usun adresata" << endl;
        cout << "6. Edytuj adresata" << endl;
        cout << "9. Zakoncz program" << endl;
        cout << endl << "Twoj wybor: ";
        wybor = wczytajZnak();

        if (wybor == '1') {
            wprowadzDaneNowegoAdresata(adresaci);
        } else if (wybor == '2') {
            wyszukajAdresataWedlugImie(adresaci);
        } else if (wybor == '3') {
            wyszukajAdresataWedlugNazwisko(adresaci);
        } else if (wybor == '4') {
            wyswietlKsiazkeAdresowa(adresaci);
        } else if (wybor == '5') {
            usunAdresata(adresaci);
        } else if (wybor == '6') {
            edytujAdresata(adresaci);
        } else if (wybor == '9') {
            zakonczProgram();
        }
    }
    return 0;
}

char wczytajZnak() {
    string ciagZnakow;
    char poprawnyZnak;

    while (true) {
        getline(cin, ciagZnakow);
        if (ciagZnakow.length() == 1) {
            poprawnyZnak = ciagZnakow[0];
            break;
        } else {
            cout << "BLAD WCZYTYWANIA" << endl << "Spronuj ponownie" << endl;
        }
    }
    return poprawnyZnak;
}

string wczytajLinie() {
    string wejscie = "";
    getline (cin, wejscie);
    return wejscie;
}

string zamienPierwszaLitereNaDuzaAPozostaleNaMale(string wyraz) {
    if (wyraz.empty() == false) {
        transform (wyraz.begin(), wyraz.end(), wyraz.begin(), ::tolower);
        wyraz[0] = toupper(wyraz[0]);
    }
    return wyraz;
}

string konwersjaZIntNaString(int zmienna) {
    ostringstream ss;
    ss << zmienna;
    string str = ss.str();

    return str;
}

Adresat pobierzDaneAdresata(string daneAdresataZapisaneWLinii) {
    Adresat osoba;
    string pojedynczaDanaAdresata = "";
    int numerPojedynczejDanejAdresata = 1;

    for (int i = 0; i < daneAdresataZapisaneWLinii.length(); i++) {
        if (daneAdresataZapisaneWLinii[i] != '|') {
            pojedynczaDanaAdresata += daneAdresataZapisaneWLinii[i];
        } else {
            switch (numerPojedynczejDanejAdresata) {
            case 1:
                osoba.idAdresata = atoi(pojedynczaDanaAdresata.c_str());
                break;
            case 2:
                osoba.imie = pojedynczaDanaAdresata;
                break;
            case 3:
                osoba.nazwisko = pojedynczaDanaAdresata;
                break;
            case 4:
                osoba.nrTelefonu = pojedynczaDanaAdresata;
                break;
            case 5:
                osoba.email = pojedynczaDanaAdresata;
                break;
            case 6:
                osoba.adres = pojedynczaDanaAdresata;
                break;
            }
            pojedynczaDanaAdresata = "";
            numerPojedynczejDanejAdresata++;
        }
    }
    return osoba;
}

void wczytajAdresatowZPliku(vector<Adresat> &adresaci) {
    Adresat osoba;
    string daneAdresataZapisaneWLinii = ""; //zmienna, do ktorej zapisane beda odczytane z pliku linie
    //int nr_linii = 1; //licznik linii

    //tworzenie zmiennej plikowej, nazwa pliku w kodzie zrodlowym to 'ksiazka_adresowa'
    fstream plikTekstowy;
    plikTekstowy.open(nazwaPlikuTxt.c_str(), ios::in); //odczyt z pliku

    if (plikTekstowy.good() == false) {
        cout << "Ksiazka adresowa jest pusta." << endl;
        cout << "Nacisnij dowolny klawisz, aby przejsc do menu glownego." << endl;
        system("pause");
    }

    //kazdy znak bialy w tym spacja jest uznawany w strumieniu za separator, czyli cos co rozdziela dwa napisy
    //jednak czasem zmienna zawiera spacje, np. podwojne imie

    while(getline(plikTekstowy,daneAdresataZapisaneWLinii)) {
        //getline pozwala aby spacja mogla byc elementem stringu
        //czyli pobiera cala linie tekstu, pobiera ja z pliku tekstowego 'plikTekstowy' i zapisuje do zmiennej 'daneAdresataZapisaneWLinii'
        //petla while zwraca wartosc 'true' do momentu, gdy funkcja 'getline' zwraca linie i zakonczy sie
        //czyli zwroci wartosc 'false' kiedy funkcja getline przestanie zwracac linie

        osoba = pobierzDaneAdresata(daneAdresataZapisaneWLinii);

        adresaci.push_back(osoba);
    } //koniec petli 'while'

    plikTekstowy.close();
    plikTekstowy.clear();
}

void wprowadzDaneNowegoAdresata(vector<Adresat> &adresaci) {
    Adresat osoba;
    string imie, nazwisko, nrTelefonu, email, adres;
    //DODANIE OSOBY DO PLIKU
    system("cls");

    if (adresaci.empty() == true) {
        osoba.idAdresata = 1;
    } else {
        osoba.idAdresata = adresaci.back().idAdresata + 1;
    }

    cout << "Podaj imie nowego adresata: ";
    cin.sync();
    getline(cin, imie);
    cout << "Podaj nazwisko nowego adresata: ";
    cin >> nazwisko;
    cout << "Podaj numer telefonu nowego adresata: ";
    cin.sync();
    getline(cin, nrTelefonu);
    cout << "Podaj adres email nowego adresata: ";
    cin >> email;
    cout << "Podaj adres do korespondencji nowego adresata: ";
    cin.sync(); /*oproznia bufor*/
    getline(cin, adres); //wczytuje razem ze spacja

    //WCZYTANIE DANYCH ADRESATA DO STRUKTURY:
    osoba.imie = imie;
    osoba.imie = zamienPierwszaLitereNaDuzaAPozostaleNaMale(osoba.imie);
    osoba.nazwisko = nazwisko;
    osoba.nazwisko = zamienPierwszaLitereNaDuzaAPozostaleNaMale(osoba.nazwisko);
    osoba.nrTelefonu = nrTelefonu;
    osoba.email = email;
    osoba.adres = adres;

    //WCZYTANIE DANYCH ADRESATA DO VECTORA:
    adresaci.push_back(osoba);

    //DODANIE OSOBY DO PLIKU TEKSTOWEGO
    fstream plikTekstowy;
    plikTekstowy.open(nazwaPlikuTxt.c_str(), ios::out | ios::app); //input output stream out - wyprowadz
    //czyli pokieruj strumien danych z programu do pliku
    // metoda c_str() konwertuje ci¹g znaków zapisany w zmiennej typu string na ci¹g który mo¿e byæ zapisany w tablicy znaków

    if (plikTekstowy.good() == true) {
        //wyrzucenie danej strumieniem do pliku na dysk:
        plikTekstowy << osoba.idAdresata << '|';
        plikTekstowy << osoba.imie << '|';
        plikTekstowy << osoba.nazwisko << '|';
        plikTekstowy << osoba.nrTelefonu << '|';
        plikTekstowy << osoba.email << '|';
        plikTekstowy << osoba.adres << '|';

        plikTekstowy.close();
        plikTekstowy.clear();

        cout << "Adresat dodany";
        Sleep(2000);
    } else {
        cout << "Nie mozna otworzyc pliku \"ksiazka_adresowa.txt\"" << endl;
    }
}

void wyszukajAdresataWedlugImie(vector<Adresat> &adresaci) {
    string szukaneImie;
    int iloscZnalezionychWynikow = 0;
    system("cls");
    cout << "Podaj imie, ktore wyszukac: ";
    cin.sync();
    getline (cin, szukaneImie);
    cout << endl;

    for (vector<Adresat> ::iterator itr = adresaci.begin(); itr != adresaci.end(); itr++) {
        if (itr -> imie == szukaneImie) {
            cout << itr -> idAdresata << endl;
            cout << itr -> imie << endl;
            cout << itr -> nazwisko << endl;
            cout << itr -> nrTelefonu << endl;
            cout << itr -> email << endl;
            cout << itr -> adres << endl;

            iloscZnalezionychWynikow++;
        } // koniec instrukcji 'if'

    } //koniec petli 'for'

    if (iloscZnalezionychWynikow == 0) {
        cout << endl << "Nie znaleziono adresatow o podanym imieniu" << endl;
    } else {
        cout << endl << "Ilosc znalezionych adresatow o podanym imieniu wynosi: " << iloscZnalezionychWynikow << endl;
    }

    system("pause");
}

void wyszukajAdresataWedlugNazwisko(vector<Adresat> &adresaci) {
    string szukaneNazwisko;
    int iloscZnalezionychWynikow = 0;
    system("cls");
    cout << "Podaj nazwisko, ktore wyszukac: ";
    cin.sync();
    getline (cin, szukaneNazwisko);
    cout << endl;

    for (vector<Adresat> ::iterator itr = adresaci.begin(); itr != adresaci.end(); itr++) {
        if (itr -> nazwisko == szukaneNazwisko) {
            cout << itr -> idAdresata << endl;
            cout << itr -> imie << endl;
            cout << itr -> nazwisko << endl;
            cout << itr -> nrTelefonu << endl;
            cout << itr -> email << endl;
            cout << itr -> adres << endl;

            iloscZnalezionychWynikow++;
        }
    }

    if (iloscZnalezionychWynikow == 0) {
        cout << endl << "Nie znaleziono adresatow o podanym nazwisku" << endl;
    } else {
        cout << endl << "Ilosc znalezionych adresatow o podanym nazwisku wynosi: " << iloscZnalezionychWynikow << endl;
    }

    system("pause");
}

void wyswietlKsiazkeAdresowa(vector<Adresat> &adresaci) {
    system("cls");
    if (!adresaci.empty()) {
        for (vector<Adresat> ::iterator itr = adresaci.begin(); itr != adresaci.end(); itr++) {
            cout << "ID: " << itr -> idAdresata << endl;
            cout << "Imie: " << itr -> imie << endl;
            cout << "Nazwisko: " << itr -> nazwisko << endl;
            cout << "Numer telefonu: " << itr -> nrTelefonu << endl;
            cout << "E-mail: " << itr -> email << endl;
            cout << "Adres: " << itr -> adres << endl << endl;
        }
    } else {
        cout << "Ksiazka adresowa jest pusta" << endl;
    }

    system ("pause");
}

void zapiszAdresatowDoPlikuTekstowego(vector<Adresat> &adresaci) {
    fstream plikTekstowy;
    string pojedynczaLiniaZDanymiAdresata = "";

    plikTekstowy.open(nazwaPlikuTxt.c_str(), ios::out );
    if (plikTekstowy.good()) {
        for(vector<Adresat> ::iterator itr = adresaci.begin(); itr != adresaci.end(); itr++) {
            pojedynczaLiniaZDanymiAdresata += konwersjaZIntNaString(itr -> idAdresata) + '|';
            pojedynczaLiniaZDanymiAdresata += itr -> imie + '|';
            pojedynczaLiniaZDanymiAdresata += itr -> nazwisko + '|';
            pojedynczaLiniaZDanymiAdresata += itr -> nrTelefonu + '|';
            pojedynczaLiniaZDanymiAdresata += itr -> email + '|';
            pojedynczaLiniaZDanymiAdresata += itr -> adres + '|';

            plikTekstowy << pojedynczaLiniaZDanymiAdresata << endl;
            pojedynczaLiniaZDanymiAdresata = "";
        }
        plikTekstowy.close();
    } else {
        cout << "Nie mozna otworzyc pliku tekstowego" << endl;
    }

}

void usunAdresata(vector<Adresat> &adresaci) {
    int idUsuwanegoAdresata;
    char znak;
    bool czyIstniejeAdresat = false;

    system("cls");
    if (!adresaci.empty()) {
        cout << "Wybierz ID adresata do usuniecia: ";
        cin >> idUsuwanegoAdresata;

        for (vector<Adresat> ::iterator itr = adresaci.begin(); itr != adresaci.end(); itr++) {
            if (itr -> idAdresata == idUsuwanegoAdresata) {
                czyIstniejeAdresat = true;
                cout << "Potwierdz naciskajac klawisz 't': ";
                cin >> znak;
                if (znak == 't') {
                    itr = adresaci.erase(itr);
                    cout << "Wskazany adresat zostal usuniety" << endl;
                    zapiszAdresatowDoPlikuTekstowego(adresaci);
                    break;
                } else {
                    cout << "Wybrany adresat nie zostal usuniety" << endl;
                }
            }

        }
        if (czyIstniejeAdresat == false) {
            cout << "Szukany adresat nie istnieje" << endl;
        }
    } else {
        cout << "Ksiazka adresowa jest pusta" << endl;
    }
    system("pause");
}

void edytujAdresata(vector<Adresat> &adresaci) {
    int idAdresataDoEdycji = 0;
    bool czyIstniejeAdresat = false;
    char znak;

    system("cls");
    if (!adresaci.empty()) {
        cout << "W celu edycji, podaj ID wybranego adresata: ";
        cin >> idAdresataDoEdycji;

        for (vector<Adresat> ::iterator itr = adresaci.begin(); itr != adresaci.end(); itr++) {
            if (itr -> idAdresata == idAdresataDoEdycji) {
                czyIstniejeAdresat = true;

                cout << "Wskaz jaka dana edytowa: " << endl;
                cout << "1. - imie" << endl;
                cout << "2. - nazwisko" << endl;
                cout << "3. - numer telefonu" << endl;
                cout << "4. - e-mail" << endl;
                cout << "5. - adres" << endl;
                cout << "6. - powrot do menu" << endl;
                znak = wczytajZnak();

                switch (znak) {
                case '1':
                    cout << "Podaj nowe imie: " << endl;
                    itr -> imie = wczytajLinie();
                    itr -> imie = zamienPierwszaLitereNaDuzaAPozostaleNaMale(itr -> imie);
                    cout << "Imie zostalo edytowane" << endl;
                    zapiszAdresatowDoPlikuTekstowego(adresaci);
                    break;
                case '2':
                    cout << "Podaj nowe nazwisko: " << endl;
                    itr -> nazwisko = wczytajLinie();
                    itr -> nazwisko = zamienPierwszaLitereNaDuzaAPozostaleNaMale(itr -> nazwisko);
                    cout << "Nazwisko zostalo edytowane" << endl;
                    zapiszAdresatowDoPlikuTekstowego(adresaci);
                    break;
                case '3':
                    cout << "Podaj nowy numer telefonu: " << endl;
                    itr -> nrTelefonu = wczytajLinie();
                    cout << "Numer telefonu zostal edytowany" << endl;
                    zapiszAdresatowDoPlikuTekstowego(adresaci);
                    break;
                case '4':
                    cout << "Podaj nowy emial: " << endl;
                    itr -> email = wczytajLinie();
                    cout << "E-mail zostal edytowany" << endl;
                    zapiszAdresatowDoPlikuTekstowego(adresaci);
                    break;
                case '5':
                    cout << "Podaj nowy adres: " << endl;
                    itr -> adres = wczytajLinie();
                    cout << "Adres zostal edytowany" << endl;
                    zapiszAdresatowDoPlikuTekstowego(adresaci);
                    break;
                case '6':
                    cout << "Powrot do menu" << endl;
                    break;
                default:
                    cout << "Niewlasciwa cyfra wyboru, sprobuj ponownie: " << endl;
                    break;
                }
            }
        }

        if (czyIstniejeAdresat == false) {
            cout << "Nie ma takiego adresata" << endl;
        }
    } else {
        cout << "Ksiazka adresowa jest pusta" << endl;
    }
    system("pause");
}

void zakonczProgram() {
    cout << "Koniec programu" << endl;
    exit(0);
}
