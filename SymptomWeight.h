#pragma once
#include <string>

// Симптом с весовым коэффициентом (важность для диагностики)
class SymptomWeight {
private:
    std::string symptomName; // название симптома
    float weight; // весовой коэффициент (0.0 - 1.0)

public:
    // Конструктор: принимает название симптома и его вес
    SymptomWeight(const std::string& name, float weight);

    std::string getSymptomName() const;
    float getWeight() const;
    void setWeight(float w);
};
