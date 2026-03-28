#include "MarkdownDecorator.h"
#include <sstream>
#include <iomanip>

MarkdownDecorator::MarkdownDecorator(const DiagnosisResult* res) : result(res) {}

std::string MarkdownDecorator::generate() const {
    std::stringstream ss;

    ss << "# Результат диагностики\n\n";

    // Информация о времени
    ss << "**Время обработки:** " << result->getProcessingTime() << " мс\n\n";

    // Диагнозы
    auto candidates = result->getCandidates();
    if (candidates.empty()) {
        ss << "## Диагноз не найден\n\n";
    }
    else {
        ss << "## Наиболее вероятные диагнозы\n\n";
        ss << "| № | Заболевание               | Вероятность | Уверенность |\n";
        ss << "|---|---------------------------|-------------|-------------|\n";

        int i = 1;
        for (const auto& candidate : candidates) {
            std::string diseaseName = candidate.getDisease()->getName();
            diseaseName += std::string(25 - diseaseName.length(), ' ');

            std::stringstream probStream;
            probStream << std::fixed << std::setprecision(1)
                << candidate.getProbability() * 100 << "%";
            std::string prob = probStream.str();
            prob += std::string(11 - prob.length(), ' ');

            std::string confidence = candidate.getConfidenceLevel();
            confidence += std::string(11 - confidence.length(), ' ');

            ss << "| " << i++ << " | "
                << diseaseName << " | "
                << prob << " | "
                << confidence << " |\n";
        }
        ss << "\n";

        // Совпавшие симптомы для лучшего диагноза
        if (!candidates.empty()) {
            auto best = candidates[0];
            auto matched = best.getMatchedSymptoms();
            if (!matched.empty()) {
                ss << "### Совпавшие симптомы\n\n";
                for (const auto& sym : matched) {
                    ss << "- " << sym << "\n";
                }
                ss << "\n";
            }
        }
    }

    // Предупреждения
    auto warnings = result->getWarnings();
    if (!warnings.empty()) {
        ss << "## Предупреждения\n\n";
        for (const auto& warning : warnings) {
            ss << warning << "\n";
        }
        ss << "\n";
    }

    return ss.str();
}