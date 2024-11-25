#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#ifdef __unix__
#include <sys/wait.h>
#endif

#define BUFFER_SIZE 1024
#define END_OF_TRANSMISSION 26

void find_occurrences(const char *text, const char *pattern, int *count, char *uppercase_words) {
    const char *pos = text;
    *count = 0;

    // Проходим по тексту и ищем вхождения
    while ((pos = strstr(pos, pattern)) != NULL) {
        (*count)++;

        const char *start = pos;
        while (start > text && *(start - 1) != ' ') start--;
        const char *end = pos + strlen(pattern);
        while (*end != ' ' && *end != '\0') end++;

        char word[BUFFER_SIZE];
        strncpy(word, start, end - start);
        word[end - start] = '\0';

        snprintf(uppercase_words + strlen(uppercase_words), BUFFER_SIZE, "%s, ", word);

        pos++;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *input_path = argv[1];
    const char *output_path = argv[2];

    int pipe_fd[2];
    pid_t pid;


    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Создание дочернего процесса
    if ((pid = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {  // Родительский процесс
        close(pipe_fd[0]);

        FILE *input_file = fopen(input_path, "r");
        if (input_file == NULL) {
            perror("Ошибка открытия входного файла");
            close(pipe_fd[1]);
            exit(EXIT_FAILURE);
        }

        char buffer[BUFFER_SIZE];
        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, input_file)) > 0) {
            write(pipe_fd[1], buffer, bytes_read);
        }

        char eof = END_OF_TRANSMISSION;
        write(pipe_fd[1], &eof, 1);

        fclose(input_file);
        close(pipe_fd[1]);
        wait(NULL);

    } else {  // Дочерний процесс
        close(pipe_fd[1]);

        char buffer[BUFFER_SIZE];
        char text[BUFFER_SIZE * 10] = "";
        size_t total_read = 0;
        char ch;

        while (read(pipe_fd[0], &ch, 1) > 0) {
            if (ch == END_OF_TRANSMISSION) break;
            buffer[total_read++] = ch;
        }
        buffer[total_read] = '\0';

        if (total_read >= 2) {
            char pattern[3] = {buffer[0], buffer[1], '\0'};
            int total_count = 0;
            char uppercase_words[BUFFER_SIZE] = "";

            find_occurrences(buffer, pattern, &total_count, uppercase_words);


            if (strlen(uppercase_words) > 0) {
                uppercase_words[strlen(uppercase_words) - 2] = '\0';
            }

            // Открываем файл для записи результатов
            FILE *output_file = fopen(output_path, "w");
            if (output_file == NULL) {
                perror("Ошибка открытия выходного файла");
                close(pipe_fd[0]);
                exit(EXIT_FAILURE);
            }

            // Формируем строки с результатами
            char result1[BUFFER_SIZE];
            char result2[BUFFER_SIZE];
            snprintf(result1, BUFFER_SIZE, "Общее количество вхождений: %d\n", total_count);
            snprintf(result2, BUFFER_SIZE, "Слова, в которых встречается сочетание: %s\n", uppercase_words);

            // Записываем результаты в файл
            fprintf(output_file, "%s", result1);
            fprintf(output_file, "%s", result2);
            fclose(output_file);

            // Дублируем вывод в терминал
            printf("%s", result1);
            printf("%s", result2);
        }

        close(pipe_fd[0]);
    }

    return 0;
}
