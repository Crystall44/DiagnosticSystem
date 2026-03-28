#pragma once
#include <string>

// Интерфейс для отчётов (декоратор)
class IReport {
public:
    virtual ~IReport() = default;
    virtual std::string generate() const = 0;
};