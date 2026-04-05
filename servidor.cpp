#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sstream>
#include <fstream>
#include <cstring>

#include "banco.h"

using namespace std;

int main(){

pthread_mutex_init(&mutex_banco, NULL);

string operacao;
int id;
string nome;

mkfifo("pipe_db", 0666);

int fd = open("pipe_db", O_RDONLY);
if(fd < 0){
    cout << "Erro ao abrir o pipe" << endl;
    return 1;
}

char buffer[100];
while(true){

    int n = read(fd, buffer, sizeof(buffer)-1);
    buffer[n] = '\0';
    cout<<"Recebido: "<<buffer<<endl;

    Registro* reg = new Registro;
    stringstream ss(buffer);
    ss >> operacao >> id >> nome;
    reg->id = id;
    strcpy(reg->nome, nome.c_str());

    pthread_t thread;


    if(operacao == "INSERT"){
        pthread_create(&thread, NULL, insert_func, reg);
        pthread_detach(thread);
    } else if(operacao == "SELECT"){
        pthread_create(&thread, NULL, select_func, reg);
        pthread_detach(thread);
    } else if(operacao == "UPDATE"){
        pthread_create(&thread, NULL, update_func, reg);
        pthread_detach(thread);
    }else if(operacao == "DELETE"){
        pthread_create(&thread, NULL, delete_func, reg);
        pthread_detach(thread);
    }else if(operacao == "EXIT"){
        cout<<"Saindo..."<<endl;
        delete reg;
        break;
    }else{
        cout<<"Operacao invalida!"<<endl;
        delete reg;
    }

}

close(fd);
pthread_mutex_destroy(&mutex_banco);

return 0;
}
