#pragma once
#include "ISymptomChecker.h"
#include <vector>
#include <string>

// Проверка наличия критических симптомов (требуют срочной госпитализации)
class CriticalSymptomChecker : public ISymptomChecker {
private:
    // Список критических симптомов, которые требуют внимания
    std::vector<std::string> criticalSymptoms;

public:
    // Конструктор - заполняет список симптомов по умолчанию
    CriticalSymptomChecker();

    // Метод проверки (из интерфейса ISymptomChecker)
    std::vector<std::string> check(
        const std::vector<std::string>& symptoms,
        const Patient* patient
    ) override;

    void addCriticalSymptom(const std::string& symptom);
    void removeCriticalSymptom(const std::string& symptom);
};
