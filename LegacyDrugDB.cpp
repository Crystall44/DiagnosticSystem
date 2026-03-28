#include "LegacyDrugDB.h"

LegacyDrugDB::LegacyDrugDB() {
    // Заполняем тестовые данные
    drugNames["ASP001"] = "Аспирин";
    drugNames["PEN001"] = "Пенициллин";
    drugNames["PAR001"] = "Парацетамол";
    drugNames["IBU001"] = "Ибупрофен";

    contraindications["Аспирин"] = { "язва желудка", "гемофилия", "бронхиальная астма" };
    contraindications["Пенициллин"] = { "аллергия на пенициллин" };
    contraindications["Парацетамол"] = { "заболевания печени" };
    contraindications["Ибупрофен"] = { "язва желудка", "заболевания почек" };
}

std::string LegacyDrugDB::queryDrugData(const std::string& query) {
    // Формат запроса: "DRUG:CODE" или "CONTRA:NAME"
    if (query.find("DRUG:") == 0) {
        std::string code = query.substr(5);
        auto it = drugNames.find(code);
        if (it != drugNames.end()) {
            return it->second;
        }
        return "UNKNOWN";
    }
    else if (query.find("CONTRA:") == 0) {
        std::string name = query.substr(7);
        auto it = contraindications.find(name);
        if (it != contraindications.end()) {
            std::string result;
            for (const auto& c : it->second) {
                result += c + ",";
            }
            return result;
        }
        return "";
    }
    return "";
}

std::string LegacyDrugDB::fetchRecord(int id) {
    // Упрощённая имитация
    if (id == 1) return "ASP001|Аспирин|противовоспалительное";
    if (id == 2) return "PEN001|Пенициллин|антибиотик";
    return "";
}

std::vector<std::string> LegacyDrugDB::getContraindicationsInternal(const std::string& drugCode) {
    std::string name = queryDrugData("DRUG:" + drugCode);
    auto it = contraindications.find(name);
    if (it != contraindications.end()) {
        return it->second;
    }
    return {};
}