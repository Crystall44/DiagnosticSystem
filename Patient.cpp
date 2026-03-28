#include "Patient.h"
#include <sstream>

// Создаёт пациента с указанными именем, возрастом и полом
Patient::Patient(const std::string& name, int age, const std::string& gender)
    : name(name), age(age), gender(gender) {}

// Возвращает имя пациента
std::string Patient::getName() const {
    return name;
}

// Возвращает возраст пациента
int Patient::getAge() const {
    return age;
}

// Возвращает пол пациента
std::string Patient::getGender() const {
    return gender;
}

// Возвращает историю болезней (копию вектора)
std::vector<std::string> Patient::getMedicalHistory() const {
    return medicalHistory;
}

// Добавляет запись в историю болезней
void Patient::addToMedicalHistory(const std::string& record) {
    medicalHistory.push_back(record);
}

// Возвращает список аллергий (копию вектора)
std::vector<std::string> Patient::getAllergies() const {
    return allergies;
}

// Добавляет аллергию в список
void Patient::addAllergy(const std::string& allergy) {
    allergies.push_back(allergy);
}

// Возвращает пустой вектор (будет переопределён в ChronicPatient)
std::vector<std::string> Patient::getChronicDiseases() const {
    return std::vector<std::string>(); // базовый класс возвращает пустой список
}

// Формирует строку с базовой информацией о пациенте
std::string Patient::getPatientInfo() const {
    std::stringstream ss;

    // Собираем строку: "Пациент: Имя, возраст лет, пол"
    ss << "Пациент: " << name << ", " << age << " лет, " << gender;
    return ss.str();
}