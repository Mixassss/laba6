// Вариант 10
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <time.h>

using namespace std;

void Generate_Matrix(int** matrix, int m, int n) { // ф-ия заполнения матрицы рандомными значениями [-50; 50] и её вывод
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            matrix[i][j] = rand() % 49 + 50;
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

int find_elements(vector<int>& nums, int** matrix, int m, int n) { // ф-ия нахождения неотрицательных элементов матрицы, стоящих в чётных строках, на нечётных местах. И их суммы
    int sum = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (((i+1) % 2 == 0) && ((j+1) % 2 != 0) && (matrix[i][j] >= 0)) {
                nums.push_back(matrix[i][j]);
                sum += matrix[i][j];
            }
        }
    }
return sum;
}

template <typename T>
ostream& operator<<(ostream& os, const vector<T>& vec) { // перегрузка вывода вектора
    for (const auto& element : vec) {
        os << element << " ";
    }
    return os;
}

int main() {
    setlocale(LC_ALL, "Rus");
    srand(time(0));

    int m, n;
    cout << "Введите размерность матрицы: ";
    cin >> m >> n;

    int **matrix = new int*[m];
    for(int i = 0; i < m; i++) {
        matrix[i] = new int[n];
    }
    Generate_Matrix(matrix, m, n);
 
    vector<int> nums; 
    int sum = find_elements(nums, matrix, m, n);
    cout << "Массив неотрицательных элементов матрицы, стоящих в чётных строках, на нечётных местах: " << nums << endl;
    cout << "Сумма этих элементов: " << sum << endl;

    return 0;
}
