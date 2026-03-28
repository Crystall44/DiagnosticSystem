#pragma once
#include <string>
#include <vector>
#include "IDrugSource.h"
#include "LegacyDrugDB.h"

// Адаптер: превращает LegacyDrugDB в IDrugSource
class DrugAdapter : public IDrugSource {
private:
    LegacyDrugDB* legacyDB;
    mutable std::map<std::string, std::unique_ptr<Disease>> cachedDiseases;

    void loadDrugsToCache() const;

public:
    DrugAdapter(LegacyDrugDB* db);

    std::vector<std::unique_ptr<Disease>> getAllDrugs() const override;
    Disease* findDrugByName(const std::string& name) const override;
    std::vector<Symptom> getContraindicationsAsSymptoms(const std::string& drugName) const override;
};