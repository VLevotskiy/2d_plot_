#include "simulation.h"

Simulation::Simulation(){
    Sim_dT = 0;
    Simulation_started = false;
}

void Simulation::AddMotor(const std::string& obj_name) {
    //проверяем нет ли уже моторов с таким именем
    if (motors.find(obj_name) == motors.end()){
        std::shared_ptr<Motor> sp1(new Motor);
        motors.insert(std::pair<std::string, std::shared_ptr<Motor>>(obj_name,sp1));
    }
    else {
        throw std::string("Name " + obj_name + " is already in use!");
    }
}

void Simulation::AddPen(const std::string& obj_name){
    if (pens.find(obj_name) == pens.end()){
        std::shared_ptr<Pen> sp1(new Pen);
        pens.insert(std::pair<std::string, std::shared_ptr<Pen>>(obj_name,sp1));
    }
    else {
        throw std::string("Name " + obj_name + " is already in use!");
    }
}

void Simulation::Stop(){
    Simulation_started = false;
}


void Simulation::Set_Sim_dT(const float val) {
    Sim_dT = val;
}

void Simulation::Set_Log_dT(const float val) {
    logger.Set_dT(val);
}

void Simulation::Set_motor_param(std::string& motor_name,char param, float value){
    auto motor_it = motors.find(motor_name);
    if (motor_it == motors.end()) {
        throw std::string("Motor with name " +  motor_name + "  not found\n");
        return;
    }

    switch(param){
    case 'S': {motor_it->second->Set_S_max_aups(value); break;}
    case 'P': {motor_it->second->Set_TP(value);         break;}
    case 'A': {motor_it->second->Set_A_aups(value);     break;}
    default : {throw std::string("Unknow motor param " + param); break;}
    }

}

void Simulation::Toggle_pen(std::string& pen_name,bool state){

    auto pen_it = pens.find(pen_name);
    if (pen_it == pens.end()) { throw std::string("Pen with name " + pen_name + "  not found\n"); return;}
    pen_it->second->Set_Toggle(state);
    std::cout << pen_it->second->isToggled() << std::endl;
}


float Simulation::Get_Sim_dT() const {
    return Sim_dT;
}

bool Simulation::isStarted() const {
    return Simulation_started;
}

void Simulation::Attatch(std::string& motor_name, std::string& pen_name,std::string& axis) {
    auto pen_it = pens.find(pen_name);
    if (pen_it == pens.end()) {
        throw std::string("Pen with name " + pen_name +" not found\n");
        return;
    }

    auto motor_it = motors.find(motor_name);
    if (motor_it == motors.end()) {
        throw std::string("Motor with name " + motor_name + " not found\n");
        return;
    }

    auto temp_pen_ptr = pen_it->second;
    if (axis == "X" || axis == "x") {
        if (!temp_pen_ptr->Attatch_Motor_x(motor_it->second)){
            throw std::string("Can't attatch motor " + motor_name + " to pen " + pen_name + "with axis " + axis + ". Motor alredy attatched");
            return;
        }
    }
    else if (axis == "y" || axis == "Y") {
        if (!temp_pen_ptr->Attatch_Motor_y(motor_it->second)){
            throw std::string("Can't attatch motor " + motor_name + " to pen " + pen_name + "with axis " + axis + ". Motor alredy attatched\n");
            return;
        }
    }
    else {
        throw std::string("Unknow axis " + axis);
        return;
    }
}

void Simulation::Start() {
    Simulation_started = true;

    //добавляем перья для логгирования
    for (auto i = pens.begin(); i != pens.end(); ++i){
        logger.Add_Pen(i->first,i->second);
    }
    logger.Set_Start_time(clock());

    //запуск логгирования в отдельном потоке
    std::thread log_thread(&Logger::Start, std::ref(logger));
    log_thread.detach();
    //симуляция. Переменная Simulation_started сбрасывается вызовов методом Simulation::Stop() из родительского потока
    while (Simulation_started != false) {
        clock_t end_time = clock() + Sim_dT * CLOCKS_PER_SEC ;  // время завершения
        while (clock() < end_time);
        for (auto i = motors.begin(); i != motors.end(); ++i) {
            i->second->Update_velocity(Sim_dT);
            i->second->Update_position(Sim_dT);
        }
    }
    logger.Stop();
}
