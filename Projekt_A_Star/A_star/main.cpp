#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct Wezel {
    int x;
    int y;
    double wartosc;
    Wezel* rodzic;

    Wezel(int x, int y, double wartosc, Wezel* rodzic) : x(x), y(y), wartosc(wartosc), rodzic(rodzic) {}
};

struct Komparator {
    bool operator()(Wezel* a, Wezel* b) {
        return a->wartosc > b->wartosc;
    }
};

vector<vector<int>> czytnik_tekstu(const string& nazwa_pliku) {
    ifstream plik(nazwa_pliku);
    vector<vector<int>> siatka;
    string linia;
    while (getline(plik, linia)) {
        vector<int> wiersz;
        stringstream ss(linia);
        int liczba;
        while (ss >> liczba) wiersz.push_back(liczba);
        siatka.push_back(wiersz);
    }
    plik.close();
    return siatka;
}

void odwroc_punkt(vector<int>& punkt, int n) {
    int temp = punkt[0];
    punkt[0] = n - 1 - punkt[1];
    punkt[1] = temp;
}

vector<vector<bool>> utworz_odwiedzone(vector<vector<int>> siatka) {
    int n = siatka.size();
    vector<vector<bool>> odwiedzone(n, vector<bool>(n, false));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (siatka[i][j] == 5) odwiedzone[i][j] = true;
    return odwiedzone;
}

bool znajdz_sciezke(vector<vector<int>>& siatka, vector<int>& start, vector<int>& koniec) {
    int n = siatka.size();
    odwroc_punkt(start, n);
    odwroc_punkt(koniec, n);
    vector<vector<int>> ruchy = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    priority_queue<Wezel*, vector<Wezel*>, Komparator> kolejka;
    vector<vector<bool>> odwiedzone = utworz_odwiedzone(siatka);
    kolejka.push(new Wezel(start[0], start[1], 0, nullptr));
    while (!kolejka.empty()) {
        Wezel* c = kolejka.top();
        kolejka.pop();
        if (odwiedzone[c->x][c->y]) continue;
        odwiedzone[c->x][c->y] = true;
        if (c->x == koniec[0] && c->y == koniec[1]) {
            Wezel* aktualny = c;
            while (aktualny != nullptr) {
                siatka[aktualny->x][aktualny->y] = 1;
                aktualny = aktualny->rodzic;
            }
            return true;
        }
        for (vector<int> r : ruchy) {
            int wiersz = c->x + r[0];
            int kolumna = c->y + r[1];
            if (!(wiersz < 0 || wiersz >= n || kolumna < 0 || kolumna >= n || odwiedzone[wiersz][kolumna])) {
                double roznica = c->wartosc + 1 + sqrt((wiersz - koniec[0]) * (wiersz - koniec[0]) + (kolumna - koniec[1]) * (kolumna - koniec[1]));
                kolejka.push(new Wezel(wiersz, kolumna, c->wartosc + 1, c));
            }
        }
    }
    return false;
}

void a_gwiazdka(vector<vector<int>>& siatka, vector<int>& start, vector<int>& koniec) {
    if (znajdz_sciezke(siatka, start, koniec)) {
        cout << "Znaleziono droge:" << endl;
        for (int i = 0; i < siatka.size(); i++) {
            for (int j = 0; j < siatka.size(); j++) cout << siatka[i][j] << " ";
            cout << endl;
        }
    } else {
        cout << "Nie znaleziono drogi.";
    }
}

int main() {
    vector<vector<int>> siatka = czytnik_tekstu("grid.txt");
    vector<int> start = {0, 0};
    vector<int> koniec = {19, 19};
    a_gwiazdka(siatka, start, koniec);

    return 0;
}
