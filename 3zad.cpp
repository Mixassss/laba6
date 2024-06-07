#include <iostream>
#include <chrono>
#include <thread>
#include <random>

using namespace std;

// Размеры поля с учётом границ
const int width = 10;
const int height = 10;

// Функция для очистки экрана 
static void clear_screen() {
    cout << "\033[2J\033[1;1H";  // ANSI escape код для очистки экрана
}

// Функция для отображения текущего состояния поля
void display_field(int** field) {
    clear_screen(); // Очищаем экран перед выводом нового состояния поля
    // Перебираем все клетки поля и выводим их состояние
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1)
                cout << "*";  // Границы поля
            else if (field[i][j] == 1)
                cout << "#";  // Если клетка живая, выводим символ #
            else
                cout << " ";  // Если клетка мертвая, выводим пробел
        }
        cout << endl; // Переходим на новую строку после вывода строки клеток
    }
}

// Функция для вычисления следующего поколения
void next_generation(int**& field) {
    int** next_field = new int* [height]; // Создаём новое поле для следующего поколения
    for (int i = 0; i < height; ++i) {
        next_field[i] = new int[width]; // Выделяем память под каждую строку нового поля
    }

    // Перебираем каждую клетку текущего поколения и вычисляем её состояние в следующем поколении
    for (int i = 1; i < height - 1; ++i) { // не учитываем границы
        for (int j = 1; j < width - 1; ++j) {
            int neighbors = 0;
            // Подсчитываем количество соседей для каждой клетки
            for (int ni = -1; ni <= 1; ++ni) { 
                for (int nj = -1; nj <= 1; ++nj) {
                    if (ni == 0 && nj == 0) //проверка на текущую клетку
                        continue;  // Пропускаем текущую клетку
                    neighbors += field[i + ni][j + nj]; // Увеличиваем счётчик соседей для живых клеток
                }
            }
            // Применяем правила клеточного автомата "Жизнь"
            if (field[i][j] == 1) {
                if (neighbors < 2 || neighbors > 3) // если у живой клетки соседей меньше 2 или больше 3 она станоится мертвой
                    next_field[i][j] = 0;  // Смерть из-за перенаселённости или одиночества
                else
                    next_field[i][j] = 1;  // Выживание
            }else { // если у мертвой клетки  соседей 3,то она рождается,в другом случает остается мертвой
                if (neighbors == 3)
                    next_field[i][j] = 1;  // Рождение новой клетки
                else
                    next_field[i][j] = 0;  // Клетка остаётся мёртвой
            }
        }
    }

    // Удаление старого поля
    for (int i = 0; i < height; ++i) {
        delete[] field[i]; // оочищаем  память для каждой строки
    }
    delete[] field; // очищаем память массива указателей на строки

    // Передача указателя на новое поле
    field = next_field;
}

// Функция для инициализации поля случайными значениями
int** initialize_field() {
    int** field = new int* [height]; // Создание двумерного массива указателей для строк поля
    for (int i = 0; i < height; ++i) {
        field[i] = new int[width]; // Выделение памяти под каждую строку поля
    }

    // Заполнение поля нулями (все клетки мертвы)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            field[i][j] = 0; // Установка значения 0 (мертвая клетка) для каждой клетки поля
        }
    }

    return field; // Возвращаем указатель на инициализированное поле
}

// Функция для инициализации поля с шаблонами
void initialize_field_with_patterns(int** field) {
    // Очистка поля перед установкой планера
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            field[i][j] = 0; // Установка всех клеток в начальное состояние (мертвые)
        }
    }

    // Установка планера на поле
    field[2][3] = field[3][4] = field[4][2] = field[4][3] = field[4][4] = 1;  // Планер
}

int main() {
    // Инициализация поля
    int** field = initialize_field();

    // Инициализация с определенными шаблонами 
    initialize_field_with_patterns(field);

    int generation_count = 0; // Счётчик поколений

    // Анимация
    for (int generation = 0; generation < 100; ++generation) {
        display_field(field);
        if (generation_count == 13) {
            initialize_field_with_patterns(field); // Обновляем планер
            generation_count = 0; // Сбрасываем счётчик поколений
        }
        else {
            next_generation(field); // Вычисляем следующее поколение
            generation_count++; // Увеличиваем счётчик поколений
        }
        this_thread::sleep_for(chrono::milliseconds(100)); // Задержка в миллисекундах
    }

    // Освобождение памяти
    for (int i = 0; i < height; ++i) {
        delete[] field[i];
    }
    delete[] field;

    return 0;
}
