#include "lab10.h"

// Функция для вывода первых 10 переменных окружения способом 1 (через массив env)
void print_env_var_method1(char **env) {
    for (int i = 0; i < 10 && env[i] != NULL; i++) {
        printf("%s\n\n", env[i]);
    }
}

// Функция для вывода первых 10 переменных окружения способом 2 (через getenv)
void print_env_var_method2() {
    const char *env_vars[] = {"PATH", "HOME", "SHELL", "USER", "LANG", "LOGNAME", "PWD", "TERM", "HOSTNAME", "MAIL"};
    for (int i = 0; i < 10; i++) {
        const char *value = getenv(env_vars[i]);
        if (value) {
            printf("%s=%s\n\n", env_vars[i], value);
        }
    }
}

// Функция для чтения и вывода содержимого файла
void print_file_content(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Ошибка открытия файла '%s': %s\n", filename, strerror(errno));
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }

    if (ferror(file)) {
        fprintf(stderr, "Ошибка при чтении файла '%s'\n", filename);
    }

    if (fclose(file) != 0) {
        fprintf(stderr, "Ошибка закрытия файла '%s': %s\n", filename, strerror(errno));
    }
}

// Функция для вывода справки о правильном использовании программы
void print_usage(const char *progname) {
    fprintf(stderr, "Использование: %s [-1 | -2] [-f файл]\n", progname);
    fprintf(stderr, " -1        вывести первые 10 переменных окружения способом 1\n");
    fprintf(stderr, " -2        вывести первые 10 переменных окружения способом 2\n");
    fprintf(stderr, " -f файл   вывести содержимое указанного файла\n");
}

// Главная функция программы
int main(int argc, char *argv[], char *envp[]) {
    int option = 0;
    int method = 0;  // 1 для способа 1, 2 для способа 2
    char *filename = NULL;

    while ((option = getopt(argc, argv, "12f:")) != -1) {
        switch (option) {
            case '1':
                method = 1;
                break;
            case '2':
                method = 2;
                break;
            case 'f':
                filename = optarg;
                break;
            default:
                print_usage(argv[0]);
                return EXIT_FAILURE;
        }
    }

    if (method == 0) {
        fprintf(stderr, "Ошибка: необходимо выбрать -1 или -2 для выбора способа вывода переменных окружения\n");
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    // Вывод первых 10 переменных окружения
    if (method == 1) {
        print_env_var_method1(envp);
    } else if (method == 2) {
        print_env_var_method2();
    }

    // Вывод содержимого файла, если указан флаг -f
    if (filename) {
        print_file_content(filename);
    }

    // Вывод информации об авторе
    printf("\n%s\n", AUTHOR);
    printf("Идентификатор пользователя: %d\n", getuid());

    return EXIT_SUCCESS;
}
