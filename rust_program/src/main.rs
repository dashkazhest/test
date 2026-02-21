use std::os::unix::net::UnixStream;
use std::io::{Read, Write};
use std::fs::OpenOptions;
use std::io::BufWriter;
use chrono::Local;

//Козловская Дарья Константиновна, 4 курс, ТУСУР, Информатика и вычислительная техника.
//Rust программа (клиент, подключается к сокету, отсылает и принимает данные, записывает их в лог-файл.)

fn main() {

    //подключение к сокету 
    let mut stream = UnixStream::connect("/tmp/unix_sock").expect("connect error");

    //отправка данных
    stream.write_all(b"SOCKET_CLIENT_ON_RUST_HI").expect("send error");

    //получение данных от программы-сервера
    let mut buf = [0u8; 64];
    let message = stream.read(&mut buf).expect("recv error");
    let text = String::from_utf8_lossy(&buf[..message]);

    // получение текущей даты и времени
    let now = Local::now().format("%d-%m-%Y %H:%M").to_string();

    //открытите лог-файла и последующая запись с временем
    let log_file = OpenOptions::new()
        .create(true)
        .append(true)
        .open("socket_client.log")
        .expect("log error");

    let mut writer = BufWriter::new(log_file);
    writeln!(writer, "data: {} [ date and time: {} ]", text, now).expect("write error");
    println!("{}", text);

}