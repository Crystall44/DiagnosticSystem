#include "DrugAdapter.h"
#include <algorithm>

DrugAdapter::DrugAdapter(LegacyDrugDB* db) : legacyDB(db) {
    loadDrugsToCache();
}

void DrugAdapter::loadDrugsToCache() const {
    // Загружаем лекарства из legacyDB в кэш как заболевания
    // В реальности здесь был бы запрос ко всем лекарствам
    std::vector<std::string> drugCodes = { "ASP001", "PEN001", "PAR001", "IBU001" };

    for (const auto& code : drugCodes) {
        std::string name = legacyDB->queryDrugData("DRUG:" + code);
        if (name != "UNKNOWN") {
            auto disease = std::make_unique<Disease>(name, "DRG_" + code);

            // Добавляем противопоказания как симптомы
            auto contraindications = legacyDB->getContraindicationsInternal(code);
            for (const auto& contra : contraindications) {
                disease->addSymptom(contra, 0.9f);  // высокий вес для противопоказаний
            }

            cachedDiseases[name] = std::move(disease);
        }
    }
}

std::vector<std::unique_ptr<Disease>> DrugAdapter::getAllDrugs() const {
    std::vector<std::unique_ptr<Disease>> result;
    for (auto it = cachedDiseases.begin(); it != cachedDiseases.end(); ++it) {
        result.push_back(std::make_unique<Disease>(*(it->second)));
    }
    return result;
}

Disease* DrugAdapter::findDrugByName(const std::string& name) const {
    auto it = cachedDiseases.find(name);
    if (it != cachedDiseases.end()) {
        return it->second.get();
    }
    return nullptr;
}

std::vector<Symptom> DrugAdapter::getContraindicationsAsSymptoms(const std::string& drugName) const {
    std::vector<Symptom> symptoms;

    auto disease = findDrugByName(drugName);
    if (disease) {
        for (const auto& sw : disease->getSymptoms()) {
            symptoms.emplace_back(sw.getSymptomName(), "Противопоказание", 10);
        }
    }

    return symptoms;
}