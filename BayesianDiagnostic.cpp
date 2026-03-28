#include "BayesianDiagnostic.h"
#include <cmath>

BayesianDiagnostic::BayesianDiagnostic(float defaultPrior)
    : defaultPrior(defaultPrior) {}

std::unique_ptr<DiagnosisResult> BayesianDiagnostic::performDiagnosis(
    const Appointment* appointment,
    const DiseaseDirectory* directory
) {
    auto result = std::make_unique<DiagnosisResult>();
    auto symptoms = appointment->getSymptoms();
    auto diseases = directory->getAllDiseases();

    // Вычисляем evidence (общую вероятность всех симптомов)
    float evidence = 0.0f;
    std::map<std::string, float> likelihoods;

    for (const auto& disease : diseases) {
        auto diseaseSymptoms = disease->getSymptoms();
        float likelihood = 0.0f;

        for (const auto& diseaseSym : diseaseSymptoms) {
            for (const auto& patientSym : symptoms) {
                if (patientSym == diseaseSym.getSymptomName()) {
                    likelihood += diseaseSym.getWeight();
                    break;
                }
            }
        }
        likelihood /= diseaseSymptoms.size();
        likelihoods[disease->getName()] = likelihood;

        float prior = getPriorProbability(disease->getName());
        evidence += prior * likelihood;
    }

    // Вычисляем апостериорные вероятности
    for (const auto& disease : diseases) {
        float prior = getPriorProbability(disease->getName());
        float likelihood = likelihoods[disease->getName()];

        float probability = calculateBayesian(prior, likelihood, evidence);

        if (probability > 0.01f) {
            DiagnosisCandidate candidate(const_cast<Disease*>(disease), probability);

            // Добавляем совпавшие симптомы
            auto diseaseSymptoms = disease->getSymptoms();
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


float BayesianDiagnostic::matchSymptoms(
    const std::vector<std::string>& patientSymptoms,
    const std::vector<SymptomWeight>& diseaseSymptoms
) {
    if (diseaseSymptoms.empty()) return 0.0f;

    // 1. Берем априорную вероятность (для конкретной болезни)
    float prior = defaultPrior;

    // Вычисляем likelihood (правдоподобие)
    float likelihood = 0.0f;

    // Проходим по всем симптомам болезни
    for (const auto& diseaseSymptom : diseaseSymptoms) {
        for (const auto& patientSymptom : patientSymptoms) {
            // Проверяем, есть ли этот симптом у пациента
            if (patientSymptom == diseaseSymptom.getSymptomName()) {
                // Если есть - добавляем вес симптома
                likelihood += diseaseSymptom.getWeight();
                break; // выходим из внутреннего цикла
            }
        }
    }

    // Нормируем (делим на количество симптомов болезни)
    likelihood /= diseaseSymptoms.size();

    // Упрощенная формула Байеса
    return prior * likelihood;
}

void BayesianDiagnostic::setPriorProbability(
    const std::string& diseaseName,
    float probability
) {
    priorProbabilities[diseaseName] = probability;
}

float BayesianDiagnostic::getPriorProbability(const std::string& diseaseName) const {
    auto it = priorProbabilities.find(diseaseName);
    if (it != priorProbabilities.end()) {
        return it->second;
    }
    return defaultPrior;
}

float BayesianDiagnostic::calculateBayesian(
    float prior,
    float likelihood,
    float evidence
) const {
    if (evidence == 0) return 0;
    return (prior * likelihood) / evidence;
}