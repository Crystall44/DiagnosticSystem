#include "ChronicPatient.h"
#include <sstream> // дл€ работы с потоками строк

ChronicPatient::ChronicPatient(const std::string& name, int age, const std::string& gender)
    : Patient(name, age, gender) {}

// ¬озвращает список названий хронических заболеваний (без дат)
std::vector<std::string> ChronicPatient::getChronicDiseases() const {
    std::vector<std::string> result;
    for (const auto& pair : chronicDiseases) {
        result.push_back(pair.first);
    }
    return result;
}

// ƒобавл€ет новое хроническое заболевание или обновл€ет дату существующего
void ChronicPatient::addChronicDisease(const std::string& disease, const std::string& diagnosedDate) {
    chronicDiseases[disease] = diagnosedDate;
}

// ‘ормирует строку с полной информацией о пациенте
std::string ChronicPatient::getPatientInfo() const {
    // —оздаем строковый поток
    std::stringstream ss;
    ss << "ѕациент с хроническими заболевани€ми: " << name << ", " << age << " лет, " << gender;
    if (!chronicDiseases.empty()) {
        ss << "\n’ронические заболевани€:";
        for (const auto& pair : chronicDiseases) {
            ss << "\n  - " << pair.first << " (с " << pair.second << ")";
        }
    }
    return ss.str();
}