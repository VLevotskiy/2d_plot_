#ifndef MOTOR_H
#define MOTOR_H
#include <string>

class Motor {
private:
    //std::string name;
    float S_max_aups;   //максимальнеая скорость
    float A_aups;       //ускорение
    float TP;           //целевая позиция
    float P;            //текущая позиция
    float V;            //текущая скорость
    //void Set_P(const float);
public:
    Motor();
    void Set_S_max_aups(const float value);
    void Set_A_aups(const float value);
    void Set_TP(const float value);
    void Set_V(const float value);

    void Update_position(const float time);  //Вычисление текущей позиции
    void Update_velocity(const float time);  //Вычисление текущей скорости

    float Get_S_max_aups() const;
    float Get_A_aups() const;
    float Get_TP() const;
    float Get_P() const;
    float Get_V() const;
};

#endif // MOTOR_H
