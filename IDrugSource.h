#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Disease.h"
#include "Symptom.h"

// Целевой интерфейс — то, что хочет наша система
class IDrugSource {
public:
    virtual ~IDrugSource() = default;

    // Получить все лекарства как заболевания
    virtual std::vector<std::unique_ptr<Disease>> getAllDrugs() const = 0;

    // Найти лекарство по названию
    virtual Disease* findDrugByName(const std::string& name) const = 0;

    // Получить противопоказания как симптомы
    virtual std::vector<Symptom> getContraindicationsAsSymptoms(const std::string& drugName) const = 0;
};