#include "SymptomGroup.h"
#include <algorithm>
#include <memory>

// Класс для отдельного симптома (лист)
class SymptomLeaf : public SymptomComponent {
private:
    Symptom symptom;

public:
    SymptomLeaf(const Symptom& sym) : symptom(sym) {}

    std::vector<std::string> getAllSymptoms() const override {
        return { symptom.getName() };  // возвращаем только имя
    }

    std::vector<std::string> getAllSymptomNames() const override {
        return { symptom.getName() };
    }

    std::string getName() const override {
        return symptom.getName();
    }

    bool isLeaf() const override {
        return true;
    }
};

// Реализация SymptomGroup
SymptomGroup::SymptomGroup(const std::string& groupName) : name(groupName) {}

std::vector<std::string> SymptomGroup::getAllSymptoms() const {
    std::vector<std::string> result;
    for (const auto& child : children) {
        auto childSymptoms = child->getAllSymptoms();
        result.insert(result.end(), childSymptoms.begin(), childSymptoms.end());
    }
    return result;
}

std::vector<std::string> SymptomGroup::getAllSymptomNames() const {
    std::vector<std::string> result;
    for (const auto& child : children) {
        auto childNames = child->getAllSymptomNames();
        result.insert(result.end(), childNames.begin(), childNames.end());
    }
    return result;
}

void SymptomGroup::add(std::unique_ptr<SymptomComponent> component) {
    children.push_back(std::move(component));
}

void SymptomGroup::remove(const std::string& name) {
    auto it = std::remove_if(children.begin(), children.end(),
        [&name](const std::unique_ptr<SymptomComponent>& child) {
            return child->getName() == name;
        });
    children.erase(it, children.end());
}

std::string SymptomGroup::getName() const {
    return name;
}
void SymptomGroup::addSymptom(const Symptom& symptom) {
    children.push_back(std::make_unique<SymptomLeaf>(symptom));
}