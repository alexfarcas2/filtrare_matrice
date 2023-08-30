#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <cmath>

using namespace std;

// Structura pentru a reprezenta un punct în spațiu
struct Point {
    float x, y;
};

// Clasa pentru matrice
template<typename T>
class Matrix {
private:
    vector<vector<T>> data; // Stocarea datelor matricei
    int rows; // Numărul de rânduri
    int columns; // Numărul de coloane

public:
    // Constructorul clasei Matrix
    Matrix(int numRows, int numColumns) : rows(numRows), columns(numColumns) {
        data.resize(rows, vector<T>(columns)); // Inițializarea dimensiunilor matricei
    }

    // Metodă pentru a seta un element în matrice
    void setElement(int row, int column, T value) {
        data[row][column] = value;
    }

    // Metodă pentru a seta un element de tip Point în matrice
    void setElement(int row, int column, float x, float y) {
        Point p;
        p.x = x;
        p.y = y;
        data[row][column] = p;
    }

    // Metodă pentru a obține un element din matrice
    T getElement(int row, int column) const {
        return data[row][column];
    }

    // Metodă pentru a calcula distanța euclidiană între două puncte
    float calculateDistance(const Point& p1, const Point& p2) {
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        return sqrt(dx * dx + dy * dy);
    }

    // Metodă pentru a filtra matricea și a marca punctele care se află la o distanță mai mică decât o valoare dată
    void filterMatrix(float distance) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                for (int k = i + 1; k < rows; k++) {
                    for (int l = j + 1; l < columns; l++) {
                        if (calculateDistance(data[i][j], data[k][l]) < distance) {
                            data[k][l].x = -1.0;
                            data[k][l].y = -1.0;
                            data[i][l].x = -1.0;
                            data[i][l].y = -1.0;
                            data[k][j].x = -1.0;
                            data[k][j].y = -1.0;
                            data[i][j].x = -1.0;
                            data[i][j].y = -1.0;
                        }
                    }
                }
            }
        }
    }

    // Metodă pentru a transforma elementele matricei într-un fișier de ieșire
    void transformElements(const string& outputFile) {
        ofstream outFile(outputFile);

        if (!outFile.is_open()) {
            cout << "Nu s-a putut deschide fisierul pentru scriere!" << endl;
            return;
        }

        outFile << "DGS0,FO0.0,CH34,LP29,BI0,TM0,DP2;FOR0:0,WX210,HY297;" << endl;

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                if (data[i][j].x != -1.0 && data[i][j].y != -1.0) {
                    outFile << fixed << setprecision(2) << data[i][j].x << ":" << fixed << setprecision(2) << data[i][j].y << endl;
                }
            }
        }

        outFile.close();
    }

    // Metodă pentru a afișa matricea pe consolă
    void printMatrix() const {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                cout << "(" << data[i][j].x << ", " << data[i][j].y << ") ";
            }
            cout << endl;
        }
    }
};

int main() {
    int numRows, numColumns;
    cout << "Introdu numarul de randuri al matricii: ";
    cin >> numRows;
    cout << "Introdu numarul de coloane al matricii: ";
    cin >> numColumns;

    Matrix<Point> mat(numRows, numColumns); // Crearea obiectului matrice

    srand(time(0));
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numColumns; j++) {
            float x = static_cast<float>(rand()) / RAND_MAX * 99 + 1; // Generarea unei coordonate x aleatoare
            float y = static_cast<float>(rand()) / RAND_MAX * 99 + 1; // Generarea unei coordonate y aleatoare
            mat.setElement(i, j, x, y); // Setarea elementului în matrice
        }
    }

    float distance;
    cout << "Introdu distanta dorita: ";
    cin >> distance;

    mat.filterMatrix(distance); // Aplicarea filtrului pe matrice

    string outputFile = "coordonate.ibe";
    mat.transformElements(outputFile); // Transformarea elementelor și scrierea în fișierul de ieșire

    return 0;
}
