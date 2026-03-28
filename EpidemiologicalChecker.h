#pragma once
#include "ISymptomChecker.h"
#include <map>

// Учёт эпидемиологической обстановки и сезонности
class EpidemiologicalChecker : public ISymptomChecker {
private:
    std::string currentSeason; // текущий сезон
    std::string region; // регион
    std::map<std::string, std::vector<std::string>> seasonalDiseases; // сезон -> список болезней

public:
    EpidemiologicalChecker();

    // Проверить симптомы с учётом эпидобстановки
    std::vector<std::string> check(
        const std::vector<std::string>& symptoms,
        const Patient* patient
    ) override;

    void setSeason(const std::string& season);
    std::string getSeason() const;
    void setRegion(const std::string& region);

    // Добавить сезонное заболевание
    void addSeasonalDisease(const std::string& season, const std::string& diseaseName);
};