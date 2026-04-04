#ifndef BANCO_H_INCLUDED
#define BANCO_H_INCLUDED

extern pthread_mutex_t mutex_banco;

typedef struct{
    int id;
    char nome[50];
}Registro;

void* insert_func(void* arg);
void* select_func(void* arg);
void* update_func(void* arg);
void* delete_func(void* arg);

#endif
