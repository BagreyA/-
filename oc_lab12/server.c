#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "header.h"

void process_file(const char *filename, const char *output_filename) {
    FILE *input_file = fopen(filename, "r");
    if (!input_file) {
        perror("Ошибка открытия входного файла");
        return;
    }

    FILE *output_file = fopen(output_filename, "w");
    if (!output_file) {
        perror("Ошибка открытия выходного файла");
        fclose(input_file);
        return;
    }

    char line[BUFFER_SIZE];
    while (fgets(line, sizeof(line), input_file)) {
        fputs(line, output_file);
    }

    fclose(input_file);
    fclose(output_file);
}

int main() {
    mkfifo(FIFO_NAME, 0666); // Создаем именованный канал

    char buffer[BUFFER_SIZE];
    char filename[BUFFER_SIZE];

    printf("Сервер запущен.\n");

    while (1) {
        // Ожидаем получения строки от клиента
        int fd_read = open(FIFO_NAME, O_RDONLY);
        read(fd_read, buffer, sizeof(buffer));
        close(fd_read);

        // Проверяем, начинается ли строка с префикса
        if (strncmp(buffer, FILENAME_PREFIX, strlen(FILENAME_PREFIX)) == 0) {
            // Получаем имя файла
            sscanf(buffer + strlen(FILENAME_PREFIX), "%s", filename);
            printf("Получено имя файла: %s\n", filename);

            // Обработка файла
            char output_filename[BUFFER_SIZE];
            snprintf(output_filename, sizeof(output_filename), "output_%s", filename);
            process_file(filename, output_filename);

            // Отправляем имя выходного файла обратно клиенту
            int fd_write = open(FIFO_NAME, O_WRONLY);
            write(fd_write, output_filename, strlen(output_filename) + 1);
            close(fd_write);
        }
    }

    return 0;
}
