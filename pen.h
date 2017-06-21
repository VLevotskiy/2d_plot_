#ifndef PEN_H
#define PEN_H
#include <string>
#include <motor.h>
#include <memory>

class Pen {
private:
    std::shared_ptr<Motor> p_motor_x;   //указатель на мотор на оси x
    std::shared_ptr<Motor> p_motor_y;   //указатель на мотор на оси у
    bool toggled;                       //состояние пера
public:
    Pen();
    bool Attatch_Motor_x(const std::shared_ptr<Motor>&);      //прикрепить мотор к оси х
    bool Attatch_Motor_y(const std::shared_ptr<Motor>&);      //прикрепить мотор к оси y
    void Set_Toggle(bool state);                              //установить состояние пера

    float Get_pos_x() const;
    float Get_pos_y() const;
    bool isToggled() const;
};


#endif // PEN_H
