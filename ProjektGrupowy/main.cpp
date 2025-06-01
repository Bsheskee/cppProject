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

double obliczSrednia(const int oceny[], int rozmiar);

struct STUDENT {
    string imieNazwisko;
    int oceny[6];
    double srednia;
    string ocenaKoncowa;
};

int main() {
    ifstream plik("dane.txt");
    vector<STUDENT> studenci;
    
    if (!plik.is_open()) {
        cerr << "Nie mozna otworzyc pliku!" << endl;
        return 1;
    }
    
    string linia;
    while (getline(plik, linia)) {
        STUDENT student;
        student.imieNazwisko = linia;
        
        if (getline(plik, linia)) {
            istringstream iss(linia); // do odczytu ocen z pliku jako int zamiast string <sstream>
            for (int i = 0; i < 6; i++) {
                iss >> student.oceny[i]; // pobieramy oceny jako inty z pliku dla każdego studenta
            }
            student.srednia = obliczSrednia(student.oceny, 6);
            studenci.push_back(student);
        }
    }
    plik.close();

    // Wyświetlenie wyników
    cout << "Dane studentów:" << endl;
    cout << "-------------------------------------------------------" << endl;
    cout << setw(20) << left << "Imię i nazwisko"
    << setw(25) << "Oceny"
    << setw(10) << "Średnia"
    << "Ocena końcowa" << endl;
    cout << "-------------------------------------------------------" << endl;
    
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
        cout << endl;
//        cout << student.ocenaKoncowa << endl;
    }
    
    return 0;
}

double obliczSrednia(const int oceny[], int rozmiar) {
    double suma = 0;
    for (int i = 0; i < rozmiar; i++) {
        suma += oceny[i];
    }
    return suma / rozmiar;
}
