#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Symptom.h"

// Интерфейс для компонентов симптомов (Composite)
class SymptomComponent {
public:
    virtual ~SymptomComponent() = default;

    // Получить все симптомы в плоском виде
    virtual std::vector<std::string> getAllSymptoms() const = 0;

    // Добавить компонент (для групп)
    virtual void add(std::unique_ptr<SymptomComponent> component) {}

    // Удалить компонент по имени
    virtual void remove(const std::string& name) {}

    // Получить названия симптомов (для совместимости со старым кодом)
    virtual std::vector<std::string> getAllSymptomNames() const = 0;

    // Получить имя компонента
    virtual std::string getName() const = 0;

    // Проверить, является ли листом
    virtual bool isLeaf() const = 0;

};