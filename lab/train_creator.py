import struct
import re

class Train:
    def __init__(self, destination, train_number, departure_time):
        self.destination = destination
        self.train_number = train_number
        self.departure_time = departure_time

    def to_bytes(self):
        # Очистка строки назначения от некорректных символов
        destination_cleaned = re.sub(r'[^\w\sА-Яа-яЁё]', '?', self.destination)
        destination_bytes = destination_cleaned.encode('utf-8', errors='replace')
        return struct.pack('20sif', destination_bytes[:20], self.train_number, self.departure_time)

def convert_time_to_float(time_str):
    hours, minutes = map(int, time_str.split(':'))
    return hours + minutes / 60.0

def create_train_file(filename):
    try:
        with open(filename, 'wb') as file:
            for i in range(8):
                destination = input(f"Введите название пункта назначения для поезда {i + 1} (или 'exit' для выхода): ")
                if destination.lower() == 'exit':
                    print("Выход из программы.")
                    break
                
                train_number_str = input(f"Введите номер поезда {i + 1} (или 'exit' для выхода): ")
                if train_number_str.lower() == 'exit':
                    print("Выход из программы.")
                    break
                
                try:
                    train_number = int(train_number_str)
                except ValueError:
                    print("Ошибка: номер поезда должен быть целым числом. Попробуйте снова.")
                    continue  # Пропустить текущую итерацию цикла
                
                departure_time_str = input(f"Введите время отправления поезда {i + 1} (в формате HH:MM): ")
                departure_time = convert_time_to_float(departure_time_str)

                train = Train(destination, train_number, departure_time)
                file.write(train.to_bytes())
        print("Файл Train.dat успешно создан.")

    except Exception as e:
        print(f"Ошибка при работе с файлом: {e}")

    finally:
        print("Автор программы: Багрей А.О., Учетное имя: pylonge, Группа: Ваша ИА-232")

if __name__ == "__main__":
    create_train_file("Train.dat")
