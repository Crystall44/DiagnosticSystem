#include "HeaderDecorator.h"
#include <sstream>
#include <iomanip>

HeaderDecorator::HeaderDecorator(std::unique_ptr<IReport> report, const std::string& headerText)
    : wrapped(std::move(report)), header(headerText) {}

std::string HeaderDecorator::generate() const {
    // Сначала заголовок, затем отчёт
    return header + "\n" + wrapped->generate();
}