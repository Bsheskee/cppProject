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
string okreslOceneKoncowa(double srednia);


int main() {
    vector<STUDENT> studenci;
    
    // 1. Wczytanie danych
    if (!wczytajDane("dane.txt", studenci)) {
        return 1;
    }
    
    // 2. Obliczenie średnich i ocen
    for (auto& student : studenci) {
        student.srednia = obliczSrednia(student.oceny, 6);
        student.ocenaKoncowa = okreslOceneKoncowa(student.srednia);
    }
    
    // 3. Obliczenie średniej wszystkich
    double sredniaWszystkich = obliczSredniaWszystkich(studenci);
    int powyzej = liczbaStudentowPowyzejSredniej(studenci, sredniaWszystkich);

    //
    
    // Wyświetlenie wyników
    cout << "Dane studentow:" << endl;
    cout << "--------------------------------------------------------------------" << endl;
    cout << setw(20) << left << "Imie i nazwisko"
        << setw(25) << "Oceny"
        << setw(10) << "Srednia"
        << "Ocena koncowa" << endl;
    cout << "--------------------------------------------------------------------" << endl;
    
    for (const auto& student : studenci) {
        cout << setw(20) << left << student.imieNazwisko << " ";
        
        // Wyświetlanie ocen
        cout << "[";
        for (int i = 0; i < 6; ++i) {
            cout << student.oceny[i];
            if (i < 5) cout << ", ";
        }
        cout << "] ";
        
        // Wyświetlanie średniej i oceny końcowej
        cout << setw(9) << fixed << setprecision(2) << student.srednia << " ";
        cout << student.ocenaKoncowa;
        cout << endl;

    }
    cout << "--------------------------------------------------------------------" << endl;
    cout << "srednia studentow = " << sredniaWszystkich << endl;
    cout << "Ilosc studentow powyzej sredniej: " << powyzej << endl;
    cout << "Studenci powyzej sredniej: " << endl;
    cout << "--------------------------------------------------------------------" << endl;

    for (const auto& student : studenci) {
        if (wynikPowyzejSredniej(student.srednia, sredniaWszystkich) > sredniaWszystkich) {
            cout << setw(20) << left << student.imieNazwisko << " ";
            cout << setw(3) << fixed << setprecision(2) << student.srednia << " ";
            cout << endl;
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
    for (const auto& student : studenci) { // od C++11; automatyczne wykrycie typu; & unikamy kopiowania
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

string okreslOceneKoncowa(double srednia) {
    if (srednia >= 91) return "bardzo dobry";
    else if (srednia >= 81) return "dobry plus";
    else if (srednia >= 71) return "dobry";
    else if (srednia >= 61) return "dostateczny plus";
    else if (srednia >= 51) return "dostateczny";
    else return "niedostateczny";
}
