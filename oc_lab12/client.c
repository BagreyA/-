#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "header.h"

int main() {
    char filename[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];

    printf("Клиент запущен.\n");
    printf("Введите имя файла (например, input.txt): ");
    fgets(filename, sizeof(filename), stdin);

    // Убираем символ новой строки
    filename[strcspn(filename, "\n")] = '\0'; 

    // Формируем строку формата FILENAME~имя_файла
    char request[BUFFER_SIZE];
    snprintf(request, sizeof(request), "%s%s", FILENAME_PREFIX, filename);

    // Отправляем запрос серверу
    int fd_write = open(FIFO_NAME, O_WRONLY);
    write(fd_write, request, strlen(request) + 1);
    close(fd_write);

    // Ожидаем имя выходного файла
    int fd_read = open(FIFO_NAME, O_RDONLY);
    read(fd_read, buffer, sizeof(buffer));
    close(fd_read);

    printf("Сервер отправил имя выходного файла: %s\n", buffer);

    // Читаем выходной файл и выводим на экран
    FILE *output_file = fopen(buffer, "r");
    if (output_file == NULL) {
        perror("Ошибка открытия выходного файла");
        return 1;
    }

    char line[BUFFER_SIZE];
    printf("Содержимое выходного файла:\n");
    while (fgets(line, sizeof(line), output_file) != NULL) {
        printf("%s", line);
    }

    fclose(output_file);
    return 0;
}
