#pragma once
#include "Patient.h"

// Обычный пациент (стандартная реализация)
class RegularPatient : public Patient {
public:
    // Конструктор передаёт параметры базовому классу Patient
    RegularPatient(const std::string& name, int age, const std::string& gender);

    // Переопределённый метод для получения информации о пациенте
    std::string getPatientInfo() const override;
};
