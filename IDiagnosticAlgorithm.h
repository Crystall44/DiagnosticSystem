#pragma once
#include <string>
#include <vector>
#include <memory>
#include "SymptomWeight.h"
#include "DiagnosisResult.h"
#include "Appointment.h"
#include "DiseaseDirectory.h"

// Интерфейс для всех алгоритмов диагностики (стратегия)
class IDiagnosticAlgorithm {
public:
    virtual ~IDiagnosticAlgorithm() {}

    // Возвращает уникальный указатель на результат диагностики
    virtual std::unique_ptr<DiagnosisResult> performDiagnosis(
        const Appointment* appointment,
        const DiseaseDirectory* directory
    ) = 0;

    // Сопоставить симптомы пациента с симптомами заболевания
    // Возвращает вероятность/степень совпадения (0.0 - 1.0)
    virtual float matchSymptoms(
        const std::vector<std::string>& patientSymptoms,
        const std::vector<SymptomWeight>& diseaseSymptoms
    ) = 0;
};
