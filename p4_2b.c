// Данная программа осуществляет чтение данных из созданной первой программой области разделяемой памяти и выводит эти данные на экран

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

#define CNT 3047                    // Число символов кода первой программы

int main() {
    key_t key;                      // IPC-ключ
    char pathname[] = "p4_2a.c";    // Строка, содержащая имя файла, по которому генерируется ключ
    int shmid;                      // IPC-дескриптор для области РП
    char *array;                    // Указатель на РП

    key = ftok(pathname, 0);        // Генерация IPC-ключа
    if (key < 0) {                  // Если значение ключа -1, завершить работу с ошибкой
        printf("Error generating key!\n");
        exit(-1);
    }
    shmid = shmget(key, CNT, 0);    // Создание новой области разделяемой памяти
    if (shmid < 0) {                // Если значение IPC-дескриптора -1, завершить работу с ошибкой
        printf("Cant\'t get shared memory! It may be not created yet.\n");
        exit(-1);
    }
    array = shmat(shmid, NULL, 0);
    if (array == (char *)(-1)) {    // Если значение array -1, завершить с ошибкой
        printf("Can't add shared memory!\n");
        exit(-1);
    }
    for (int i = 0; i < CNT; i++) { // Посимвольный вывод содержимого РП
        printf("%c", array[i]);
    }
    if (shmdt(array) < 0) {         // Открепление РП из адресного пространства процесса, ошибка в случае неудачи
        printf("Error on deattach shared memory!\n");
        exit(-1);
    }
    if (shmctl(shmid, IPC_RMID, NULL) < 0) {    // Удаление РП из системы
        printf("Error on delete shared memory!\n");
        exit(-1);
    }
    return 0;
}