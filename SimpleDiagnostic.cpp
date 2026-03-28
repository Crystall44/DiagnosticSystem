#include "SimpleDiagnostic.h"
#include "DiagnosisResult.h"
#include <algorithm>

// Сохраняет порог совпадения (по умолчанию 0.3)
SimpleDiagnostic::SimpleDiagnostic(float threshold) : threshold(threshold) {}

std::unique_ptr<DiagnosisResult> SimpleDiagnostic::performDiagnosis(
    const Appointment* appointment,
    const DiseaseDirectory* directory
) {
    auto result = std::make_unique<DiagnosisResult>();
    auto patient = appointment->getPatient();
    auto symptoms = appointment->getSymptoms();

    // Получаем все заболевания
    auto diseases = directory->getAllDiseases();

    // Для каждого заболевания считаем вероятность
    for (const auto& disease : diseases) {
        auto diseaseSymptoms = disease->getSymptoms();

        // Используем matchSymptoms для вычисления вероятности
        float probability = matchSymptoms(symptoms, diseaseSymptoms);

        // Если вероятность выше порога
        if (probability >= threshold) {
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

// Вычисляет процент совпадения симптомов
float SimpleDiagnostic::matchSymptoms(
    const std::vector<std::string>& patientSymptoms, // симптомы пациента
    const std::vector<SymptomWeight>& diseaseSymptoms // эталонные симптомы болезни
) {
    // Если у болезни нет симптомов - вероятность 0
    if (diseaseSymptoms.empty()) return 0.0f;

    int matchCount = 0; // счётчик совпавших симптомов

    // Перебираем все симптомы пациента
    for (const auto& patientSymptom : patientSymptoms) {
        // Перебираем все симптомы болезни
        for (const auto& diseaseSymptom : diseaseSymptoms) {
            // Если названия совпадают
            if (patientSymptom == diseaseSymptom.getSymptomName()) {
                matchCount++;// увеличиваем счётчик
                break; // выходим из внутреннего цикла (нашли этот симптом)
            }
        }
    }

    // Возвращаем отношение совпавших симптомов к общему количеству
    return static_cast<float>(matchCount) / diseaseSymptoms.size();
}

// Устанавливает новый порог совпадения
void SimpleDiagnostic::setThreshold(float t) {
    threshold = t;
}

// Возвращает текущий порог совпадения
float SimpleDiagnostic::getThreshold() const {
    return threshold;
}