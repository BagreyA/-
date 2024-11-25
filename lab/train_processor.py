import struct
import os
import tempfile

class Train:
    def __init__(self, destination, train_number, departure_time):
        self.destination = destination
        self.train_number = train_number
        self.departure_time = departure_time

    @classmethod
    def from_bytes(cls, bytes_data):
        destination, train_number, departure_time = struct.unpack('20sif', bytes_data)
        return cls(destination.decode('utf-8').strip('\x00'), train_number, departure_time)

    def to_bytes(self):
        destination_bytes = self.destination.encode('utf-8')
        destination_bytes = destination_bytes[:20] + b'\x00' * (20 - len(destination_bytes))  # Паддинг до 20 байт
        return struct.pack('20sif', destination_bytes, self.train_number, self.departure_time)

def convert_time_to_float(time_str):
    hours, minutes = map(int, time_str.split(':'))
    return hours + minutes / 60.0

def float_to_time_str(float_time):
    hours = int(float_time)
    minutes = int((float_time - hours) * 60)
    return f"{hours:02}:{minutes:02}"

def create_sample_file(filename):
    try:
        with open(filename, 'wb') as file:
            trains = [
                ("Москва", 111, "10:10"),
                ("Омск", 222, "11:11"),
                ("Казань", 333, "12:12"),
                ("Сочи", 444, "13:13"),
                ("Владивосток", 555, "14:14"),
                ("Нижний Новгород", 666, "15:15"),
                ("Екатеринбург", 777, "16:16"),
                ("Ростов-на-Дону", 888, "17:17"),
            ]
            for destination, train_number, departure_time_str in trains:
                departure_time = convert_time_to_float(departure_time_str)
                train = Train(destination, train_number, departure_time)
                file.write(train.to_bytes())
        print("Файл Train.dat успешно создан с примерными данными.")

    except Exception as e:
        print(f"Ошибка при создании файла: {e}")

def process_train_file(filename, input_time):
    try:
        found_trains = False
        with open(filename, 'rb') as file:
            while True:
                bytes_data = file.read(struct.calcsize('20sif'))
                if not bytes_data:
                    break
                train = Train.from_bytes(bytes_data)

                if train.departure_time > input_time:
                    found_trains = True
                    departure_time_str = float_to_time_str(train.departure_time)
                    print(f"Поезд {train.train_number} направляется в {train.destination} в {departure_time_str}.")

        if not found_trains:
            print("Поездов, отправляющихся после указанного времени, не найдено.")

    except Exception as e:
        print(f"Ошибка при работе с файлом: {e}")

    finally:
        print("Автор программы: Багрей А.О., Учетное имя: pylonge, Группа: Ваша ИА-232")

if __name__ == "__main__":
    # Создаем временную директорию для хранения файла
    with tempfile.TemporaryDirectory() as temp_dir:
        filename = os.path.join(temp_dir, "Train.dat")
        
        # Проверяем, существует ли файл
        if not os.path.exists(filename):
            print("Файл Train.dat не найден. Создаю новый файл...")
            create_sample_file(filename)  # Если файл не существует, создаем его с примерными данными

        input_time_str = input("Введите время (в формате HH:MM), после которого необходимо искать поезда: ")
        try:
            input_time = convert_time_to_float(input_time_str)  # Преобразуем ввод в формат HH.MM
            process_train_file(filename, input_time)
        except ValueError:
            print("Ошибка: Время должно быть введено в формате HH:MM.")
