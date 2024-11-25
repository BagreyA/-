#include <iostream>
#include <fstream>
#include <cstring>

struct Train {
    char destination[50];
    int number;
    int departure_time[2];
};

// Функция для записи поездов в файл
void writeTrainsToFile() {
    std::ofstream ofs("Train.dat", std::ios::binary);
    if (!ofs) {
        std::cerr << "Ошибка при создании файла для записи." << std::endl;
        return;
    }

    Train trains[] = {
        {"Moscow", 101, {10, 30}},
        {"Saint Petersburg", 102, {12, 45}},
        {"Kazan", 103, {8, 0}},
        {"Sochi", 104, {16, 15}},
        {"Novosibirsk", 105, {9, 50}},
        {"Vladivostok", 106, {22, 30}},
        {"Ekaterinburg", 107, {14, 25}},
        {"Samara", 108, {18, 10}}
    };

    ofs.write(reinterpret_cast<char*>(trains), sizeof(trains));
    if (!ofs) {
        std::cerr << "Ошибка при записи в файл." << std::endl;
    }

    ofs.close();
    std::cout << "Данные записаны в файл Train.dat." << std::endl;
    std::cout << "\nАвтор программы: Багрей А.О., учетное имя: BagreyAO, группа: ИА-232" << std::endl;
}

int main() {
    writeTrainsToFile();
    return 0;
}
