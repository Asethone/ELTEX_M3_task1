// Данная программа создает область разделяемой памяти (РП) и записывает туда собственный текст

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define CNT 3047                    // Число символов данного кода (получено с помощью команды wc -c)

int main() {
    key_t key;                      // IPC-ключ
    char pathname[] = "p4_2a.c";    // Строка, содержащая имя файла, по которому генерируется ключ
    int shmid;                      // IPC-дескриптор для области РП
    char *array;                    // Указатель на РП
    int fd;                         // Файловый дескриптор
    size_t size;                    // Количество считываемых байт (символов)

    key = ftok(pathname, 0);        // Генерация IPC-ключа
    if (key < 0) {                  // Если значение ключа -1, завершить работу с ошибкой
        printf("Error generating key!\n");
        exit(-1);
    }
    shmid = shmget(key, CNT, 0666 | IPC_CREAT | IPC_EXCL);   // Создание новой области разделяемой памяти
    if (shmid < 0) {                // Если значение IPC-дескриптора -1, завершить работу с ошибкой
        printf("Cant\'t create new shared memory! It may be already exist.\n");
        exit(-1);
    }
    array = shmat(shmid, NULL, 0);
    if (array == (char *)(-1)) {    // Если значение array -1, завершить с ошибкой
        printf("Can't add shared memory!\n");
        exit(-1);
    }

    fd = open("p4_2a.c", O_RDONLY); // Открытие текущего данного файла для чтения
    if (fd < 0) {                   // Если дескриптор файла равен -1, завершить работу с ошибкой
        printf("Error on open file!\n");
        exit(-1);
    }
    size = read(fd, array, CNT);    // Чтение из файла CNT байт и запись по адресу РП 
    if (size < 0) {               // Если количество считанных байт менее CNT, завершить работу с ошибкой
        printf("Error on reading file!\n");
        exit(-1);
    }
    if (close(fd) < 0) {            // Закрытие файла и ошибка в случае неудачи
        printf("Error on closing file!\n");
        exit(-1);
    }
    if (shmdt(array) < 0) {         // Открепление РП из адресного пространства процесса, ошибка в случае неудачи
        printf("Error on deattach shared memory!\n");
        exit(-1);
    }

    return 0;
}
