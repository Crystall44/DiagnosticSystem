#include "DiagnosisResult.h"
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <ctime>

// Устанавливает текущее время и обнуляет время обработки
DiagnosisResult::DiagnosisResult() : timestamp(std::time(nullptr)), processingTimeMs(0) {}

// Добавляет кандидата и сразу сортирует список
void DiagnosisResult::addCandidate(const DiagnosisCandidate& candidate) {
    candidates.push_back(candidate); // добавляем в конец
    sortCandidatesByProbability(); // сортируем (самый вероятный - первый)
}

// Возвращает всех кандидатов (копию вектора)
std::vector<DiagnosisCandidate> DiagnosisResult::getCandidates() const {
    return candidates;
}

// Возвращает первых N кандидатов (самых вероятных)
std::vector<DiagnosisCandidate> DiagnosisResult::getTopCandidates(int count) const {
    std::vector<DiagnosisCandidate> result;
    // Берем минимум из запрошенного count и реального размера
    int limit = std::min(count, static_cast<int>(candidates.size()));

    // Копируем первые limit элементов
    for (int i = 0; i < limit; ++i) {
        result.push_back(candidates[i]);
    }
    return result;
}

// Возвращает самого вероятного кандидата (первый в отсортированном списке)
DiagnosisCandidate DiagnosisResult::getBestCandidate() const {
    if (candidates.empty()) {
        return DiagnosisCandidate(nullptr, 0.0f);
    }
    return candidates[0]; // первый элемент - самый вероятный (после сортировки)
}

// Добавляет предупреждение в список
void DiagnosisResult::addWarning(const std::string& warning) {
    warnings.push_back(warning);
}

// Возвращает все предупреждения
std::vector<std::string> DiagnosisResult::getWarnings() const {
    return warnings;
}

// Проверяет, есть ли предупреждения
bool DiagnosisResult::hasWarnings() const {
    return !warnings.empty();
}

// Устанавливает время диагностики
void DiagnosisResult::setTimestamp(std::time_t time) {
    timestamp = time;
}

// Возвращает время диагностики
std::time_t DiagnosisResult::getTimestamp() const {
    return timestamp;
}

// Устанавливает время выполнения диагностики (в миллисекундах)
void DiagnosisResult::setProcessingTime(int ms) {
    processingTimeMs = ms;
}

// Возвращает время выполнения диагностики
int DiagnosisResult::getProcessingTime() const {
    return processingTimeMs;
}

// Сортирует кандидатов по убыванию вероятности
void DiagnosisResult::sortCandidatesByProbability() {
    // std::sort с лямбда-функцией для сравнения
    std::sort(candidates.begin(), candidates.end(),
        [](const DiagnosisCandidate& a, const DiagnosisCandidate& b) {
            // Возвращаем true, если a должно идти перед b
            return a.getProbability() > b.getProbability();
        });
}

std::string DiagnosisResult::generate() const {
    std::stringstream ss;

    ss << "РЕЗУЛЬТАТ ДИАГНОСТИКИ\n";
    ss << "====================\n\n";

    ss << "Время обработки: " << processingTimeMs << " мс\n\n";

    if (candidates.empty()) {
        ss << "Диагноз не найден.\n";
    }
    else {
        ss << "Наиболее вероятные диагнозы:\n";
        int i = 1;
        for (const auto& candidate : candidates) {
            ss << i++ << ". " << candidate.getDisease()->getName()
                << " - вероятность: " << std::fixed << std::setprecision(1)
                << candidate.getProbability() * 100 << "%\n";
            ss << "   Уверенность: " << candidate.getConfidenceLevel() << "\n";

            auto matched = candidate.getMatchedSymptoms();
            if (!matched.empty()) {
                ss << "   Совпавшие симптомы: ";
                for (size_t j = 0; j < matched.size(); ++j) {
                    ss << matched[j];
                    if (j < matched.size() - 1) ss << ", ";
                }
                ss << "\n";
            }
        }
    }

    if (!warnings.empty()) {
        ss << "\nПРЕДУПРЕЖДЕНИЯ:\n";
        for (const auto& warning : warnings) {
            ss << "- " << warning << "\n";
        }
    }

    return ss.str();
}