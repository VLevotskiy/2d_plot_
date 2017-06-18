#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include "motor.h"
#include "pen.h"
#include "simulation.h"
//режимы работы
enum WORK_MODE { CONF, SIM, FIN } mode;

const int NUM_OF_COMMANDS_1 = 5;
const int NUM_OF_COMMANDS_2 = 4;
const int NUM_OF_SET_COMMANDS = 4;
const int NUM_OF_CREATE_COMMANDS = 2;

// Возможные команды
const std::string commands_list_1[NUM_OF_COMMANDS_1] = {"create", "attatch", "set", "start", "stop"};
const std::string commands_list_2[NUM_OF_COMMANDS_2] = {"motor", "pen", "sim", "log"};
const std::string create_commands_list[NUM_OF_CREATE_COMMANDS] = {"motor", "pen"};
const std::string set_commands_list[NUM_OF_SET_COMMANDS] = {"motor", "pen","sim","log"};

//Разбор строки команды(str) на отдельные слова, разделяемые набором символов delim. Результат записывается в вектор arr.
//Возвращает ссылку на вектор со словами
std::vector<std::string>& Get_words(std::string& str, std::vector<std::string>& arr,std::string delim);
//Сравнение строки input  с набором заданных комманд commands.
//В случае совпадения возвращает порядковый номер команды в массиве commands. Иначе возвращает -1
int8_t Parser(std::string& input, const std::string *commands, uint8_t num_of_commands);

int main()
{
    mode = CONF;
    Simulation simulation;

    std::string command;        //строка для считывания команды
    std::vector<std::string> words;     //вектора для слов, составляющих строку
    std::thread* sim_thread = nullptr;  //указатель на поток для симуляции

    while (mode != FIN) {
        command.clear();
        words.clear();
        //считывание и разбор строки
        getline(std::cin, command);
        Get_words(command,words," ");
        size_t current_word = 0;
        size_t num_of_words = words.size(); //кол-во слов в строке

        //Сравнение первого слова введенной команды с возможными в программе командами
        int command_num = Parser(words[current_word++], commands_list_1, NUM_OF_COMMANDS_1);
        switch(command_num){
        //Обработка команды create
        case 0:
        {
            if (mode == SIM) { std::cerr << "Simulation is running\n!"; break;}
            if ( num_of_words != 3){
                std::cerr << "Invalid command format. Command format: \"create <object_type> <object_name>\" \n";
                break;
            }

            int command_num = Parser(words[current_word++], create_commands_list, NUM_OF_CREATE_COMMANDS);
            std::string obj_name = words[current_word];
            try{
                switch(command_num){
                case 0: {
                    /*создание мотора*/
                    simulation.AddMotor(obj_name);
                    break;
                }
                case 1: {
                    /*создание пера*/
                    simulation.AddPen(obj_name);
                    break;
                }
                default: std::cerr << "Unknown type of object" << std::endl; break;
                }
            } catch (std::string& str) {
                std::cerr << str << std::endl;
            }

            break;
        }
        //Обработка команды attatch
        case 1:
        {
            if (mode == SIM) { std::cerr << "Simulation is running!\n"; break;}
            if ( num_of_words != 6){
                std::cerr << "Invalid command format. Command format: \"attatch <motor_name> with <axis> of <pen_name>\" \n" ;
                break;
            }
            if (words[2] != "with" && words[4] != "of"){
                std::cerr << "Invalid command format(with-of). Command format: \"attatch <motor_name> with <axis> of <pen_name>\" \n";
                break;
            }

            try {
                simulation.Attatch(words[1],words[5],words[3]);
            } catch(std::string& str) {
                std::cerr << str << std::endl;
            }

            break;
        }
        //Обработка команды Set
        case 2: {
            int command_num = Parser(words[1], set_commands_list, NUM_OF_SET_COMMANDS);
            switch(command_num) {
            //motor
            case 0:{
                if (num_of_words != 4){
                    std::cerr << "Invalid command format. Command format: \"Set motor <motor_name> <Param>=<Value> \n";
                    break;
                }
                std::string motor_name = words[2];
                std::string to_float = words[3].substr(2);
                float value = std::stof(to_float);
                if ((mode == CONF && words[3][0] != 'P') || (words[3][0] == 'P' && mode ==SIM)){
                    simulation.Set_motor_param(motor_name,words[3][0],value);
                }
                else {
                     std::cerr << "This command can't be running in current mode" << std::endl; break;
                }

                break;
            }
            //pen
            case 1: {
                if (num_of_words != 4){
                    std::cerr << "Invalid command format. Command format: \"Set pen <pen_name> on|off\n";
                    break;
                }
                std::string pen_name = words[2];
                bool state = (words[3].compare("on") == 0);
                simulation.Toggle_pen(pen_name,state);
                break;
            }
            //sim
            case 2:{
                if (num_of_words != 3){
                    std::cerr << "Invalid command format. Command format: \"Set sim dT=<Value> \n";
                    break;
                }
                // так как 3ий параметр команды dT=<value> не имеет пробелов, отделяем цифры от буквы
                std::string to_float = words[2].substr(3);
                //приводим результат к типу float
                float value = std::stof(to_float);
                if (value <= 0) {
                    std::cerr << "Error! Simulation time <= 0\n";
                    break;
                }
                simulation.Set_Sim_dT(value);
                break;
            }
            //log
            case 3:{
                if (num_of_words != 3){
                    std::cerr << "Invalid command format. Command format: \"Set log dT=<Value> \n";
                    break;
                }
                std::string to_float = words[2].substr(3);
                float value = std::stof(to_float);
                if (value <= 0) {
                    std::cerr << "Error! Logging time <= 0\n";
                    break;
                }
                simulation.Set_Log_dT(value);
                break;
            }
            default: {std::cerr << "Unknow parameter of command set: " << words[1] << "\n"; break;}
            }

            break;
        }
        //команда start
        case 3: { if (mode == SIM) { std::cerr << "Simulation alredy running!" << std::endl; break;}
            if (simulation.Get_Sim_dT() == 0) { std::cerr << "Simulation time not set\n"; break;}
            mode = SIM;
            //создаем отдеьный поток для работы симуляции
            sim_thread = new std::thread(&Simulation::Start,std::ref(simulation));
            sim_thread->detach();
            break;
        }
        //команда stop
        case 4: { if (mode != SIM) { std::cerr << "Simulation isn't running!" << std::endl; break;}
            mode = FIN;
            simulation.Stop();
            break;
        }
        default:{ std::cerr << "Unknown command" <<std::endl; break; }
        }
    }
    delete sim_thread;

    return 0;
}

//Разбор строки команды(str) на отдельные слова, разделяемые набором символов delim. Результат записывается в вектор arr.
//Возвращает ссылку на вектор со словами
std::vector<std::string>& Get_words(std::string& str, std::vector<std::string>& arr,std::string delim){
    size_t prev = 0;
    size_t next;
    size_t delta = delim.length();

    while( ( next = str.find( delim, prev ) ) != std::string::npos ){
      arr.push_back( str.substr( prev, next-prev ) );
      prev = next + delta;
    }
    arr.push_back( str.substr( prev ) );
    return arr;

}

//Сравнение строки input  с набором заданных комманд commands.
//В случае совпадения возвращает порядковый номер команды в массиве commands. Иначе возвращает -1
int8_t Parser(std::string& input_command,const std::string* commands_list, uint8_t num_of_commands) {
    int8_t command_num = -1;
    for (int i = 0; i < num_of_commands; i++){
        size_t first_word_ptr = input_command.find(commands_list[i]);

        if (first_word_ptr != std::string::npos) {
            if (first_word_ptr == 0) {
                command_num = i;
                break;
            }
        }
    }
    return command_num;
}

