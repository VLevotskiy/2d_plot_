#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include "pen.h"

//Класс logger предназначен для записи координат перьев в файлы
class Logger {
private:
    float Log_dT;   //интервал записей
    std::vector<std::shared_ptr<Pen>> pens_ptrs;    //указатели на перья, для которых необходимо вести запись
    std::vector<std::ofstream> logs;                //дескрипторы файлов
    std::vector<bool> log_state;                    //состаяние записи для перьев (писать/не писать)
    bool started;                                   //запущенна ли запись
    clock_t start_time;

public:
    Logger();
    void Add_Pen(std::string pen_name, std::shared_ptr<Pen>&);  //добавить перо
    void Set_dT(float value);                                   //установить интервал записи
    void Set_Start_time(clock_t);                               //установить время начала записи
    void Start();                                               //включить запись
    void Stop();                                                //остановить запись
    void Close_all();                                           //закрыть все дескрипторы файлов
    ~Logger();

};

#endif // LOGGER_H
