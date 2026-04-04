#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstring>

#include "banco.h"

using namespace std;

pthread_mutex_t mutex_banco;

void* insert_func(void* arg){
    ofstream file("banco.txt", ios::app);
    if (!file.is_open()){
        cout<<"Erro com o arquivo"<<endl;
        return NULL;
    }
    Registro* reg = (Registro*) arg;
    pthread_mutex_lock(&mutex_banco);
    file << reg->id << " " << reg->nome<< endl;
    file.close();
    cout << "Inserido: " << reg->id << " " << reg->nome << endl;
    pthread_mutex_unlock(&mutex_banco);
    delete reg;
    return NULL;
}


void* select_func(void* arg){
    ifstream file("banco.txt");
    if (!file.is_open()){
        cout<<"Erro com o arquivo"<<endl;
        return NULL;
    }
    Registro* reg = (Registro*) arg;
    pthread_mutex_lock(&mutex_banco);
    int id;
    string nome;
    while(file >> id >> nome){
        if(id == reg->id){
            cout<<"Encontrado: "<<id<<" "<<nome<<endl;
            file.close();
            pthread_mutex_unlock(&mutex_banco);
            return NULL;
        }
    }

    cout<<"Registro nao encontrado"<<endl;
    file.close();
    pthread_mutex_unlock(&mutex_banco);
    delete reg;
    return NULL;
}


void* update_func(void* arg){
    ifstream file("banco.txt");
    if (!file.is_open()){
        cout<<"Erro com o arquivo"<<endl;
        return NULL;
    }

    Registro* reg = (Registro*) arg;
    pthread_mutex_lock(&mutex_banco);

    vector<Registro> dados;
    Registro r;
    bool encontrado = false;
    while(file >> r.id >> r.nome){
        dados.push_back(r);
    }
    file.close();
    for(int i=0; i < dados.size(); i++){
        if(dados[i].id == reg->id){
            strcpy(dados[i].nome, reg->nome);
            encontrado = true;
            break;
        }
    }

    if (encontrado){
        ofstream file2("banco.txt");

            if (!file2.is_open()){
                cout<<"Erro com o arquivo"<<endl;
                return NULL;
            }

        for(int i=0; i<dados.size(); i++){
            file2 << dados[i].id << " " << dados[i].nome << endl;
        }
        file2.close();
        cout<< "Atualizado: "<<reg->id<<" "<<reg->nome<<endl;
    }else{
        cout<<"Registro nao encontrado"<<endl;
    }

    pthread_mutex_unlock(&mutex_banco);
    delete reg;
    return NULL;
}


void* delete_func(void* arg){
    ifstream file("banco.txt");
    if (!file.is_open()){
        cout<<"Erro com o arquivo"<<endl;
        return NULL;
    }
    Registro* reg = (Registro*) arg;
    pthread_mutex_lock(&mutex_banco);
    vector<Registro> dados;
    Registro r;
    bool encontrado = false;
    while(file >> r.id >> r.nome){
        if(r->id != reg->id){
            dados.push_back(r);
        }else{
            encontrado = true;
        }
    }
    file.close();
    if(encontrado){
        ofstream file2("banco.txt");

        if (!file2.is_open()){
            cout<<"Erro com o arquivo"<<endl;
            return NULL;
        }

        for(int i=0; i < dados.size(); i++){
            file2 << dados[i].id << " " << dados[i].nome << endl;
        }
        file2.close();
        cout<<"Deletado: "<<reg->id<<endl;
    }else{
        cout<<"Registro nao encontrado"<<endl;
    }

    pthread_mutex_unlock(&mutex_banco);
    delete reg;
    return NULL;
}
