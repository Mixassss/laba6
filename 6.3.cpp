#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

const int SizeMatrix = 4; // Количество уравнений и переменных


// Функция для вычисления нового значения переменной xi на k-й итерации 
double calculateNewValue(int k, int i, double** matrix, double* reply, double* x) {
    double sum1 = 0, sum2 = 0;
    for (int j = 0; j < i; ++j) {
        sum1 += matrix[i][j] * x[j]; // Сумма соседних переменных с меньшим индексом aij * xj(k+1) 
    }
    for (int j = i + 1; j < SizeMatrix; ++j) {
        sum2 += matrix[i][j] * x[j]; // Сумма соседних переменных с большим индексом 
    }
    return (reply[i] - sum1 - sum2) / matrix[i][i]; // Новое значение переменной xi  ; основная формула
}

// Функция для решения системы уравнений методом Зейделя 
void solveSystem(double** matrix, double* reply, double* x, double epsilon, int maxIterations) {
    double* xNew = new double[SizeMatrix](); // Новое приближение к решению 
    int iterations = 0; // Счетчик итераций 

    cout << "|-----|----------|----------|----------|----------|-------------|" << endl;
    cout << "|  N  |    x1    |    x2    |    x3    |    x4    |   Epsilon   |" << endl;
    cout << "|-----|----------|----------|----------|----------|-------------|" << endl;

    while (iterations < maxIterations) {
        double maxDiff = 0; // Максимальное изменение переменной на текущей итерации 
        for (int i = 0; i < SizeMatrix; ++i) {
            double oldValue = xNew[i]; //обновляем старое значение 
            xNew[i] = calculateNewValue(iterations, i, matrix, reply, xNew); // Вычисление нового значения переменной xi 
            maxDiff = max(maxDiff, abs(xNew[i] - oldValue)); // Обновление максимального изменения  для сравнения с epsilon
        }
        cout << "|  " << iterations << "  | " << setw(8) << xNew[0] << " | " << setw(8) << xNew[1] << " | " << setw(8) << xNew[2] << " | " << setw(8) << xNew[3] << " | " << setw(11) << epsilon << " |" << endl;
        if (maxDiff < epsilon) { // Проверка условия сходимости 
            cout << "|-----|----------|----------|----------|----------|-------------|" << endl << endl;
            cout << "Количество итераций:  " << iterations << endl;
            break;
        }
        iterations++;
    }



    // Вывод решения 
    cout << "Решение:" << endl;
    for (int i = 0; i < SizeMatrix; ++i) {
        cout << "x" << i + 1 << " = " << xNew[i] << endl;
    }

}

void canonicalForm(double** matrix, double* reply) { //вывод канонического вида
    cout << "Канонический вид: " << endl;
    string variables[] = { "x1", "x2", "x3", "x4"};
    for (int i = 0; i < SizeMatrix; ++i) {
        for (int j = 0; j < SizeMatrix; ++j) {
            cout << "(" << setw(4) << fixed << setprecision(3) << matrix[i][j] << "*" << variables[j] << ")";
            if (j < SizeMatrix - 1) {
                cout << " + ";
            }
        }
        cout << " = " << reply[i] << endl;
    }
    cout << endl;
}


// Функция для выполнения LU-разложения
void luDecomposition(double** A, double** L, double** U, int n) {

    for (int i = 0; i < n; i++) { //// Проходим по каждой строке матрицы A (размерность n)
        // Вычисление элементов верхнетреугольной матрицы U
        for (int j = i; j < n; j++) { // Внутренний цикл этого цикла проходится по столбцам матрицы A, начиная с i-го столбца. 
            //Переменная j здесь используется для индексации столбцов.
           
            double sum = 0;
            // Вычисление суммы произведений элементов нижнетреугольной матрицы L и верхнетреугольной матрицы U
            for (int k = 0; k < i; k++) {
                sum += L[i][k] * U[k][j];
            }
            // Вычисление элемента верхнетреугольной матрицы U
            U[i][j] = A[i][j] - sum;
        }

        // Вычисление элементов нижнетреугольной матрицы L
        for (int i1 = i; i1 < n; i1++) {
            int j = i; // столбец для L
            // Если индекс строки равен индексу столбца, устанавливаем соответствующий элемент матрицы L в 1
            if (i == i1) { //диагонали
                L[i][i1] = 1;
            }else {
                // Инициализация суммы нулем
                double sum = 0;
                // Вычисление суммы произведений элементов нижнетреугольной матрицы L и верхнетреугольной матрицы U
                for (int k = 0; k < j; k++) {
                    sum += L[i1][k] * U[k][j];
                }
                // Вычисление элемента нижнетреугольной матрицы L
                L[i1][j] = (A[i1][j] - sum) / U[i][j];
            }
        }
    }



    // Вывод матриц U и L
    cout << "Матрица U:" << endl;
    for (int i = 0; i < SizeMatrix; ++i) {
        for (int j = 0; j < SizeMatrix; ++j) {
            cout << fixed << setprecision(4) << setw(8) << U[i][j] << " ";
        }
        cout << endl;
    }

    cout << "Матрица L:" << endl;
    for (int i = 0; i < SizeMatrix; ++i) {
        for (int j = 0; j < SizeMatrix; ++j) {
            cout << fixed << setprecision(4) << setw(8) << L[i][j] << " ";
        }
        cout << endl;
    }
}


// Функция для выполнения прямой подстановки
double* forwardSubstitution(double** L, double* b, int n) { //Решаем Ly = b
    // Создаем массив y для хранения результатов прямой подстановки
    double* y = new double[n]();
    // Проходимся по каждой строке матрицы L
    for (int k = 0; k < n; k++) {
        // Инициализируем y[k] значением b[k]
        y[k] = b[k];
        // Проходимся по каждому столбцу до диагонального элемента
        for (int j = 0; j < k; j++) {
            // Вычитаем сумму произведений из y[k]
            y[k] -= L[k][j] * y[j];
        }
    }
    // Возвращаем массив y
    return y;
}

// Функция для выполнения обратной подстановки
double* backwardSubstitution(double** U, double* y, int n) { // Решаем Ux = y
    // Создаем массив x для хранения результатов обратной подстановки
    double* x = new double[n]();
    // Проходимся по каждой строке матрицы U, начиная с последней
    for (int k = n - 1; k >= 0; k--) {
        // Инициализируем x[k] значением y[k]
        x[k] = y[k];
        // Проходимся по каждому столбцу после диагонального элемента
        for (int j = k + 1; j < n; j++) {
            // Вычитаем сумму произведений из x[k]
            x[k] -= U[k][j] * x[j];
        }
        // Делим x[k] на соответствующий диагональный элемент U[k][k]
        x[k] /= U[k][k];
    }
    // Возвращаем массив x
    return x;
}


int main() {
    setlocale(LC_ALL, "ru");
    double M = -0.88, N = 0.1, P = 0.91; // переменные по варианту
    double epsilon = 0.001; // точность
    int maxIterations = 1000; // макс. кол-во итераций

    int n = 4; // размер матрицы

    // Создание матрицы A и вектора b
    double** A = new double* [n];
    for (int i = 0; i < n; i++) {
        A[i] = new double[n];
    }

    A[0][0] = M; A[0][1] = -0.04; A[0][2] = 0.21; A[0][3] = -18;
    A[1][0] = 0.25; A[1][1] = -1.23; A[1][2] = N; A[1][3] = -0.09;
    A[2][0] = -0.21; A[2][1] = N; A[2][2] = 0.8; A[2][3] = -0.13;
    A[3][0] = 0.15; A[3][1] = -1.31; A[3][2] = 0.06; A[3][3] = P;

    double* b = new double[n]; //вектор b
    b[0] = -1.24; b[1] = P; b[2] = 2.56; b[3] = M;

    // Создание матрицы L и U
    double** L = new double* [n];  //выделяем память под массив указателей
    double** U = new double* [n]; 
    for (int i = 0; i < n; i++) {
        L[i] = new double[n](); //выделяем память для каждой строки
        U[i] = new double[n]();
    }

    // Выполнение LU-разложения
    luDecomposition(A, L, U, n);


    // Прямая подстановка для нахождения y
    double* y = forwardSubstitution(L, b, n);

    // Обратная подстановка для нахождения x
    double* x = backwardSubstitution(U, y, n);

    // Вывод результатов прямой подстановки
    cout << "Результаты прямой подстановки (нахождение y):" << endl;
    for (int i = 0; i < n; i++) {
        cout << "y" << i + 1 << " = " << y[i] << endl;
    }

    // Вывод результатов обратной подстановки
    cout << "Результаты обратной подстановки (нахождение x):" << endl;
    for (int i = 0; i < n; i++) {
        cout << "x" << i + 1 << " = " << x[i] << endl;
    }


    //2 Способ

    // Матрица коэффициентов системы уравнений  с наибольшими элементами по диагонали
    double** matrix = new double* [SizeMatrix];
    for (int i = 0; i < SizeMatrix; ++i) {
        matrix[i] = new double[SizeMatrix];
    }

    matrix[0][0] = 0.25; matrix[0][1] = -1.23; matrix[0][2] = 0.2; matrix[0][3] = -0.09;
    matrix[1][0] = -0.15; matrix[1][1] = 1.31; matrix[1][2] = -0.06; matrix[1][3] = -0.88;
    matrix[2][0] = -0.21; matrix[2][1] = 0.2; matrix[2][2] = 0.8; matrix[2][3] = -0.13;
    matrix[3][0] = 1.21; matrix[3][1] = 0.04; matrix[3][2] = -0.21; matrix[3][3] = 18;

    // Вектор свободных членов 
    double* reply = new double[SizeMatrix];
    reply[0] = 0.88; reply[1] = 1.21; reply[2] = 2.56; reply[3] = 1.24;

    canonicalForm(matrix, reply);

    // Начальное приближение к решению (все переменные равны 0) 
    double* x1 = new double[SizeMatrix]();

    // Решение системы уравнений 
    solveSystem(matrix, reply, x1, epsilon, maxIterations);

    return 0;
}
