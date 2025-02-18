#include "function.cpp"

// Функция для выполнения одного блока AES
string DeblockAES(string text, string key, bool crypt, vector<int> initialization) {
    int rounds = 10;
    vector<string> keys = KEY(key, rounds);
    if (crypt == true) {
        cout << endl << "Зашифрованное:" << endl << endl;
    } else {
        cout << endl << "Расшифровка:" << endl << endl;
    }
    cout << "Раунд 1:" << endl << "AddRoundKey: ";
    string retention = AddRoundKey(text, key);
    cout << retention << endl << endl;
    for (int i = 1; i < rounds - 1; i++) {
        cout << "Раунд " << i + 1 << ":" << endl;
        if (crypt == true) {
            cout << "SubBytes: ";
            retention = SubBytes(retention);
            cout << retention << endl;
            cout << "ShiftRows: ";
            retention = ShiftRows(retention, crypt);
            cout << retention << endl;
            cout << "MixColumns: ";
            retention = MixColumns(retention, initialization);
            cout << retention << endl;
            cout << "AddRoundKey: ";
            retention = AddRoundKey(retention, keys[i]);
            cout << retention << endl << endl;
        } else {
            cout << "InvShiftRows: ";
            retention = ShiftRows(retention, crypt);
            cout << retention << endl;
            cout << "InvSubBytes: ";
            retention = SubBytes(retention);
            cout << retention << endl;
            cout << "AddRoundKey: ";
            retention = AddRoundKey(retention, keys[i]);
            cout << retention << endl << endl;
            cout << "InvMixColumns: ";
            retention = MixColumns(retention, initialization);
            cout << retention << endl;
        }
    }
    cout << "Последний раунд:" << endl;
    if (crypt == true) {
        cout << "SubBytes: ";
        retention = SubBytes(retention);
        cout << retention << endl;
        cout << "ShiftRows: ";
        retention = ShiftRows(retention, crypt);
    } else {
        cout << "InvShiftRows: ";
        retention = ShiftRows(retention, crypt);
        cout << retention << endl;
        cout << "InvSubBytes: ";
        retention = SubBytes(retention);
    }
    cout << retention << endl;
    cout << "AddRoundKey: ";
    retention = AddRoundKey(retention, keys[rounds - 1]);
    cout << retention << endl;
    cout << "Финальный результат: " << retention << endl;
    return retention;
}

string EncryptDecipherCFB(string text, string key, bool crypt, vector<int> initialization) {
    string result;
    string iv = key;

    for (int i = 0; i < text.size(); i += 16) {
        string block = text.substr(i, 16);
        string cipher = DeblockAES(iv, key, true, initialization);
        string outputBlock = AddRoundKey(block, cipher);
        result += outputBlock;
        iv = crypt ? outputBlock : block;  // Правильное обновление IV при расшифровке
    }

    return result;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    string text = "Определите и инициализируйте матрицу размера M х N случайными целыми числами в диапазоне [10, 50]. Поменяйте в каждой строке местами максимальный и минимальный элементы. Определите и инициализируйте новую матрицу, первая строка которой содержит минимальные элементы, а вторая строка – максимальные.";
    while (text.size() % 16 != 0) text += ' ';

    random_device ran;
    uniform_int_distribution<> dist(0, 255);
    string key(16, '\0');
    for (char& c : key) c = dist(ran);

    vector<int> initialization{3, 2, 4, 5};

    string encryptedText = EncryptDecipherCFB(text, key, true, initialization);
    cout << "Зашифрованный текст: " << encryptedText << endl;

    string decryptedText = EncryptDecipherCFB(encryptedText, key, false, initialization);
    while (!decryptedText.empty() && decryptedText.back() == ' ') decryptedText.pop_back();

    cout << "Расшифрованный текст: " << decryptedText << endl;
    return 0;
}
