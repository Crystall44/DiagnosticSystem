#include "CriticalSymptomChecker.h"
#include <algorithm> // для std::find

CriticalSymptomChecker::CriticalSymptomChecker() {
    criticalSymptoms = {
        "высокая температура >39",
        "потеря сознания",
        "судороги",
        "кровотечение",
        "острая боль в груди"
    };
}

// Проверяет, есть ли среди симптомов пациента критические
std::vector<std::string> CriticalSymptomChecker::check(
    const std::vector<std::string>& symptoms,
    const Patient* patient
) {
    std::vector<std::string> warnings;

    for (const auto& criticalSymptom : criticalSymptoms) {
        for (const auto& symptom : symptoms) {
            // Ищем вхождение критического симптома в симптом пациента
            if (symptom.find(criticalSymptom) != std::string::npos) {
                // Добавляем первое предупреждение - какой симптом обнаружен
                warnings.push_back("ВНИМАНИЕ! Обнаружен критический симптом: " + criticalSymptom);
                // Добавляем второе предупреждение - рекомендация
                warnings.push_back("Рекомендуется срочная госпитализация!");
                break;
            }
        }
    }

    return warnings;
}

void CriticalSymptomChecker::addCriticalSymptom(const std::string& symptom) {
    criticalSymptoms.push_back(symptom);
}

// Удаляет симптом из списка критических
void CriticalSymptomChecker::removeCriticalSymptom(const std::string& symptom) {
    auto it = std::find(criticalSymptoms.begin(), criticalSymptoms.end(), symptom);
    // Проверяем, нашли ли элемент
    if (it != criticalSymptoms.end()) {
        // erase(итератор) - удаляет элемент по итератору
        criticalSymptoms.erase(it);
    }
}