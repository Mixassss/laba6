#include <iostream>
#include <algorithm>
#include <random>
#include <string>

using namespace std;

//создание матрицы
void createMatrix(int**& matrix, int m) {
    random_device rd; // Создание объекта random_device для генерации случайных чисел
    mt19937 gen(rd()); // Инициализация генератора случайных чисел mt19937 с использованием random_device
    uniform_int_distribution<> dis(-100, 100); // Создание равномерного распределения в диапазоне
    matrix = new int* [m]; // Выделение памяти для массива указателей на строки
    for (int i = 0; i < m; ++i) {
        matrix[i] = new int[m]; // Выделение памяти для каждой строки
        for (int j = 0; j < m; ++j) {
            matrix[i][j] = dis(gen); // Инициализация случайным числом
        }
    }
}

// Функция для вывода матрицы на экран
void printMatrix(int** matrix, int m) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
}

// Функция для сортировки столбцов матрицы
void sortColumns(int** matrix, int m) {
    // Итерация по каждому столбцу матрицы
    for (int j = 0; j < m; ++j) {
        // Создаем временный вектор для хранения текущего столбца
        vector<int> column;

        // Получаем j-ый столбец и добавляем его элементы в вектор
        for (int i = 0; i < m; ++i) {
            column.push_back(matrix[i][j]);
        }

        // Если номер столбца четный
        if (j % 2 == 0) {
            // Сортировка четных столбцов по возрастанию
            sort(column.begin(), column.end());
        }else {
            // Сортировка нечетных столбцов по убыванию 
            sort(column.begin(), column.end(), greater<int>());
        }

        // Заменяем элементы столбца в матрице отсортированными элементами из вектора
        for (int i = 0; i < m; ++i) {
            matrix[i][j] = column[i];
        }
    }
}



void countPositiveElements(int** matrix, int m, int& upperHalfPositives, int& lowerHalfPositives) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            if (i < j) { // Верхняя половина
                if (matrix[i][j] > 0) {
                    ++upperHalfPositives;
                }
            }
            else if (i == j) { // Главная диагональ
                if (matrix[i][j] > 0) {
                    ++upperHalfPositives;
                    ++lowerHalfPositives;
                }
            }else { // Нижняя половина
                if (matrix[i][j] > 0) {
                    ++lowerHalfPositives;
                }
            }
        }
    }
    if (lowerHalfPositives > upperHalfPositives) {
        cout << "В нижней половине матрицы больше положительных элементов. " << lowerHalfPositives << ">" << upperHalfPositives << endl;
    }
    else if (lowerHalfPositives < upperHalfPositives) {
        cout << "В верхней половине матрицы больше положительных элементов. " << lowerHalfPositives << "<" << upperHalfPositives << endl;
    }
    else {
        cout << "Количество элементов в верхней и нижней половине одинаково. " << lowerHalfPositives << "=" << upperHalfPositives << endl;
    }
}

int User(const string& prompt) {
    while (true) {
        try {
            cout << prompt; // Выводим приглашение к вводу
            string input; // Создаем строку для хранения ввода пользователя
            getline(cin, input); // Считываем строку из стандартного ввода
            size_t pos; // Переменная для хранения позиции, на которой закончилось преобразование
            int value = stoi(input, &pos); // Преобразуем строку в целое число
            if (pos != input.size() || value <= 5) { // Проверяем, были ли преобразованы все символы строки
                throw invalid_argument("Неверный формат числа. Пожалуйста, введите целое число больше 5(без пробелов)."); // Генерируем исключение, если не все символы были преобразованы
            }
            return value; // Возвращаем преобразованное целочисленное значение
        }
        catch (const invalid_argument& e) { // Обрабатываем исключение, если введены недопустимые символы
            cerr << "Ошибка: " << e.what() << endl; // Выводим сообщение об ошибке в стандартный поток ошибок 
        }
    }
}



int main() {
    setlocale(LC_ALL, "ru");
    int M = User("Введите порядок квадратной матрицы: ");

    int** matrix;
    createMatrix(matrix, M);

    cout << "Исходная матрица:" << endl;
    printMatrix(matrix, M);

    // Сортировка столбцов
    sortColumns(matrix, M);

    cout << "Матрица после сортировки:" << endl;
    printMatrix(matrix, M);

    // Подсчет положительных элементов в каждой половине матрицы
    int upperHalfPositives = 0, lowerHalfPositives = 0;
    countPositiveElements(matrix, M, upperHalfPositives, lowerHalfPositives);

    return 0;
}
