#include "DiagnosticEngine.h"
#include <chrono>
#include <algorithm>

// Принимает алгоритм и справочник заболеваний
// algorithm - уникальное владение (std::move)
// directory - сырой указатель (не владеет)
DiagnosticEngine::DiagnosticEngine(
    std::unique_ptr<IDiagnosticAlgorithm> algo,
    DiseaseDirectory* dir
) : algorithm(std::move(algo)), directory(dir) {}

// Заменяет текущий алгоритм новым
void DiagnosticEngine::setAlgorithm(std::unique_ptr<IDiagnosticAlgorithm> algo) {
    algorithm = std::move(algo);
}

// Возвращает сырой указатель на текущий алгоритм (без передачи владения)
IDiagnosticAlgorithm* DiagnosticEngine::getAlgorithm() const {
    return algorithm.get();
}

// Добавляет новую проверку в вектор
void DiagnosticEngine::addChecker(std::unique_ptr<ISymptomChecker> checker) {
    checkers.push_back(std::move(checker));
}

// Удаляет все проверки
void DiagnosticEngine::removeAllCheckers() {
    checkers.clear();
}

// Реальная логика диагностики (без кэширования)
std::unique_ptr<DiagnosisResult> DiagnosticEngine::diagnose(const Appointment* appointment) {
    // Засекаем время начала
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Получаем данные
    auto patient = appointment->getPatient();
    auto symptoms = appointment->getSymptoms();

    // Алгоритм делает всю диагностику
    auto result = algorithm->performDiagnosis(appointment, directory);

    // Проверки добавляют предупреждения
    for (const auto& checker : checkers) {
        auto warnings = checker->check(symptoms, patient);
        for (const auto& warning : warnings) {
            result->addWarning(warning);
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        endTime - startTime
        );
    result->setProcessingTime(duration.count());

    // Возвращаем результат (уникальное владение)
    return result;
}

