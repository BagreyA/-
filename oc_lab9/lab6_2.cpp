#include <iostream>
#include <fstream>
#include <cstring>

struct Train {
    char destination[50];
    int number;
    int departure_time[2];
};

bool isValidTime(int hours, int minutes) {
    return (hours >= 0 && hours <= 23) && (minutes >= 0 && minutes <= 59);
}

// Функция для проверки, находится ли время в пределах одного часа
bool isWithinOneHour(int train_hours, int train_minutes, int input_hours, int input_minutes) {
    int train_total_minutes = train_hours * 60 + train_minutes;
    int input_total_minutes = input_hours * 60 + input_minutes;
    
    return (train_total_minutes >= input_total_minutes) && (train_total_minutes <= input_total_minutes + 60);
}

// Функция для проверки, отбыли ли поезда
bool hasDeparted(int train_hours, int train_minutes, int input_hours, int input_minutes) {
    int train_total_minutes = train_hours * 60 + train_minutes;
    int input_total_minutes = input_hours * 60 + input_minutes;
    
    return train_total_minutes < input_total_minutes;
}

void displayAllTrains(int current_hours, int current_minutes) {
    std::ifstream ifs("Train.dat", std::ios::binary);
    if (!ifs) {
        std::cerr << "Ошибка при открытии файла для чтения.\n";
        return;
    }

    Train current_train;
    std::cout << "\nСписок всех поездов:\n";
    while (ifs.read(reinterpret_cast<char*>(&current_train), sizeof(Train))) {

        if (!hasDeparted(current_train.departure_time[0], current_train.departure_time[1], current_hours, current_minutes)) {
            std::cout << "Пункт назначения: " << current_train.destination 
                      << ", Номер поезда: " << current_train.number 
                      << ", Время отправления: " << current_train.departure_time[0] 
                      << ":" << (current_train.departure_time[1] < 10 ? "0" : "") << current_train.departure_time[1] 
                      << std::endl;
        }
    }
    ifs.close();
}

int main() {
    int input_hours, input_minutes;

    while (true) {
        std::cout << "Введите время (часы и минуты) для поиска поездов: ";
        std::cin >> input_hours >> input_minutes;

        if (!isValidTime(input_hours, input_minutes)) {
            std::cout << "Ошибка: время указано некорректно. Часы должны быть от 0 до 23, минуты от 0 до 59.\n";
            continue;
        }
        break;
    }

    int current_hours = input_hours;
    int current_minutes = input_minutes;

    displayAllTrains(current_hours, current_minutes);

    std::ifstream ifs("Train.dat", std::ios::binary);
    if (!ifs) {
        std::cerr << "Ошибка при открытии файла для чтения.\n";
        return 1;
    }

    Train current_train;
    bool found = false;
    std::cout << "\nПоезда, отправляющиеся в пределах одного часа от введенного времени:\n";
    while (ifs.read(reinterpret_cast<char*>(&current_train), sizeof(Train))) {
        if (hasDeparted(current_train.departure_time[0], current_train.departure_time[1], current_hours, current_minutes)) {
            continue;
        }
        if (isWithinOneHour(current_train.departure_time[0], current_train.departure_time[1], current_hours, current_minutes)) {
            std::cout << "Пункт назначения: " << current_train.destination 
                      << ", Номер поезда: " << current_train.number 
                      << ", Время отправления: " << current_train.departure_time[0] 
                      << ":" << (current_train.departure_time[1] < 10 ? "0" : "") << current_train.departure_time[1] 
                      << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "Поездов, отправляющихся в пределах одного часа от введенного времени, не найдено.\n";
    }
    std::cout << "\nАвтор программы: Багрей А.О., учетное имя: BagreyAO, группа: ИА-232" << std::endl;
    ifs.close();
    return 0;
}
