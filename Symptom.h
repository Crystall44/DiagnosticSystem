#pragma once
#include <string>
#include <vector>

// Класс отдельного симптома
class Symptom {
private:
    std::string name;        // название симптома
    std::string description; // описание (опционально)
    int severity;            // степень тяжести (1-10)

public:
    Symptom(const std::string& name, const std::string& desc = "", int severity = 5);

    std::string getName() const;
    std::string getDescription() const;
    int getSeverity() const;

    void setSeverity(int s);

    // Для вывода
    std::string toString() const;

    // Операторы сравнения для поиска
    bool operator==(const Symptom& other) const;
    bool operator==(const std::string& otherName) const;
};