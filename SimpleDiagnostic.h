#pragma once
#include "IDiagnosticAlgorithm.h"

// Простая диагностика - процент совпадения симптомов (все симптомы равнозначны)
class SimpleDiagnostic : public IDiagnosticAlgorithm {
private:
    float threshold;// порог совпадения (по умолчанию 0.3 = 30%)

public:
    // Конструктор с возможностью задать порог
    SimpleDiagnostic(float threshold = 0.3f); 

    // Полная диагностика
    std::unique_ptr<DiagnosisResult> performDiagnosis(
        const Appointment* appointment,
        const DiseaseDirectory* directory
    ) override;


    // Вычисляет процент совпадения симптомов
    float matchSymptoms(
        const std::vector<std::string>& patientSymptoms,
        const std::vector<SymptomWeight>& diseaseSymptoms
    ) override;

    // Изменить порог совпадения
    void setThreshold(float t);

    // Получить текущий порог
    float getThreshold() const;
};
