#pragma once
#include "IReport.h"
#include "DiagnosisResult.h"

// Декоратор для форматирования отчёта в Markdown
class MarkdownDecorator : public IReport {
private:
    const DiagnosisResult* result;

public:
    MarkdownDecorator(const DiagnosisResult* res);
    std::string generate() const override;
};