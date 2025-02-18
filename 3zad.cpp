#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
 
using namespace std;
 
// Размеры игрового поля
const int WIDTH = 50;
const int HEIGHT = 20;
 
// Символы для отображения живых и мертвых клеток
const char ALIVE_CELL = &apos;*&apos;;
const char DEAD_CELL = &apos;.&apos;;
 
// Задержка между кадрами (в миллисекундах)
const int FRAME_DELAY = 100;
 
// Функция для отрисовки текущего состояния игрового поля
void draw(const vector<vector<bool>>& grid) {
    system("cls"); // Очистка консоли
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            cout << (grid[y][x] ? ALIVE_CELL : DEAD_CELL); // Если true - живая кл, false - мертвая
        }
        cout << endl;
    }
}
 
// Функция для обновления состояния игрового поля на один шаг
void update(vector<vector<bool>>& grid) {
    vector<vector<bool>> newGrid = grid;
    // Применяем правила клеточного автомата
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            // Считаем количество живых соседей
            int aliveNeighbors = 0;
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if (dx == 0 && dy == 0) continue; // Пропускаем текущую клетку
                    int ny = (y + dy + HEIGHT) % HEIGHT;
                    int nx = (x + dx + WIDTH) % WIDTH;
                    if (grid[ny][nx]) aliveNeighbors++;
                }
            }
            // Применяем правила
            if (grid[y][x]) {
                if (aliveNeighbors < 2 || aliveNeighbors > 3) newGrid[y][x] = false; // Умирает от одиночества или перенаселения
            } else {
                if (aliveNeighbors == 3) newGrid[y][x] = true; // Воскрешается от трех соседей
            }
        }
    }
    grid = newGrid;
}
 
int main() {
    srand(time(0));
 
    // Инициализация игрового поля
    vector<vector<bool>> grid(HEIGHT, vector<bool>(WIDTH, false));
 
    // Пример «паровоза»
    grid[5][5] = true;
    grid[5][6] = true;
    grid[5][7] = true;
    grid[4][7] = true;
    grid[3][6] = true;
 
    // Основной цикл игры
    while (true) {
        draw(grid);
        update(grid);
        this_thread::sleep_for(chrono::milliseconds(FRAME_DELAY)); // Задержка между кадрами
    }
 
    return 0;
}
