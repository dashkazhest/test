#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <ctime>
#include <string>
#include <cstring>

using namespace std;

//Козловская Дарья Константиновна, 4 курс, ТУСУР, Информатика и вычислительная техника.
//C++ программа (сервер, создаёт сокет, слушает подключения, 
//принимает данные и отсылает клиенту свои, записывает логи).

int main() {

    //создание сокета
    int sock = socket (AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "socket error" << endl;
        return 1;
    }

    //установление адреса - путь к файлу
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "/tmp/unix_sock", sizeof(addr.sun_path)-1);

    unlink("/tmp/unix_sock"); // удаление сокета с прошлого запуска
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr))<0){ // привязка адреса
        cerr << "bind error" << endl;
        return 1;
    }

    //прослушивание подключений клиента и ожидание
    if(listen(sock,1) <0){ 
        cerr << "listen error" << endl;
        return 1;
    };

    int client = accept(sock, 0, 0);
    if (client < 0) {
        cerr << "accept error" << endl;
        return 1;
    }

    //принятие данных от программы-клиента
    char buf[64]; 
    memset(buf, 0, sizeof(buf));
    int message = recv(client, buf, sizeof(buf)-1, 0);
    if (message < 0) {
        cerr << "recv error" << endl;
        return 1;
    } 
    cout << buf << endl;

    //логирование с подписью даты и времени получения
    time_t now = time(0);
    char timebf[64];
    strftime(timebf, sizeof(timebf), "%d-%m-%Y %H:%M", localtime(&now));
    ofstream log("socket_server.log", ios::app);
    log << "data: " << buf << " [ date and time of receipt: " << timebf << "]"<< endl;
    log.close();

    // отправка данных программе-клиенту
    char text[64] = "SOCKET_SERVER_ON_CPP_HI";
    send(client, text, strlen(text), 0);

    close(client); // закрытие сокетов
    close(sock);
    return 0;
}
