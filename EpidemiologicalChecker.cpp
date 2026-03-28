#include "EpidemiologicalChecker.h"

// заполняет справочник сезонных заболеваний
EpidemiologicalChecker::EpidemiologicalChecker()
    : currentSeason("winter"), region("central") {
    // Ключ - сезон, значение - вектор строк с названиями болезней
    seasonalDiseases["winter"] = { "грипп", "ОРВИ", "ангина" };
    seasonalDiseases["spring"] = { "аллергия", "авитаминоз" };
    seasonalDiseases["summer"] = { "пищевое отравление", "солнечный удар" };
    seasonalDiseases["fall"] = { "ОРВИ", "депрессия" };
}

// Проверяет симптомы с учётом текущего сезона
std::vector<std::string> EpidemiologicalChecker::check(
    const std::vector<std::string>& symptoms, // симптомы пациента (не используются в этой реализации)
    const Patient* patient // пациент (не используется)
) {
    std::vector<std::string> warnings;

    // Ищем текущий сезон в map-е seasonalDiseases
    auto it = seasonalDiseases.find(currentSeason);

    // Если сезон найден
    if (it != seasonalDiseases.end()) {
        // Формируем предупреждение
        std::string warning = "Текущий сезон: " + currentSeason + ". Возможны сезонные заболевания: ";

        // Перебираем все болезни для этого сезона
        for (const auto& disease : it->second) {
            warning += disease + " ";
        }
        // Добавляем предупреждение в результат
        warnings.push_back(warning);
    }

    return warnings;
}

// Устанавливает текущий сезон
void EpidemiologicalChecker::setSeason(const std::string& season) {
    currentSeason = season;
}

// Возвращает текущий сезон
std::string EpidemiologicalChecker::getSeason() const {
    return currentSeason;
}

// Устанавливает регион
void EpidemiologicalChecker::setRegion(const std::string& region) {
    this->region = region;
}

// Добавляет новое сезонное заболевание
void EpidemiologicalChecker::addSeasonalDisease(
    const std::string& season,
    const std::string& diseaseName
) {
    // seasonalDiseases[season] - получаем вектор для этого сезона
    // Если сезона нет - создаётся новый вектор
    // .push_back(diseaseName) - добавляем болезнь в вектор
    seasonalDiseases[season].push_back(diseaseName);
}