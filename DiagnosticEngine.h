#pragma once
#include <memory>
#include <vector>
#include "IDiagnosticAlgorithm.h"
#include "ISymptomChecker.h"
#include "DiseaseDirectory.h"
#include "Appointment.h"
#include "DiagnosisResult.h"
#include "IDiagnosticEngine.h"

// Реальный движок, который выполняет диагностику
class DiagnosticEngine : public IDiagnosticEngine {
private:
    std::unique_ptr<IDiagnosticAlgorithm> algorithm; // текущий алгоритм
    std::vector<std::unique_ptr<ISymptomChecker>> checkers; // список проверок
    DiseaseDirectory* directory; // справочник заболеваний

public:
    DiagnosticEngine(std::unique_ptr<IDiagnosticAlgorithm> algo, DiseaseDirectory* dir);

    // Сменить алгоритм диагностики
    void setAlgorithm(std::unique_ptr<IDiagnosticAlgorithm> algo);

    // Получить текущий алгоритм
    IDiagnosticAlgorithm* getAlgorithm() const;

    void addChecker(std::unique_ptr<ISymptomChecker> checker);

    // Удалить все проверки
    void removeAllCheckers();

    // Выполнить диагностику для указанного приема
    std::unique_ptr<DiagnosisResult> diagnose(const Appointment* appointment);
};
