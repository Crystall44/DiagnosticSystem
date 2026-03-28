#include "RegularPatient.h"
#include <sstream>

// Просто передаёт параметры конструктору базового класса Patient
RegularPatient::RegularPatient(const std::string& name, int age, const std::string& gender)
    : Patient(name, age, gender) {}

// Переопределённый метод - возвращает информацию об обычном пациенте
std::string RegularPatient::getPatientInfo() const {
    std::stringstream ss;
    ss << "Обычный пациент: " << name << ", " << age << " лет, " << gender;
    return ss.str();
}