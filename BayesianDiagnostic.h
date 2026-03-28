#pragma once
#include "IDiagnosticAlgorithm.h"
#include <map> // для хранения пар "болезнь -> вероятность"
#include <memory>

// Вероятностная диагностика на основе формулы Байеса
class BayesianDiagnostic : public IDiagnosticAlgorithm {
private:
    // Ключ: название болезни, значение: априорная вероятность
    std::map<std::string, float> priorProbabilities;

    // Значение по умолчанию, если болезнь не найдена в map
    float defaultPrior;

public:
    BayesianDiagnostic(float defaultPrior = 0.01f);

    // Основной метод диагностики
    float matchSymptoms(
        const std::vector<std::string>& patientSymptoms,
        const std::vector<SymptomWeight>& diseaseSymptoms
    ) override;

    // Полная диагностика
    std::unique_ptr<DiagnosisResult> performDiagnosis(
        const Appointment* appointment,
        const DiseaseDirectory* directory
    ) override;

    // Установить априорную вероятность для конкретной болезни
    void setPriorProbability(const std::string& diseaseName, float probability);

    // Получить априорную вероятность болезни
    float getPriorProbability(const std::string& diseaseName) const;

private:
    // Приватный метод для вычисления формулы Байеса
    float calculateBayesian(float prior, float likelihood, float evidence) const;
};
