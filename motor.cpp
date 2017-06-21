#include "motor.h"


Motor::Motor(): S_max_aups(1.0f),A_aups(1.0f),TP(0),P(0),V(0){ }

void Motor::Set_S_max_aups(const float value) {
    S_max_aups = value;
}
void Motor::Set_A_aups(const float value){
    A_aups = value;
}

void Motor::Set_TP(const float value) {
    TP = value;
    if (A_aups == 0) {
        A_aups = 1;
    }
}

/*void Motor::Set_P(const float value){
    P = value;
}*/

void Motor::Set_V(const float value){
    V = value;
}

float Motor::Get_S_max_aups() const {
    return S_max_aups;
}
float Motor::Get_A_aups() const {
    return A_aups;
}

float Motor::Get_TP() const {
    return TP;
}

float Motor::Get_P() const {
    return P;
}

float Motor::Get_V() const {
    return V;
}

void Motor::Update_position(const float dT) {
    if ( P != TP) {
        P += V*dT;
    }
    else {V =0; A_aups = 0;}
}

void Motor::Update_velocity(const float dT) {
    if (TP < P) {
        V -= A_aups*dT;
    }
    else {
        V += A_aups*dT;
    }
    if (abs(V) >= S_max_aups) {V = V < 0 ? -1 * S_max_aups : S_max_aups; A_aups = 0;}
}
