//
//  main.cpp
//  ProjektGrupowy
//
//

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream> //- konwersja string na int (istringstream)
#include <iomanip> //- formatowanie wyjścia
using namespace std;

struct STUDENT {
    string imieNazwisko;
    int oceny[6];
    double srednia;
    string ocenaKoncowa;
};

bool wczytajDane(const string& nazwaPliku, vector<STUDENT>& studenci);
double obliczSrednia(const int oceny[], int rozmiar);
double obliczSredniaWszystkich(const vector<STUDENT>& studenci);
double wynikPowyzejSredniej(double sredniaStudenta, double sredniaStudentow);
int liczbaStudentowPowyzejSredniej(const vector<STUDENT>& studenci, double srednia);
vector<STUDENT> znajdzStudentowPowyzejSredniej(const vector<STUDENT>& studenci, double progSredniej);
bool zapiszSrednieDoPliku(const vector<STUDENT>& studenci, const string& nazwaPliku);
void wyszukajIWyswietlStudenta(const vector<STUDENT>& studenci);
int pobierzIntZWalidacja(const string& komunikat, int minVal, int maxVal);
void zmodyfikujOceneStudenta(vector<STUDENT>& studenci);
bool zapiszZaktualizowaneDaneDoPliku(const vector<STUDENT>& studenci, const string& nazwaPliku);
void dodajStudenta(vector<STUDENT>& studenci);
string okreslOceneKoncowa(double srednia);


int main() {
    vector<STUDENT> studenci;
    
    // Wczytanie danych
    if (!wczytajDane("dane.txt", studenci)) {
        return 1;
    }
    
    int wybor;
    bool programDziala = true;
    vector<STUDENT> powyzejSredniej;
    
    while (programDziala) {
        cout << "\n--- MENU GLOWNE ---" << endl;
        cout << "1. Wyswietl oceny, srednie i oceny koncowe studentow." << endl;
        cout << "2. Wyswietl studentow powyzej sredniej wszystkich studentow" << endl;
        cout << "3. Zapisz studentow powyzej sredniej do pliku" << endl;
        cout << "4. Wyszukaj studenta" << endl;
        cout << "5. Zmodyfikuj ocene studenta." << endl;
        cout << "6. Dodaj studenta." << endl;
        cout << "7. Zapisz zaktualizowane dane do nowego pliku" << endl;
        cout << "0. Wyjdz z programu." << endl;
        cout << "Wybierz opcje: ";
        
        wybor = pobierzIntZWalidacja("", 0, 7);
        switch (wybor) {
            case 1:
                // Obliczenie srednich i ocen
                for (auto& student : studenci) { // od C++11; automatyczne wykrycie typu
                    student.srednia = obliczSrednia(student.oceny, 6);
                    student.ocenaKoncowa = okreslOceneKoncowa(student.srednia);
                }
                
                // Wyswietlenie wynikow
                cout << "-------------------OPCJA 1-----------------------------" << endl;
                cout << "Dane studentow:" << endl;
                cout << "--------------------------------------------------------------------" << endl;
                cout << setw(20) << left << "Imie i nazwisko"
                    << setw(25) << "Oceny"
                    << setw(10) << "Srednia"
                    << "Ocena koncowa" << endl;
                cout << "--------------------------------------------------------------------" << endl;
                
                for (const auto& student : studenci) {
                    cout << setw(20) << left << student.imieNazwisko << " ";
                    // Wyswietlanie ocen
                    cout << "[";
                    for (int i = 0; i < 6; ++i) {
                        cout << student.oceny[i];
                        if (i < 5) cout << ", ";
                    }
                    cout << "] ";
                    
                    // Wyswietlanie sredniej i oceny koncowej
                    cout << setw(9) << fixed << setprecision(2) << student.srednia << " ";
                    cout << student.ocenaKoncowa;
                    cout << endl;
                }
                
                break;
            case 2: {
                // Obliczenie sredniej wszystkich
                double sredniaWszystkich = obliczSredniaWszystkich(studenci);
                int powyzej = liczbaStudentowPowyzejSredniej(studenci, sredniaWszystkich);
                
                cout << "-------------------OPCJA 2-----------------------------" << endl;
                cout << "Srednia wszystkich studentow = " << sredniaWszystkich << endl;
                cout << "Ilosc studentow powyzej sredniej: " << powyzej << endl;
                cout << "--------------------------------------------------------------------" << endl;
                
                // 1 petla do dwoch zastosowan: 1 wyswietlanie, 2 przechowanie tych powSr do zapisu (zamiast dwoch petli)
                cout << "Studenci powyzej sredniej:" << endl;
                for (const auto& student : studenci) {
                    if (student.srednia > sredniaWszystkich) {
                        cout << setw(20) << left << student.imieNazwisko << " ";
                        cout << setw(3) << fixed << setprecision(2) << student.srednia << endl;
                        
                        powyzejSredniej.push_back(student); // Dodaj do wektora do zapisu, na koniec
                    }
                }
                cout << "--------------------------------------------------------------------" << endl;
            }
                break;
            case 3:
                cout << "-------------------OPCJA 3-----------------------------" << endl;
                if (zapiszSrednieDoPliku(powyzejSredniej, "powyzej_sredniej.txt")) {
                    cout << "Zapisano studentow powyżej sredniej" << endl;
                }
                cout << endl;
                break;
            case 4:
                cout << "-------------------OPCJA 4-----------------------------" << endl;
                wyszukajIWyswietlStudenta(studenci);
                break;
            case 5:
                cout << "-------------------OPCJA 5-----------------------------" << endl;
                zmodyfikujOceneStudenta(studenci);
                break;
            case 6:
                cout << "-------------------OPCJA 6-----------------------------" << endl;
                dodajStudenta(studenci);
                break;
            case 7:
                if (zapiszZaktualizowaneDaneDoPliku(studenci, "dane_update.txt")) {
                    cout << "Zapisano pełne dane!\n";
                }
                break;
            case 0:
                programDziala = false;
                cout << "Zamykanie programu. Do widzenia!" << endl;
                break;
            default:
                cout << "Nieprawidlowy wybor. Sprobuj ponownie." << endl;
                break;
        }
    }
    return 0;
}
// boolean bo chcemy dalsze operacje przeprowadzac jedynie wtedy gdy udalo sie wczytac dane z pliku
bool wczytajDane(const string& nazwaPliku, vector<STUDENT>& studenci) {
    ifstream plik(nazwaPliku);
    if (!plik.is_open()) {
        cerr << "Nie mozna otworzyc pliku: " << nazwaPliku << endl;
        return false;
    }
    string linia;
    while (getline(plik, linia)) { //petla kazdego studenta
        STUDENT student;
        student.imieNazwisko = linia;
        
        if (getline(plik, linia)) {
            istringstream iss(linia);
            for (int i= 0; i < 6; i++) { // petla kazdej oceny obecnie iterowanego studenta
                if (!(iss >> student.oceny[i])) {
                    cerr << "Blad formatu ocen dla: " << student.imieNazwisko << endl;
                    return false;
                }
            }
            studenci.push_back(student);
        }
    }
    plik.close();
    return true; //udalo sie wczytac wszystkie dane bez bledow
}

double obliczSrednia(const int oceny[], int rozmiar) {
    double suma = 0;
    for (int i = 0; i < rozmiar; i++) {
        suma += oceny[i];
    }
    return suma / rozmiar;
}
double obliczSredniaWszystkich(const vector<STUDENT>& studenci) {
    if (studenci.empty()) return 0;
    
    double suma = 0;
    for (const auto& student : studenci) {
        suma += student.srednia;
    }
    return suma / studenci.size();
}

double wynikPowyzejSredniej(double sredniaStudenta, double sredniaStudentow) {
    double wynikPowyzejSredniej = 0.0;
    if (sredniaStudenta > sredniaStudentow) {
        wynikPowyzejSredniej = sredniaStudenta;
    }
    return wynikPowyzejSredniej;
}

int liczbaStudentowPowyzejSredniej(const vector<STUDENT>& studenci, double srednia) {
    return count_if(studenci.begin(), studenci.end(),
        [srednia](const STUDENT& s) { return s.srednia > srednia; });
}

vector<STUDENT> znajdzStudentowPowyzejSredniej(const vector<STUDENT>& studenci, double progSredniej) {
    vector<STUDENT> wynik;
    
    for (const auto& student : studenci) {
        if (student.srednia > progSredniej) {
            wynik.push_back(student);
        }
    }
    return wynik;
}


bool zapiszSrednieDoPliku(const vector<STUDENT>& studenci, const string& nazwaPliku) {
    // 1. Stworz plik do zapisu (jesli nie istnieje) i otworz
    ofstream plik(nazwaPliku);
    if (!plik) {
        cout << "Nie mozna otworzyć pliku!" << endl;
        return false;
    }
    
    // 2. Zapisz kazdego studenta
    for (const auto& student : studenci) {
        plik << student.imieNazwisko << " "
             << student.srednia << " "
             << student.ocenaKoncowa << endl;
    }
    
    // 3. Sprawdz czy zapis sie udal
    if (!plik) {
        cout << "Blad podczas zapisu!" << endl;
        return false;
    }
    
    return true;
}

void wyszukajIWyswietlStudenta(const vector<STUDENT>& studenci) {
    while (true) {
        string szukaneImieNazwisko;
        
        cout << "\nABY WYSZUKAC STUDENTA, PODAJ IMIE I NAZWISKO (lub wpisz 'q' aby wyjsc): ";
        getline(cin, szukaneImieNazwisko);
        
        // Mozliwosc wyjscia z funkcji
        if (szukaneImieNazwisko == "q" || szukaneImieNazwisko == "Q") {
            break;
        }
        
        bool znaleziono = false;
        for (const auto& student : studenci) {
            if (student.imieNazwisko == szukaneImieNazwisko) {
                cout << "\n=== ZNALEZIONO STUDENTA ===" << endl;
                cout << "Imie i nazwisko: " << student.imieNazwisko << endl;
                cout << "Srednia: " << fixed << setprecision(2) << student.srednia << endl;
                
                cout << "Oceny: ";
                for (int i = 0; i < 6; i++) {
                    cout << student.oceny[i] << " ";
                }
                cout << endl;
                
                znaleziono = true;
                break;
            }
        }
        cout << endl;
        
        if (!znaleziono) {
            cout << "\nNie znaleziono studenta: " << szukaneImieNazwisko << endl;
            cout << "Dostepni studenci:\n";
            for (const auto& student : studenci) {
                cout << "- " << student.imieNazwisko << endl;
            }
            cout << "\nSprobuj ponownie." << endl;
        }
        else {
            // Jeśli znaleziono, pytamy czy kontynuować
            cout << "\nCzy chcesz wyszukac kolejnego studenta? (t/n): ";
            string decyzja;
            getline(cin, decyzja);
            
            if (decyzja != "t" && decyzja != "T") {
                break;
            }
        }
    }
}
int pobierzIntZWalidacja(const string& komunikat, int minVal, int maxVal) {
    int wartosc;
    while (true) {
        cout << komunikat;
        cin >> wartosc;

        if (cin.fail() || wartosc < minVal || wartosc > maxVal) {
            cout << "Nieprawidlowa wartosc. Podaj liczbe z zakresu "
                 << minVal << " do " << maxVal << ": " << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return wartosc;
        }
    }
}

void zmodyfikujOceneStudenta(vector<STUDENT>& studenci) {
    string szukaneImieNazwisko;
    bool studentZnaleziony = false;
    
    cout << "Podaj imie i nazwisko studenta do zmiany oceny: ";
    getline(cin, szukaneImieNazwisko);
    for (auto& s : studenci) {
        if (s.imieNazwisko == szukaneImieNazwisko) {
            studentZnaleziony = true;
            
            cout << "Bieżące oceny: ";
            for (size_t i = 0; i < 6; ++i) {
                cout << (i + 1) << ": " << s.oceny[i] << " ";
            }
            cout << endl;
            
            int indeksDoZmiany = pobierzIntZWalidacja(
                                                      "Ktora ocene zmienic (podaj numer, np. 1 dla pierwszej): ",
                                                      1, 6
                                                      );
            int nowaOcena = pobierzIntZWalidacja(
                                                 "Podaj nowa ocene: ",
                                                 0, 100
                                                 );
            s.oceny[indeksDoZmiany - 1] = nowaOcena;
            cout << "Ocena zmieniona pomyslnie!" << endl;
            
            cout << "Zaktualizowane oceny: ";
            for (size_t i = 0; i < 6; ++i) {
                cout << (i + 1) << ": " << s.oceny[i] << " ";
            }
            cout << endl;
            return;
        }
        
    }
    cout << "Student o podanym imieniu i nazwisku nie znaleziony." << endl;
}

bool zapiszZaktualizowaneDaneDoPliku(const vector<STUDENT>& studenci, const string& nazwaPliku) {
    ofstream plik(nazwaPliku);
    if (!plik) {
        cerr << "Nie można otworzyć pliku " << nazwaPliku << endl;
        return false;
    }
    
    for (const auto& student : studenci) {
        plik << student.imieNazwisko << "\n";
        for (int i = 0; i < 6; ++i) {
            plik << student.oceny[i];
            if (i < 5) plik << " ";
        }
        plik << "\n";
    }
    
    if (!plik.good()) {
        cerr << "Błąd podczas zapisu pełnych danych!" << endl;
        return false;
    }
    
    return true;
}
void dodajStudenta(vector<STUDENT>& studenci) {
    STUDENT nowyStudent;
    
    cout << "--- Dodawanie nowego studenta ---" << endl;
    cout << "Podaj imie i nazwisko nowego studenta: ";
    getline(cin, nowyStudent.imieNazwisko);
    cout << "Podaj 6 ocen dla studenta (kazda po kolei):" << endl;
    for (int i = 0; i < 6; ++i) {
        nowyStudent.oceny[i] = pobierzIntZWalidacja(
                                                    "Ocena #" + to_string(i + 1) + ": ",
                                                    0, 100
                                                    );
    }
    studenci.push_back(nowyStudent);
    cout << "Student " << nowyStudent.imieNazwisko << " zostal dodany pomyslnie!" << endl;
}

string okreslOceneKoncowa(double srednia) {
    if (srednia >= 91) return "bardzo dobry";
    else if (srednia >= 81) return "dobry plus";
    else if (srednia >= 71) return "dobry";
    else if (srednia >= 61) return "dostateczny plus";
    else if (srednia >= 51) return "dostateczny";
    else return "niedostateczny";
}

