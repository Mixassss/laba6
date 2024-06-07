#include "3l.h"
#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <Windows.h>

using namespace std;

// Функция для генерации случайного ключа
void generate_random_key(vector<unsigned char>& key) {
    key.clear();
    key.resize(16); // 16 байт для AES-128

    random_device rd; // Создание объекта random_device для генерации случайных чисел
    mt19937 gen(rd()); // Инициализация генератора случайных чисел mt19937 с использованием random_device
    uniform_int_distribution<> dis(0, 255);

    for (int i = 0; i < 16; ++i) {
        key[i] = dis(gen); // генерируем ключ
    }
}

// Функция для записи ключа в файл
void write_key_to_file(const vector<unsigned char>& key, const string& filename) {
    ofstream outputFile(filename, ios::binary); // в двоичной записи
    if (outputFile.is_open()) {
        for (unsigned char ch : key) {
            outputFile.write(reinterpret_cast<const char*>(&ch), sizeof(ch)); //преобразуем un char в char,для метода write,возвращаем кол-во байтов записанных в файл
        }
        outputFile.close();
        cout << "Ключ успешно записан в файл " << filename << endl;
    }else {
        cout << "Не удалось открыть файл для записи ключа" << endl;
    }
}


//функция для ввода строки
void init_str(vector<unsigned char>& str) {
    string in_str;

    cout << "(Строка должна быть не больше 16 символов): ";

    while (true) {
        getline(cin, in_str);

        if (in_str.length() > 16) {
            cout << "Строка больше чем 16 символов, попробуйте еще: ";
        }

        else {
            str.clear(); // Очищаем вектор перед добавлением новых данных
            str.reserve(in_str.size()); // Резервируем место для элементов
            for (char c : in_str) {
                str.push_back(static_cast<unsigned char>(c));
            }
            break;
        }
    }
}

//функция для заполнения матрицы 
void init_matrix(vector<unsigned char>& str, vector<vector<unsigned char>>& matrix) {
    // Очищаем матрицу перед заполнением новыми данными
    matrix.clear();

    // Инициализируем матрицу размером 4x4 нулями
    matrix.resize(4, vector<unsigned char>(4, 0));

    // Проходим по каждой строке матрицы
    for (int i = 0; i < 4; ++i) {
        // Проходим по каждому столбцу матрицы
        for (int j = 0; j < 4; ++j) {
            // Вычисляем индекс элемента матрицы
            int index = i * 4 + j;

            // Проверяем, что индекс не выходит за пределы размера вектора str
            if (index < str.size()) {
                // Присваиваем элементу матрицы значение из вектора str
                matrix[i][j] = str[index];
            }
        }
    }
}

//функция для транспонирование матрицы
void transpose_matrix(vector<vector<unsigned char>>& matrix) {
    // Получаем размерность квадратной матрицы
    int n = matrix.size();

    // Проходим по каждой строке матрицы
    for (int i = 0; i < n; ++i) {
        // Проходим по каждому столбцу, начиная с i+1, чтобы избежать повторной транспонирования уже транспонированных элементов
        for (int j = i + 1; j < n; ++j) {
            // Обмениваем значения элементов (i, j) и (j, i), чтобы выполнить транспонирование
            swap(matrix[i][j], matrix[j][i]);
        }
    }
}

//функция для расширения ключа
vector<vector<unsigned char>> key_expansion(vector<vector<unsigned char>>& matrix, int t) {
    // Создаем новый ключ, инициализируем его значениями из исходной матрицы
    vector<vector<unsigned char>> exp_key(matrix);

    // Создаем временный вектор для хранения последнего столбца матрицы
    vector<unsigned char> temp(4);

    // Копируем последний столбец матрицы во временный вектор
    for (int i = 0; i < 4; ++i) {
        temp[i] = matrix[i][3];
    }

    // Циклический сдвиг элементов вектора на одну позицию влево
    rotate(temp.begin(), temp.begin() + 1, temp.end());

    // Заменяем каждый элемент вектора с использованием S-box
    for (int i = 0; i < 4; ++i) {
        temp[i] = sBox[temp[i] / 16][temp[i] % 16];
    }

    // Выполняем XOR с раундовой константой rcon
    temp[0] = temp[0] ^ r_w[t][0];

    // Генерируем первый столбец нового ключа путем XOR-а каждого элемента соответствующего столбца матрицы и временного вектора
    for (int i = 0; i < 4; ++i) {
        exp_key[i][0] = matrix[i][0] ^ temp[i];
    }

    // Генерируем остальные столбцы нового ключа путем XOR-а предыдущего столбца нового ключа и соответствующего столбца матрицы
    for (int i = 1; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            exp_key[j][i] = exp_key[j][i - 1] ^ matrix[j][i];
        }
    }

    // Возвращаем новый расширенный ключ
    return exp_key;
}

//функция  операции XOR для каждой пары элементов матрицы matrix1 и matrix2 и сохраняет результат в новой матрице, которая затем возвращается из функции
vector<vector<unsigned char>> xor_matrices(const vector<vector<unsigned char>>& matrix1, const vector<vector<unsigned char>>& matrix2) {
    // Получаем размерность матрицы
    int n = matrix1.size();

    // Создаем новую матрицу для хранения результата
    vector<vector<unsigned char>> result(n, vector<unsigned char>(n));

    // Проходим по каждой строке матрицы
    for (int i = 0; i < n; ++i) {
        // Проходим по каждому столбцу матрицы
        for (int j = 0; j < n; ++j) {
            // Вычисляем значение элемента новой матрицы как побитовое исключающее ИЛИ элементов соответствующих позиций в матрицах matrix1 и matrix2
            result[i][j] = matrix1[i][j] ^ matrix2[i][j];
        }
    }

    // Возвращаем результат
    return result;
}


//функция  применяется к каждому байту матрицы, заменяя его значением из S - блока.(Перестановка с помощью S-блока)
void sub_bytes(vector<vector<unsigned char>>& matrix) {
    // Проходим по каждой строке матрицы
    for (int i = 0; i < 4; i++) {
        // Проходим по каждому столбцу матрицы
        for (int j = 0; j < 4; j++) {
            // Получаем текущий байт и используем его значение в качестве индексов для S-блока
            unsigned char byte = matrix[i][j]; //текущий байт
            matrix[i][j] = sBox[byte / 16][byte % 16]; 
        }
    }
}

//функция Перестановка с помощью P-блока
void shiftRows(vector<vector<unsigned char>>& state) {
    // Проходим по каждой строке матрицы, начиная с первой (индекс 1)
    for (int i = 1; i < 4; ++i) {
        // Сдвигаем элементы в текущей строке влево на количество позиций, равное индексу строки
        rotate(state[i].begin(), state[i].begin() + i, state[i].end()); // middle становится началом
    }
}


//функция для смешивания столбцов
void mixColumns(vector<vector<unsigned char>>& state) {
    // Проходим по каждому столбцу матрицы состояния (state)
    for (int i = 0; i < 4; ++i) {
        // Создаем временные массивы для хранения промежуточных значений
        unsigned char a[4]; // Для хранения значений текущего столбца
        unsigned char b[4]; // Для хранения преобразованных значений текущего столбца
        unsigned char h;    // Для хранения старшего бита значения байта

        // Заполняем массивы a и b и применяем необходимые преобразования
        for (int c = 0; c < 4; ++c) {
            // Копируем значения текущего столбца в массив a
            a[c] = state[c][i];

            // Вычисляем старший бит  значения байта и сохраняем его в h
            h = (unsigned char)((signed char)state[c][i] >> 7); //приводим state  сначала к signed, потом к unsigned

            // Применяем операцию смешивания (MixColumn), с учетом циклического сдвига и XOR с константой
            b[c] = state[c][i] << 1;   // Выполняем циклический сдвиг влево на 1 бит
            b[c] ^= 0x1B & h;          // Если старший бит (MSB) был установлен, выполняем XOR с константой 0x1B ( многочлен в конечном поле Галуа)
        }

        // Применяем операцию смешивания столбцов (MixColumn) для текущего столбца
        // Результаты комбинируем через операцию XOR и записываем обратно в столбец state
        state[0][i] = b[0] ^ a[1] ^ b[1] ^ a[2] ^ a[3];
        state[1][i] = a[0] ^ b[1] ^ a[2] ^ b[2] ^ a[3];
        state[2][i] = a[0] ^ a[1] ^ b[2] ^ a[3] ^ b[3];
        state[3][i] = a[0] ^ b[0] ^ a[1] ^ a[2] ^ b[3];
    }
}

//Функция для вывода матриц
void print_matrix_hex(const vector<vector<unsigned char>>& matrix) {
    // Проходим по каждой строке матрицы
    for (const auto& row : matrix) {
        // Выводим значения каждого элемента строки в шестнадцатеричном формате
        for (unsigned char value : row) {
            // Выводим значение элемента в формате двузначного шестнадцатеричного числа
            printf("%02X ", value);
        }
        // После окончания строки выводим символ перевода строки
        cout << "\n";
    }
    // После вывода всех строк добавляем два символа перевода строки для отделения вывода от другого текста
    cout << "\n\n";
}

//Функция для обратной перестановки с S-блоком
void invSubBytes(vector<vector<unsigned char>>& matrix) {
    // Проходим по каждому элементу матрицы
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            // Используем значение байта как индекс для обратной S-бокс (invsBox)
            matrix[i][j] = invsBox[matrix[i][j] / 16][matrix[i][j] % 16];
        }
    }
}

//функция для обратной перестановки с P-блоком
void invShiftRows(vector<vector<unsigned char>>& state) {
    // Проходим по каждой строке матрицы состояния, начиная со второй
    for (int i = 1; i < 4; ++i) {
        // Выполняем обратный циклический сдвиг вправо на i позиций для каждой строки
        rotate(state[i].rbegin(), state[i].rbegin() + i, state[i].rend());
    }
}

//функция для умножения в поле Галуа
unsigned char gmul(unsigned char a, unsigned char b) {
    unsigned char p = 0; // Результат умножения
    unsigned char counter; // Счетчик для выполнения операции умножения в цикле
    unsigned char highBit; //  для проверки старшего бита

    // Выполняем умножение в конечном поле Галуа
    for (counter = 0; counter < 8; counter++) {
        // Если младший бит b равен 1, выполняем операцию XOR между p и a
        if (b & 1) {
            p ^= a;
        }

        // Проверяем старший бит a
        highBit = (a & 0x80);

        // Сдвигаем a на один бит влево
        a <<= 1;

        // Если старший бит установлен, выполняем операцию XOR с 0x1b
        if (highBit) {
            a ^= 0x1b;
        }

        // Сдвигаем b на один бит вправо
        b >>= 1;
    }

    // Возвращаем результат умножения
    return p;
}

//функция для обратного смешивания столбцов
void invMixColumns(vector<vector<unsigned char>>& state) {
    for (int i = 0; i < 4; ++i) {
        unsigned char a[4]; //текущее значение
        unsigned char b[4]; //преобразованное

        // Копируем текущий столбец в массив a
        for (int c = 0; c < 4; ++c) {
            a[c] = state[c][i];
        }

        // Вычисляем новые значения столбца b
        b[0] = gmul(a[0], 0x0e) ^ gmul(a[1], 0x0b) ^ gmul(a[2], 0x0d) ^ gmul(a[3], 0x09);
        b[1] = gmul(a[0], 0x09) ^ gmul(a[1], 0x0e) ^ gmul(a[2], 0x0b) ^ gmul(a[3], 0x0d);
        b[2] = gmul(a[0], 0x0d) ^ gmul(a[1], 0x09) ^ gmul(a[2], 0x0e) ^ gmul(a[3], 0x0b);
        b[3] = gmul(a[0], 0x0b) ^ gmul(a[1], 0x0d) ^ gmul(a[2], 0x09) ^ gmul(a[3], 0x0e);

        // Заменяем значения столбца state новыми значениями из b
        for (int c = 0; c < 4; ++c) {
            state[c][i] = b[c];
        }
    }
}

//Функция для расшифрования (использование других функция для шифровки)
void encrypted() {
    vector<unsigned char> key; // Вектор для хранения ключа
    vector<unsigned char> message; // Вектор для хранения сообщения

    generate_random_key(key); // Генерация случайного ключа
    write_key_to_file(key, "key.txt"); // Запись ключа в файл

    init_str(message); // Инициализация сообщения

    vector<vector<unsigned char>> key_matrix;
    vector<vector<unsigned char>> message_matrix;

    init_matrix(key, key_matrix); // Преобразование ключа в матрицу
    init_matrix(message, message_matrix); // Преобразование сообщения в матрицу

    vector<vector<unsigned char>> key_exp = key_expansion(key_matrix, 0); // Расширение ключа

    cout << "Начальный ключ(1):\n";
    print_matrix_hex(key_matrix); // Вывод начального ключа в шестнадцатеричном формате


    //преобразования
    transpose_matrix(key_exp); // Транспонирование матрицы расширенного ключа
    transpose_matrix(key_matrix); // Транспонирование матрицы ключа
    transpose_matrix(message_matrix); // Транспонирование матрицы сообщения

    vector<vector<unsigned char>> result_matrix = xor_matrices(key_matrix, message_matrix); // Применение операции XOR между ключом и сообщением,дл

    for (int i = 1; i < 10; i++) {
        sub_bytes(result_matrix); // Применение операции SubBytes
        shiftRows(result_matrix); // Применение операции ShiftRows
        mixColumns(result_matrix); // Применение операции MixColumns

        cout << "State после раунда " << i << ":\n";
        print_matrix_hex(result_matrix); // Вывод промежуточного состояния после каждого раунда

        //Генерируем новый раундовйы ключ
        result_matrix = xor_matrices(result_matrix, key_exp); // Применение операции XOR между промежуточным состоянием и раундовым ключом
        transpose_matrix(key_exp); // Транспонирование матрицы раундового ключа

        key_exp = key_expansion(key_exp, i); // Расширение раундового ключа
        transpose_matrix(key_exp); // Транспонирование матрицы раундового ключа

        cout << "Раундовый ключ " << i + 1 << ":\n";
        print_matrix_hex(key_exp); // Вывод раундового ключа в шестнадцатеричном формате
    }

    sub_bytes(result_matrix); // Применение операции SubBytes
    shiftRows(result_matrix); // Применение операции ShiftRows

    result_matrix = xor_matrices(result_matrix, key_exp); // Применение операции XOR между промежуточным состоянием и последним раундовым ключом

    transpose_matrix(result_matrix); // Транспонирование заключительного состояния

    cout << "Заключительный State:\n";
    print_matrix_hex(result_matrix); // Вывод заключительного состояния в шестнадцатеричном формате

    vector<unsigned char> ciphertext; // зашифрвоанный текст

    for (const auto& row : result_matrix) {
        for (const unsigned char ch : row) {
            ciphertext.push_back(ch);
        }
    }

    ofstream outputFile("ciphertext.txt", ios::binary);
    if (outputFile.is_open()) {
        for (unsigned char ch : ciphertext) {
            outputFile.write(reinterpret_cast<const char*>(&ch), sizeof(ch));
        }
        outputFile.close();
        cout << "Зашифрованный текст успешно записан в файл ciphertext.txt" << endl;
    }
    else {
        cout << "Не удалось открыть файл для записи" << endl;
    }
}


void decrypt() {
    vector<unsigned char> key; // Вектор для хранения ключа
    vector<unsigned char> ciphertext; // Вектор для хранения зашифрованного текста

    // Чтение ключа из файла
    ifstream keyFile("key.txt", ios::binary);
    if (keyFile.is_open()) {
        unsigned char ch;
        while (keyFile.read(reinterpret_cast<char*>(&ch), sizeof(ch))) {
            key.push_back(ch);
        }
        keyFile.close();
    }else {
        cout << "Не удалось открыть файл с ключом" << endl;
        return;
    }

    // Чтение зашифрованного текста из файла
    ifstream inputFile("ciphertext.txt", ios::binary);
    if (inputFile.is_open()) {
        unsigned char ch;
        while (inputFile.read(reinterpret_cast<char*>(&ch), sizeof(ch))) {
            ciphertext.push_back(ch);
        }
        inputFile.close();
    }else {
        cout << "Не удалось открыть файл для чтения" << endl;
        return;
    }

    vector<vector<unsigned char>> key_matrix; // Матрица для ключа
    vector<vector<unsigned char>> ciphertext_matrix; // Матрица для зашифрованного текста

    init_matrix(key, key_matrix); // Преобразование ключа в матрицу
    init_matrix(ciphertext, ciphertext_matrix); // Преобразование зашифрованного текста в матрицу

    cout << "Зашифрованное сообщение (State):\n";
    print_matrix_hex(ciphertext_matrix); // Вывод зашифрованного текста в шестнадцатеричном формате

    vector<vector<vector<unsigned char>>> round_key_exp; // Вектор для хранения расширенных раундовых ключей
    vector<vector<unsigned char>> first_key_matrix = key_matrix;
    transpose_matrix(first_key_matrix);

    round_key_exp.push_back(first_key_matrix);
    transpose_matrix(first_key_matrix);

    // Создание и сохранение расширенных раундовых ключей
    for (int i = 0; i < 10; i++) {
        vector<vector<unsigned char>> round = key_expansion(first_key_matrix, i);
        transpose_matrix(round);
        round_key_exp.push_back(round);
        transpose_matrix(round);
        first_key_matrix = round;
    }

    transpose_matrix(ciphertext_matrix); // Транспонирование матрицы зашифрованного текста

    vector<vector<unsigned char>> result_matrix = xor_matrices(ciphertext_matrix, round_key_exp[10]); // Применение операции XOR к зашифрованному тексту и последнему раундовому ключу

        // Обратные шаги для расшифровки
    for (int i = 9; i > 0; i--) {
        invShiftRows(result_matrix); // Обратная операция ShiftRows
        invSubBytes(result_matrix); // Обратная операция SubBytes
        result_matrix = xor_matrices(result_matrix, round_key_exp[i]); // Применение операции XOR к промежуточному результату и текущему раундовому ключу
        invMixColumns(result_matrix); // Обратная операция MixColumns
    }

    invShiftRows(result_matrix); // Обратная операция ShiftRows
    invSubBytes(result_matrix); // Обратная операция SubBytes
    result_matrix = xor_matrices(result_matrix, round_key_exp[0]); // Применение операции XOR к промежуточному результату и первому раундовому ключу
    transpose_matrix(result_matrix); // Транспонирование результата

    vector<unsigned char> plaintext; // Вектор для расшифрованного текста
    for (const auto& row : result_matrix) {
        for (const unsigned char ch : row) {
            plaintext.push_back(ch);
        }
    }
       // Запись расшифрованного текста в файл
    ofstream outputFile("decrypted_message.txt", ios::binary);
    if (outputFile.is_open()) {
        for (unsigned char ch : plaintext) {
            outputFile.write(reinterpret_cast<const char*>(&ch), sizeof(ch));
        }
        outputFile.close();
        cout << "Расшифрованный текст успешно записан в файл decrypted_message.txt" << endl;
        cout << "Содержимое файла: " << endl;
        for (unsigned char ch : plaintext) {
            cout << ch;
        }
        cout << endl;
    }
    else {
        cout << "Не удалось открыть файл для записи" << endl;
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251); 
    setlocale(LC_ALL, "rus");

    encrypted();  
    decrypt();
}
