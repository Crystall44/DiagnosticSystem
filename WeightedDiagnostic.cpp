#include "WeightedDiagnostic.h"

// Сохраняет флаг нормализации
WeightedDiagnostic::WeightedDiagnostic(bool normalize) : normalize(normalize) {}

std::unique_ptr<DiagnosisResult> WeightedDiagnostic::performDiagnosis(
    const Appointment* appointment,
    const DiseaseDirectory* directory
) {
    auto result = std::make_unique<DiagnosisResult>();
    auto symptoms = appointment->getSymptoms();
    auto diseases = directory->getAllDiseases();

    for (const auto& disease : diseases) {
        auto diseaseSymptoms = disease->getSymptoms();
        float probability = matchSymptoms(symptoms, diseaseSymptoms);

        // Для взвешенной диагностики используем меньший порог
        if (probability > 0.1f) {
            DiagnosisCandidate candidate(const_cast<Disease*>(disease), probability);

            // Добавляем совпавшие симптомы
            for (const auto& patientSym : symptoms) {
                for (const auto& diseaseSym : diseaseSymptoms) {
                    if (patientSym == diseaseSym.getSymptomName()) {
                        candidate.addMatchedSymptom(patientSym);
                        break;
                    }
                }
            }

            result->addCandidate(candidate);
        }
    }

    return result;
}

// Вычисляет взвешенную сумму совпавших симптомов
float WeightedDiagnostic::matchSymptoms(
    const std::vector<std::string>& patientSymptoms, // симптомы пациента
    const std::vector<SymptomWeight>& diseaseSymptoms // эталонные симптомы с весами
) {
    // Если у болезни нет симптомов - вероятность 0
    if (diseaseSymptoms.empty()) return 0.0f;

    float totalWeight = 0.0f; // сумма весов всех симптомов болезни
    float matchedWeight = 0.0f; // сумма весов только совпавших симптомов

    // Перебираем все симптомы болезни (с весами)
    for (const auto& diseaseSymptom : diseaseSymptoms) {
        // Добавляем вес текущего симптома к общей сумме
        totalWeight += diseaseSymptom.getWeight();

        // Проверяем, есть ли этот симптом у пациента
        for (const auto& patientSymptom : patientSymptoms) {
            // Если названия совпадают
            if (patientSymptom == diseaseSymptom.getSymptomName()) {
                // Добавляем вес этого симптома к сумме совпавших
                matchedWeight += diseaseSymptom.getWeight();
                break;
            }
        }
    }

    // Если нужна нормализация и общая сумма не ноль
    if (normalize && totalWeight > 0) {
        // Возвращаем долю совпавших весов
        return matchedWeight / totalWeight;
    }

    // Иначе возвращаем просто сумму весов совпавших симптомов
    return matchedWeight;
}

// Включает/выключает нормализацию
void WeightedDiagnostic::setNormalization(bool enable) {
    normalize = enable;
}