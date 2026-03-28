#include "Disease.h"
#include <algorithm>

// Создаёт заболевание с названием и кодом МКБ-10
Disease::Disease(const std::string& name, const std::string& icd10Code)
    : name(name), icd10Code(icd10Code) {}

// Возвращает название заболевания
std::string Disease::getName() const {
    return name;
}

// Возвращает код по МКБ-10
std::string Disease::getIcd10Code() const {
    return icd10Code;
}

// Добавляет симптом с указанным весом
void Disease::addSymptom(const std::string& symptomName, float weight) {
    symptoms.push_back(SymptomWeight(symptomName, weight));
}

// Возвращает вектор симптомов (с весами)
std::vector<SymptomWeight> Disease::getSymptoms() const {
    return symptoms;
}

// Удаляет симптом по названию
void Disease::removeSymptom(const std::string& symptomName) {
    // std::remove_if перемещает все элементы, НЕ удовлетворяющие условию,
    // в начало вектора, и возвращает итератор на первый "удалённый" элемент
    auto it = std::remove_if(symptoms.begin(), symptoms.end(),
        [&symptomName](const SymptomWeight& sw) {
            // возвращаем true для элементов, которые нужно "удалить"
            return sw.getSymptomName() == symptomName;
        });
    // erase реально удаляет элементы от it до конца
    symptoms.erase(it, symptoms.end());
}