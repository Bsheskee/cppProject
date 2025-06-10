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
bool zapiszDoPliku(const vector<STUDENT>& studenci, const string& nazwaPliku);
string okreslOceneKoncowa(double srednia);


int main() {
    vector<STUDENT> studenci;
    
    // Wczytanie danych
    if (!wczytajDane("dane.txt", studenci)) {
        return 1;
    }
    
    // Obliczenie srednich i ocen
    for (STUDENT& student : studenci) {
        student.srednia = obliczSrednia(student.oceny, 6);
        student.ocenaKoncowa = okreslOceneKoncowa(student.srednia);
    }
    
    // Obliczenie sredniej wszystkich
    double sredniaWszystkich = obliczSredniaWszystkich(studenci);
    int powyzej = liczbaStudentowPowyzejSredniej(studenci, sredniaWszystkich);
    
    // Wyswietlenie wynikow
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
    cout << "--------------------------------------------------------------------" << endl;
    cout << "Srednia wszystkich studentow = " << sredniaWszystkich << endl;
    cout << "Ilosc studentow powyzej sredniej: " << powyzej << endl;
    cout << "--------------------------------------------------------------------" << endl;

    vector<STUDENT> powyzejSredniej;

    cout << "Studenci powyzej sredniej:" << endl;
    for (const auto& student : studenci) {
        if (student.srednia > sredniaWszystkich) {
            cout << setw(20) << left << student.imieNazwisko << " ";
            cout << setw(3) << fixed << setprecision(2) << student.srednia << endl;
            
            powyzejSredniej.push_back(student); // Dodaj do wektora do zapisu, na koniec
        }
    }
    cout << "--------------------------------------------------------------------" << endl;

    if (zapiszDoPliku(powyzejSredniej, "powyzej_sredniej.txt")) {
        cout << "Zapisano studentow powyżej sredniej" << endl;
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


bool zapiszDoPliku(const vector<STUDENT>& studenci, const string& nazwaPliku) {
    // 1. Stworz plik do zapisu (jesli nie istnieje) i otworz
    ofstream plik(nazwaPliku);
    if (!plik) {
        cout << "Nie można otworzyć pliku!" << endl;
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
        cout << "Błąd podczas zapisu!" << endl;
        return false;
    }
    
    return true;
}

string okreslOceneKoncowa(double srednia) {
    if (srednia >= 91) return "bardzo dobry";
    else if (srednia >= 81) return "dobry plus";
    else if (srednia >= 71) return "dobry";
    else if (srednia >= 61) return "dostateczny plus";
    else if (srednia >= 51) return "dostateczny";
    else return "niedostateczny";
}
