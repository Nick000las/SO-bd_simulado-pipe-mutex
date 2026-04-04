#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

int main(){
    int fd = open("pipe_db", O_WRONLY);
    if(fd < 0){
        cout<<"Erro ao abrir pipe"<<endl;
        return 1;
    }

    string msg = "INSERT 1 Joao";
    while(true){
        cout<<"Digite o Query: ";
        getline(cin, msg);

        write(fd, msg.c_str(), msg.size());

        if(msg == "EXIT"){
            break;
        }
    }
    return 0;
}
