#pragma once
#include "SymptomComponent.h"
#include <memory>
#include <vector>

// Группа симптомов (контейнер)
class SymptomGroup : public SymptomComponent {
private:
    std::string name;
    std::vector<std::unique_ptr<SymptomComponent>> children;

public:
    SymptomGroup(const std::string& groupName);

    std::vector<std::string> getAllSymptoms() const override;
    std::vector<std::string> getAllSymptomNames() const override;
    void add(std::unique_ptr<SymptomComponent> component) override;
    void remove(const std::string& name) override;
    std::string getName() const override;
    bool isLeaf() const override { return false; }

    // Добавить отдельный симптом
    void addSymptom(const Symptom& symptom);
};