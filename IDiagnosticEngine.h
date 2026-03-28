#pragma once
#include <memory>
#include "Appointment.h"
#include "DiagnosisResult.h"
#include "IDiagnosticAlgorithm.h"
#include "ISymptomChecker.h"

// Интерфейс для всех диагностических движков
class IDiagnosticEngine {
public:
    virtual ~IDiagnosticEngine() = default;

    // Основной метод диагностики
    virtual std::unique_ptr<DiagnosisResult> diagnose(const Appointment* appointment) = 0;

    // Методы для настройки
    virtual void setAlgorithm(std::unique_ptr<IDiagnosticAlgorithm> algo) = 0;
    virtual void addChecker(std::unique_ptr<ISymptomChecker> checker) = 0;
    virtual void removeAllCheckers() = 0;
};