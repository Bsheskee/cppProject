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
double obliczSredniaWszystkich(double suma, int rozmiar);
double wynikPowyzejSredniej(double sredniaStudenta, double sredniaStudentow);
string okreslOceneKoncowa(double srednia);

struct STUDENT {
    string imieNazwisko;
    int oceny[6];
    double srednia;
    string ocenaKoncowa;
};

int main() {
    ifstream plik("dane.txt");
    vector<STUDENT> studenci;
    double sumaOcenWszystkich = 0.0;
    double sredniaWszystkich = 0.0;
    
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
            student.ocenaKoncowa = okreslOceneKoncowa(student.srednia);
            sumaOcenWszystkich += student.srednia;
            
            studenci.push_back(student);
        }
    }
    plik.close();
    sredniaWszystkich = obliczSredniaWszystkich(sumaOcenWszystkich, int(studenci.size()));

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

double obliczSrednia(const int oceny[], int rozmiar) {
    double suma = 0;
    for (int i = 0; i < rozmiar; i++) {
        suma += oceny[i];
    }
    return suma / rozmiar;
}
double obliczSredniaWszystkich(double suma, int rozmiar) {
    return suma / rozmiar;
}
double wynikPowyzejSredniej(double sredniaStudenta, double sredniaStudentow) {
    double wynikPowyzejSredniej = 0.0;
    if (sredniaStudenta > sredniaStudentow) {
        wynikPowyzejSredniej = sredniaStudenta;
    }
    return wynikPowyzejSredniej;
}

string okreslOceneKoncowa(double srednia) {
    if (srednia >= 91) return "bardzo dobry";
    else if (srednia >= 81) return "dobry plus";
    else if (srednia >= 71) return "dobry";
    else if (srednia >= 61) return "dostateczny plus";
    else if (srednia >= 51) return "dostateczny";
    else return "niedostateczny";
}
