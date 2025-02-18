#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
 
using namespace std;
 
template <typename T>
ostream& operator<<(ostream& os, const vector<T>& vec) { // перегрузка вывода вектора
    for (const auto& element : vec) {
        os << element << " ";
    }
    return os;
}
 
void Generate_Matrix(int** matrix, int m) { // ф-ия заполнения квадратной матрицы рандомными значениями [100:200] и её вывод
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            matrix[i][j] = rand() % 99 + 100;
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}
 
void find_maxstr_inmatrix(int** matrix, int m) { // ф-ия нахождения строки со второй максимальной суммой элементов строк квадратной матрицы
    int sum = 0;
    vector<int> sums;
    for (int i = 0; i < m; i++ ) { // заполняем массив сумм элементов строк кв. матрицы
        sum = 0;
        for (int j = 0; j < m; j++ ) {
            sum += matrix[i][j];
        }
        sums.push_back(sum);
    }
    cout << "Массив сумм элементов строк квадратной матрицы: " << sums << endl;
    int maxsumindex = distance(sums.begin(), max_element(sums.begin(), sums.end())); // находим индекс строки с максимальной суммой элементов
    sums[maxsumindex] = 0; // обнуляем максимальную сумму
    maxsumindex = distance(sums.begin(), max_element(sums.begin(), sums.end())); // ещё раз находим индекс макс. суммы элементов
    cout << "Строка со второй максимальной суммой элементов строк квадратной матрицы: ";
    for (int i = 0; i < m; ++i) {
        cout << matrix[maxsumindex][i] << " ";
    }
}
 
int main() {
    setlocale(LC_ALL, "Rus");
    srand(time(0));
 
    int m;
    cout << "Введите размерность квадратной матрицы: ";
    cin >> m;
 
    int **matrix = new int*[m];
    for(int i = 0; i < m; i++) {
        matrix[i] = new int[m];
    }
    Generate_Matrix(matrix, m);
 
    find_maxstr_inmatrix(matrix, m);
 
    return 0;
}
