#include "DiagnosisCandidate.h"

DiagnosisCandidate::DiagnosisCandidate(Disease* disease, float probability)
    : disease(disease), probability(probability) {}

Disease* DiagnosisCandidate::getDisease() const {
    return disease;
}

float DiagnosisCandidate::getProbability() const {
    return probability;
}

// Устанавливает новое значение вероятности
void DiagnosisCandidate::setProbability(float prob) {
    probability = prob;
}

// Добавляет симптом, который совпал с эталонным, в список
void DiagnosisCandidate::addMatchedSymptom(const std::string& symptom) {
    matchedSymptoms.push_back(symptom);
}

std::vector<std::string> DiagnosisCandidate::getMatchedSymptoms() const {
    return matchedSymptoms;
}

// Преобразует числовую вероятность в текстовую оценку уверенности
std::string DiagnosisCandidate::getConfidenceLevel() const {
    if (probability >= 0.8f) return "Высокая";
    if (probability >= 0.5f) return "Средняя";
    return "Низкая";
}