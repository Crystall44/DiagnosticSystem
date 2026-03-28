#pragma once
#include <string>
#include <vector>
#include "SymptomWeight.h"

// Класс заболевания (название, код МКБ-10, список симптомов с весами)
class Disease {
private:
    std::string name; // название заболевания
    std::string icd10Code; // код по международной классификации болезней
    std::vector<SymptomWeight> symptoms; // симптомы с весовыми коэффициентами

public:
    Disease(const std::string& name, const std::string& icd10Code);

    std::string getName() const;
    std::string getIcd10Code() const;

    void addSymptom(const std::string& symptomName, float weight);
    std::vector<SymptomWeight> getSymptoms() const;

    // Удалить симптом по названию
    void removeSymptom(const std::string& symptomName);
};
