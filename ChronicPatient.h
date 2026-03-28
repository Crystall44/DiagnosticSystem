#pragma once
#include "Patient.h"
#include <map>

// Пациент с хроническими заболеваниями
class ChronicPatient : public Patient { // наследуется от Patient
private:
    // Ключ: название болезни, значение: дата диагностирования
    std::map<std::string, std::string> chronicDiseases;

public:
    // Конструктор
    ChronicPatient(const std::string& name, int age, const std::string& gender);

    // Получить список хронических болезней
    std::vector<std::string> getChronicDiseases() const override;

    // Добавить новую хроническую болезнь
    void addChronicDisease(const std::string& disease, const std::string& diagnosedDate);

    // Получить информацию о пациенте (переопределение)
    std::string getPatientInfo() const override;
};
