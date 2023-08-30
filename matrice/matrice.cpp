#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip> // Pentru setarea preciziei afișării numerelor
#include <cmath>   // Pentru funcția sqrt()

using namespace std;

struct Point {
    float x, y;
};

template<typename T>
class Matrix {
private:
    vector<vector<T>> data; // Matricea propriu-zisă
    int rows; // Numărul de rânduri al matricii
    int columns; // Numărul de coloane al matricii

public:
    Matrix(int numRows, int numColumns) : rows(numRows), columns(numColumns) {
        data.resize(rows, vector<T>(columns)); // Redimensionarea matricii interne în funcție de numărul de rânduri și coloane
    }

    void setElement(int row, int column, T value) {
        data[row][column] = value; // Setarea unei valori specifice într-un element al matricii
    }

    void setElement(int row, int column, float x, float y) {
        Point p;
        p.x = x;
        p.y = y;
        data[row][column] = p; // Setarea unui punct într-un element al matricii
    }

    T getElement(int row, int column) const {
        return data[row][column]; // Returnează elementul specificat din matrice
    }

    float calculateDistance(const Point& p1, const Point& p2) {
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        return sqrt(dx * dx + dy * dy);
    }

    void transformElements(const string& outputFile);

    void printMatrix() const;
};

template<typename T>
void Matrix<T>::transformElements(const string& outputFile) {
    ofstream outFile(outputFile); // Deschiderea fișierului pentru scriere

    if (!outFile.is_open()) {
        cout << "Nu s-a putut deschide fisierul pentru scriere!" << endl;
        return;
    }

    // Adăugarea primei linii cu valoarea specificată
    outFile << "DGS0,FO0.0,CH34,LP29,BI0,TM0,DP2;FOR0:0,WX210,HY297;" << endl;

    // Parcurgerea tuturor elementelor matricii
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            // Verificați dacă elementul este un spațiu gol
            if (data[i][j].x != -1.0 && data[i][j].y != -1.0) {
                // Scrierea coordonatelor în fișier cu precizia de 6 zecimale
                outFile << fixed << setprecision(6) << data[i][j].x << ":" << data[i][j].y << endl;
            }
        }
    }

    outFile.close(); // Închiderea fișierului
}

template<typename T>
void Matrix<T>::printMatrix() const {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            cout << "(" << data[i][j].x << ", " << data[i][j].y << ") "; // Afișarea elementelor matricii ca puncte
        }
        cout << endl;
    }
}

int main() {
    int numRows, numColumns;
    cout << "Introdu numarul de randuri al matricii: ";
    cin >> numRows;
    cout << "Introdu numarul de coloane al matricii: ";
    cin >> numColumns;

    Matrix<Point> mat(numRows, numColumns); // Crearea obiectului mat de tip Matrix<Point>

    // Parcurgerea tuturor elementelor matricii și setarea coordonatelor punctelor
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numColumns; j++) {
            mat.setElement(i, j, j, i); // Setarea coordonatelor punctelor în matrice
        }
    }

    float distance;
    cout << "Introdu distanta dorita: ";
    cin >> distance;

    // Filtrarea punctelor și crearea spațiilor goale în matrice (stânga-dreapta)
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numColumns; j++) {
            for (int k = j + 1; k < numColumns; k++) {
                if (mat.calculateDistance(mat.getElement(i, j), mat.getElement(i, k)) < distance) {
                    mat.setElement(i, k, -1.0, -1.0); // Setează un spațiu gol
                }
                else {
                    break; // Oprire verificare dacă distanța este mai mare
                }
            }
        }
    }

    // Filtrarea punctelor și crearea spațiilor goale în matrice (sus-jos)
    for (int j = 0; j < numColumns; j++) {
        for (int i = 0; i < numRows; i++) {
            for (int k = i + 1; k < numRows; k++) {
                if (mat.calculateDistance(mat.getElement(i, j), mat.getElement(k, j)) < distance) {
                    mat.setElement(k, j, -1.0, -1.0); // Setează un spațiu gol
                }
                else {
                    break; // Oprire verificare dacă distanța este mai mare
                }
            }
        }
    }

    string outputFile = "coordonate.ibe"; // Numele fișierului în care vor fi scrise coordonatele
    mat.transformElements(outputFile); // Transformarea elementelor și scrierea coordonatelor în fișier

    return 0;
}
