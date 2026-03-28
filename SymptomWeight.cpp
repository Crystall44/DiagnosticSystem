#include "SymptomWeight.h"

// Создаёт симптом с указанным названием и весом
SymptomWeight::SymptomWeight(const std::string& name, float weight)
    : symptomName(name), weight(weight) {}

// Возвращает название симптома
std::string SymptomWeight::getSymptomName() const {
    return symptomName;
}

// Возвращает весовой коэффициент
float SymptomWeight::getWeight() const {
    return weight;
}

// Изменяет весовой коэффициент
void SymptomWeight::setWeight(float w) {
    weight = w;
}