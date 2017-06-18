#ifndef SIMULATION_H
#define SIMULATION_H
#include <map>
#include <ctime>
#include <iostream>
#include <thread>
#include "motor.h"
#include "pen.h"
#include "logger.h"

class Simulation {
private:
    bool Simulation_started;                                //запущена ли симуляция
    std::map<std::string, std::shared_ptr<Motor>> motors;   //список моторов
    std::map<std::string, std::shared_ptr<Pen>> pens;       //список перьев
    float Sim_dT;                                           //интервал симуляции
    Logger logger;                                          //логирование координат перьев
public:
    Simulation();
    void AddMotor(const std::string& motor_name);                      //добавить мотор
    void AddPen(const std::string& pen_name);                           //добавить перо
    void Attatch(std::string& motor_name,std::string& pen_name,std::string& axis);   //прикрепить моторк к оси пера
    void Start();                                           //заупстить симуляцию
    void Stop();                                            //остановить симуляцию

    void Set_Sim_dT(const float);
    void Set_Log_dT(const float);
    void Set_motor_param(std::string& motor_name, char param, float val);   //Установка параметра param мотора motor_name в значение val
    void Toggle_pen(std::string&,bool);

    float Get_Sim_dT() const;
    float Get_Log_dT() const;
    bool isStarted() const;
};

#endif // SIMULATION_H
