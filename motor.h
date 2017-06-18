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
public:
    Motor();
    void Set_S_max_aups(const float);
    void Set_A_aups(const float);
    void Set_TP(const float);
    void Set_P(const float);
    void Set_V(const float);

    void Update_position(const float);  //Вычисление текущей позиции
    void Update_velocity(const float);  //Вычисление текущей скорости

    float Get_S_max_aups() const;
    float Get_A_aups() const;
    float Get_TP() const;
    float Get_P() const;
    float Get_V() const;
};

#endif // MOTOR_H
