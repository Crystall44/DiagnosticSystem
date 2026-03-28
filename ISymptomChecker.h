#pragma once
#include <string>
#include <vector>
#include "Patient.h"

// Интерфейс для дополнительных проверок симптомов
class ISymptomChecker {
public:
    virtual ~ISymptomChecker() {}

    // Выполнить проверку симптомов
    // Возвращает список предупреждений (пустой, если всё в порядке)
    virtual std::vector<std::string> check(
        const std::vector<std::string>& symptoms,
        const Patient* patient
    ) = 0;
};

