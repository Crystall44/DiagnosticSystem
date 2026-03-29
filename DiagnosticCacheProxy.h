#pragma once
#include <map>
#include <string>
#include <memory>
#include "IDiagnosticEngine.h"
#include "DiagnosticEngine.h"
#include "DiagnosisResult.h"

// Структура для хранения записи в кэше
struct CacheEntry {
    std::unique_ptr<DiagnosisResult> result; // результат диагностики
    std::time_t timestamp; // время создания записи

    // Конструктор
    CacheEntry(std::unique_ptr<DiagnosisResult> res)
        : result(std::move(res)), timestamp(std::time(nullptr)) {}
};

// Прокси для кэширования результатов диагностики (паттерн Proxy)
class DiagnosticCacheProxy : public IDiagnosticEngine {
private:
    std::unique_ptr<IDiagnosticEngine> realEngine; // принимает любой IDiagnosticEngine
    std::map<std::string, std::unique_ptr<CacheEntry>> cache; // кэш: ключ -> результат
    int hits; // количество попаданий в кэш
    int misses; // количество промахов
    size_t maxCacheSize; // максимальный размер кэша

public:
    DiagnosticCacheProxy(std::unique_ptr<IDiagnosticEngine> engine, size_t maxSize = 100);

    ~DiagnosticCacheProxy() = default;

    // Методы интерфейса — вызывают реальный движок с добавлением кэширования
    std::unique_ptr<DiagnosisResult> diagnose(const Appointment* appointment) override;
    void setAlgorithm(std::unique_ptr<IDiagnosticAlgorithm> algo) override;
    void addChecker(std::unique_ptr<ISymptomChecker> checker) override;
    void removeAllCheckers() override;

    // Очистить кэш
    void clearCache();

    // Получить статистику
    int getHits() const;
    int getMisses() const;
    float getHitRate() const; // процент попаданий
    size_t getCacheSize() const; // текущий размер кэша

private:
    // Сгенерировать ключ для кэша на основе данных приема
    std::string generateCacheKey(const Appointment* appointment) const;

    // Удалить старые записи при переполнении кэша
    void cleanOldCache();
};
