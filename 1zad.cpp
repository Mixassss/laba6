```
#include <iostream> 
#include <random>
#include <string>
#include <iomanip>

using namespace std;

// Функция для создания и инициализации матрицы
double** createMatrix(int m, int n) {
    random_device rd; // Создание объекта random_device для генерации случайных чисел
    mt19937 gen(rd()); // Инициализация генератора случайных чисел mt19937 с использованием random_device
    uniform_real_distribution<> dis(0.0, 100.0); // Создание равномерного распределения в диапазоне [0.0, 100.0)

    double** matrix = new double* [m]; // Выделение памяти под массив указателей на массивы (матрицу)
    for (int i = 0; i < m; ++i) { // Цикл по строкам матрицы
        matrix[i] = new double[n]; // Выделение памяти под массив значений для текущей строки
        for (int j = 0; j < n; ++j) { // Цикл по столбцам матрицы
            matrix[i][j] = dis(gen); // Заполнение текущего элемента матрицы случайным числом из указанного диапазона
        }
    }
    return matrix; // Возврат указателя на созданную матрицу
}

// Функция для вывода матрицы на экран
void printMatrix(double** matrix, int m, int n) {
    cout << "Матрица:" << endl; // Вывод заголовка для матрицы
    for (int i = 0; i < m; ++i) { // Цикл по строкам матрицы
        for (int j = 0; j < n; ++j) { // Цикл по столбцам матрицы
            cout << fixed << setprecision(2) << matrix[i][j] << "\t"; // Вывод текущего элемента матрицы с точностью 2 знака после запятой и фиксированным форматом
        }
        cout << endl; // Переход на новую строку после вывода всех элементов текущей строки матрицы
    }
}


// Функция для вычисления среднего арифметического значения всех элементов матрицы
double TotalAvg(double** matrix, int m, int n) {
    double totalSum = 0; // Общая сумма всех элементов матрицы
    for (int i = 0; i < m; ++i) { // Цикл по строкам матрицы
        for (int j = 0; j < n; ++j) { // Цикл по столбцам матрицы
            totalSum += matrix[i][j]; // Суммирование текущего элемента матрицы
        }
    }
    // Возвращаем среднее арифметическое, разделив общую сумму на количество элементов
    return totalSum / (static_cast<double>(m) * n);
}

// Функция для вычисления среднего арифметического значения каждой строки и поиска строки с максимальным значением
void RowAverages(double** matrix, int m, int n, double* StrAverages, int& maxStroka) {
    double maxRowAvg = 0; // Максимальное среднее значение строки
    for (int i = 0; i < m; ++i) { // Цикл по строкам матрицы
        double rowSum = 0; // Сумма элементов в текущей строке
        for (int j = 0; j < n; ++j) { // Цикл по столбцам матрицы
            rowSum += matrix[i][j]; // Суммирование элементов текущей строки
        }
        // Вычисляем среднее значение текущей строки
        StrAverages[i] = rowSum / static_cast<double>(n);
        // Если среднее значение текущей строки больше максимального,
        // обновляем максимальное значение и индекс строки
        if (StrAverages[i] > maxRowAvg) {
            maxRowAvg = StrAverages[i];
            maxStroka = i;
        }
    }
}

// Функция для получения целочисленного значения от пользователя с обработкой исключений
int User(const string& prompt) {
    while (true) {
        try {
            cout << prompt; // Выводим приглашение к вводу
            string input; // Создаем строку для хранения ввода пользователя
            getline(cin, input); // Считываем строку из стандартного ввода
            size_t pos; // Переменная для хранения позиции, на которой закончилось преобразование
            int value = stoi(input, &pos); // Преобразуем строку в целое число
            if (pos != input.size()) { // Проверяем, были ли преобразованы все символы строки
                throw invalid_argument("Неверный формат числа. Пожалуйста, введите целое число(без пробелов)."); // Генерируем исключение, если не все символы были преобразованы
            }
            return value; // Возвращаем преобразованное целочисленное значение
        }
        catch (const invalid_argument& e) { // Обрабатываем исключение, если введены недопустимые символы
            cerr << "Ошибка: " << e.what() << endl; // Выводим сообщение об ошибке в стандартный поток ошибок 
        }
    }
}
void DisplayTask1() {
    cout << "---Задание номер 1---" << endl;
    int m = User("Введите количество строк: ");
    int n = User("Введите количество столбцов: ");

    double** matrix = createMatrix(m, n);
    printMatrix(matrix, m, n);

    double totalAvg = TotalAvg(matrix, m, n);
    cout << "\nСреднее арифметическое значение всех элементов матрицы: " << totalAvg << endl;

    double* StrAverages = new double[m];
    int maxstroka = 0;
    RowAverages(matrix, m, n, StrAverages, maxstroka);

    cout << "\nСредние арифметические значения каждой строки:" << endl;
    for (int i = 0; i < m; ++i) {
        cout << "Строка " << i + 1 << ": " << StrAverages[i] << endl;
    }

    cout << "\nНомер строки с наибольшим средним арифметическим значением: " << maxstroka + 1 << endl;

}
 

// Основная функция
int main() {
    setlocale(LC_ALL, "ru");
    DisplayTask1();

    return 0;
} 
