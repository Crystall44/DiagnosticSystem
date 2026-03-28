#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Disease.h"

// Справочник заболеваний - хранит базу болезней с симптомами и весами
class DiseaseDirectory {
private:
    std::vector<std::unique_ptr<Disease>> diseases;

public:
    // Конструктор по умолчанию (создает пустой справочник)
    DiseaseDirectory() = default;

    // Добавить новое заболевание в справочник
    void addDisease(std::unique_ptr<Disease> disease);

    // Найти заболевание по названию (вернуть указатель или nullptr)
    Disease* findDiseaseByName(const std::string& name) const;

    // Найти заболевание по коду МКБ-10
    Disease* findDiseaseByCode(const std::string& icd10Code) const;

    // Получить все заболевания (в виде сырых указателей)
    std::vector<Disease*> getAllDiseases() const;

    // Получить количество заболеваний в справочнике
    int getDiseaseCount() const;
};