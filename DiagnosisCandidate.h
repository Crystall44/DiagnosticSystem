#pragma once
#include <string>
#include <vector>
#include "Disease.h"

// Кандидат на диагноз - заболевание с вероятностью и совпавшими симптомами
class DiagnosisCandidate {
private:
    Disease* disease;
    float probability; // вероятность (0.0 - 1.0)
    std::vector<std::string> matchedSymptoms; // какие симптомы совпали

public:
    // Конструктор - принимает заболевание и вероятность
    DiagnosisCandidate(Disease* disease, float probability);

    Disease* getDisease() const;
    float getProbability() const;
    void setProbability(float prob);

    void addMatchedSymptom(const std::string& symptom);

    // Получить все совпавшие симптомы
    std::vector<std::string> getMatchedSymptoms() const;

    // Получить текстовую оценку уверенности (Низкая/Средняя/Высокая)
    std::string getConfidenceLevel() const;
};
