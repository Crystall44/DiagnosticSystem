#pragma once
#include "IIterator.h"
#include "SymptomComponent.h"
#include <vector>
#include <string>

// Итератор для обхода симптомов
class SymptomsIterator : public IIterator<std::string> {
private:
    std::vector<std::string> flatSymptoms;
    size_t currentIndex;

public:
    // Конструктор — принимает корневую группу симптомов
    SymptomsIterator(SymptomComponent* root);

    bool hasNext() const override;
    std::string next() override;
    void reset() override;

    // Возвращает общее количество симптомов (дополнительный метод)
    size_t getSize() const;
};