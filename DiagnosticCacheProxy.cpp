#include "DiagnosticCacheProxy.h"
#include <sstream>
#include <algorithm>

// Принимает уникальный указатель на движок и максимальный размер кэша
DiagnosticCacheProxy::DiagnosticCacheProxy(
    std::unique_ptr<IDiagnosticAlgorithm> algo,
    DiseaseDirectory* dir,
    size_t maxSize
    //прокси создаёт внутри себя реальный объект
) : hits(0), misses(0), maxCacheSize(maxSize) {
    realEngine = new DiagnosticEngine(std::move(algo), dir);
}

// Диагностика с использованием кэша
std::unique_ptr<DiagnosisResult> DiagnosticCacheProxy::diagnose(const Appointment* appointment) {
    // Генерируем уникальный ключ для этого приёма (на основе пациента и симптомов)
    std::string key = generateCacheKey(appointment);

    // Ищем в кэше
    auto it = cache.find(key);
    if (it != cache.end()) {
        hits++;
        // Возвращаем копию результата из кэша
        return std::make_unique<DiagnosisResult>(*(it->second->result));
    }

    misses++; // увеличиваем счётчик промахов

    auto result = realEngine->diagnose(appointment);

    // Сохраняем в кэш
    cache[key] = std::make_unique<CacheEntry>(std::move(result));

    // Если кэш переполнен, удаляем старые записи
    if (cache.size() > maxCacheSize) {
        cleanOldCache(); // удаляем самую старую запись
    }

    // Возвращаем результат (создаем копию для сохранения оригинала в кэше)
    // Так мы отдаём копию, а оригинал остаётся в кэше
    return std::make_unique<DiagnosisResult>(*(cache[key]->result));
}

void DiagnosticCacheProxy::setAlgorithm(std::unique_ptr<IDiagnosticAlgorithm> algo) {
    // делегируем реальному объекту
    realEngine->setAlgorithm(std::move(algo));
    clearCache();  // при смене алгоритма кэш становится невалидным
}

void DiagnosticCacheProxy::addChecker(std::unique_ptr<ISymptomChecker> checker) {
    // делегируем реальному объекту
    realEngine->addChecker(std::move(checker));
    clearCache();  // при смене проверок кэш становится невалидным
}

void DiagnosticCacheProxy::removeAllCheckers() {
    realEngine->removeAllCheckers();
    clearCache();
}

// Полностью очищает кэш и сбрасывает статистику
void DiagnosticCacheProxy::clearCache() {
    cache.clear(); // удаляем все записи
    hits = 0; // сбрасываем попадания
    misses = 0; // сбрасываем промахи
}

// Возвращает количество попаданий в кэш
int DiagnosticCacheProxy::getHits() const {
    return hits;
}

// Возвращает количество промахов
int DiagnosticCacheProxy::getMisses() const {
    return misses;
}

// Возвращает процент попаданий (отношение hits к общему числу запросов)
float DiagnosticCacheProxy::getHitRate() const {
    int total = hits + misses;
    return (total == 0) ? 0.0f : static_cast<float>(hits) / total;
}

// Возвращает текущий размер кэша (количество записей)
size_t DiagnosticCacheProxy::getCacheSize() const {
    return cache.size();
}

// Генерирует уникальный строковый ключ для приёма
// Ключ используется для поиска в кэше
std::string DiagnosticCacheProxy::generateCacheKey(const Appointment* appointment) const {
    // Создаём строковый поток для сборки ключа
    std::stringstream ss;

    // Получаем пациента из приёма
    auto patient = appointment->getPatient();
    // Если пациент есть, добавляем его имя в ключ
    if (patient) {
        ss << patient->getName() << "_"; // например: "Иванов_"
    }

    // Получаем список симптомов
    auto symptoms = appointment->getSymptoms();
    // Добавляем каждый симптом в ключ
    for (const auto& symptom : symptoms) {
        ss << symptom << "_"; // например: "кашель_температура_"
    }

    // Возвращаем собранную строку
    // Пример ключа: "Иванов_кашель_температура_"
    return ss.str();
}

// Удаляет самую старую запись из кэша (при переполнении)
void DiagnosticCacheProxy::cleanOldCache() {
    // Если кэш пуст, нечего удалять
    if (cache.empty()) return;

    // Предполагаем, что первый элемент - самый старый
    auto oldest = cache.begin();
    // Проходим по всем элементам кэша
    for (auto it = cache.begin(); it != cache.end(); ++it) {
        // Сравниваем timestamp (время создания записи)
        // Если нашли элемент с меньшим (более старым) временем
        if (it->second->timestamp < oldest->second->timestamp) {
            oldest = it; // запоминаем его как самый старый
        }
    }
    // Удаляем самую старую запись
    cache.erase(oldest);
}