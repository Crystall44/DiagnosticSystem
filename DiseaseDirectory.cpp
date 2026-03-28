#include "DiseaseDirectory.h"
#include <algorithm>

// Добавляет новое заболевание в справочник
void DiseaseDirectory::addDisease(std::unique_ptr<Disease> disease) {
    diseases.push_back(std::move(disease));
}

// Ищет заболевание по названию
Disease* DiseaseDirectory::findDiseaseByName(const std::string& name) const {
    for (const auto& disease : diseases) {
        if (disease->getName() == name) {
            return disease.get();
        }
    }
    return nullptr;
}

// Ищет заболевание по коду МКБ-10
Disease* DiseaseDirectory::findDiseaseByCode(const std::string& icd10Code) const {
    for (const auto& disease : diseases) {
        if (disease->getIcd10Code() == icd10Code) {
            return disease.get();
        }
    }
    return nullptr;
}

// Возвращает вектор сырых указателей на все заболевания
std::vector<Disease*> DiseaseDirectory::getAllDiseases() const {
    std::vector<Disease*> result;
    for (const auto& disease : diseases) {
        result.push_back(disease.get());
    }
    return result;
}

// Возвращает количество заболеваний в справочнике
int DiseaseDirectory::getDiseaseCount() const {
    return diseases.size();
}