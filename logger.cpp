#include "logger.h"

Logger::Logger() {
    Log_dT = 0;
    started = false;
}

void Logger::Add_Pen(std::string pen_name,std::shared_ptr<Pen>& new_pen) {
    std::string file_name = pen_name + ".log";
    pens_ptrs.push_back(new_pen);
    logs.emplace_back(std::make_shared<std::ofstream>(std::ofstream(file_name)));
    log_state.push_back(new_pen->isToggled());
}

void Logger::Set_dT(const float value){
    Log_dT = value;
}

void Logger::Start(){
    if (started) return;
    started = true;

    while (started != false) {
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(Log_dT * CLOCKS_PER_SEC)));
        double sim_time = (clock() - start_time)/(double)(CLOCKS_PER_SEC);

        //записываем координаты каждого пера в соответствующий ему файл
        for (size_t i = 0; i < pens_ptrs.size(); ++i) {
            if (pens_ptrs.at(i)->isToggled()){
                *(logs.at(i)) << sim_time << "; ";
                *(logs.at(i)) << pens_ptrs.at(i)->Get_pos_x() << "; ";
                *(logs.at(i)) << pens_ptrs.at(i)->Get_pos_y() << "\n";
                //logs.at(i)->operator<< ("\n");
                log_state.at(i) = true;
            }
            else {
                if (log_state.at(i)){
                    *(logs.at(i)) << sim_time << "; ";
                    *(logs.at(i)) << "-;-\n";
                    log_state.at(i) = false;
                }
            }
        }
    }

    Close_all();
}

void Logger::Stop() {
    started = false;
}

void Logger::Close_all(){
    for(size_t i = 0; i < logs.size(); ++i){
        if (logs.at(i)->is_open()){
            logs.at(i)->close();
        }
    }
}

void  Logger::Set_Start_time(clock_t time) {
    start_time = time;
}

Logger::~Logger(){
    Close_all();
}
