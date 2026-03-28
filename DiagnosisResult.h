#pragma once
#include <vector>
#include <string>
#include <ctime>
#include "DiagnosisCandidate.h"
#include "IReport.h"

// –езультат диагностики - содержит список кандидатов и предупреждени€
class DiagnosisResult : public IReport {
private:
    std::vector<DiagnosisCandidate> candidates; // список веро€тных диагнозов
    std::vector<std::string> warnings; // предупреждени€ от проверок
    std::time_t timestamp; // врем€ проведени€ диагностики
    int processingTimeMs; // врем€ выполнени€ в миллисекундах

public:
    DiagnosisResult();

    // ƒобавить кандидата в результаты
    void addCandidate(const DiagnosisCandidate& candidate);

    // ѕолучить всех кандидатов
    std::vector<DiagnosisCandidate> getCandidates() const;

    // ѕолучить N лучших кандидатов (по веро€тности)
    std::vector<DiagnosisCandidate> getTopCandidates(int count) const;

    // ѕолучить лучшего кандидата (с максимальной веро€тностью)
    DiagnosisCandidate getBestCandidate() const;

    // ƒобавить предупреждение
    void addWarning(const std::string& warning);

    // ѕолучить все предупреждени€
    std::vector<std::string> getWarnings() const;

    // ѕроверить, есть ли предупреждени€
    bool hasWarnings() const;

    void setTimestamp(std::time_t time);
    std::time_t getTimestamp() const;

    void setProcessingTime(int ms);
    int getProcessingTime() const;

    // ќтсортировать кандидатов по веро€тности (от большей к меньшей)
    void sortCandidatesByProbability();

    std::string generate() const override;
};
