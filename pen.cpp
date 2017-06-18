#include "pen.h"

Pen::Pen() : toggled(true){
}

bool Pen::Attatch_Motor_x(std::shared_ptr<Motor>& ptr) {
    if (!p_motor_x) {
        p_motor_x = ptr;
        return true;
    }
    return false;
}

bool Pen::Attatch_Motor_y(std::shared_ptr<Motor>& ptr) {
    if (!p_motor_y) {
        p_motor_y = ptr;
        return true;
    }
    return false;
}

void Pen::Set_Toggle(bool state) {
    toggled = state;
}

bool Pen::isToggled() const {
    return toggled;
}

float Pen::Get_pos_x() const {
    if (p_motor_x)
        return p_motor_x->Get_P();
    else {
        return 0;
    }
}

float Pen::Get_pos_y() const{
    if (p_motor_y)
        return p_motor_y->Get_P();
    else {
        return 0;
    }
}
