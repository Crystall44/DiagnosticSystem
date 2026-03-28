#pragma once
#include "IReport.h"
#include <memory>

class HeaderDecorator : public IReport {
private:
    std::unique_ptr<IReport> wrapped;
    std::string header;

public:
    HeaderDecorator(std::unique_ptr<IReport> report, const std::string& headerText);

    std::string generate() const override;
};