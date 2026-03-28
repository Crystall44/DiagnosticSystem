#pragma once
#include "IDiagnosticAlgorithm.h"

// Взвешенная диагностика - с учётом весовых коэффициентов симптомов
class WeightedDiagnostic : public IDiagnosticAlgorithm {
private:
    bool normalize; // флаг: нормировать ли результат на сумму весов

public:
    // Конструктор с возможностью отключить нормировку
    WeightedDiagnostic(bool normalize = true);

    // Вычисляет взвешенную сумму совпавших симптомов
    // Если normalize = true, делит на сумму всех весов
    float matchSymptoms(
        const std::vector<std::string>& patientSymptoms,
        const std::vector<SymptomWeight>& diseaseSymptoms
    ) override;

    // Полная диагностика
    std::unique_ptr<DiagnosisResult> performDiagnosis(
        const Appointment* appointment,
        const DiseaseDirectory* directory
    ) override;

    // Включить/выключить нормировку
    void setNormalization(bool enable);
};
