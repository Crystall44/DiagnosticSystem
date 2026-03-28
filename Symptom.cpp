#include "Symptom.h"
#include <sstream>

Symptom::Symptom(const std::string& name, const std::string& desc, int severity)
    : name(name), description(desc), severity(severity) {}

std::string Symptom::getName() const {
    return name;
}

std::string Symptom::getDescription() const {
    return description;
}

int Symptom::getSeverity() const {
    return severity;
}

void Symptom::setSeverity(int s) {
    severity = s;
}

std::string Symptom::toString() const {
    std::stringstream ss;
    ss << name;
    if (severity > 0) {
        ss << " (тяжесть: " << severity << "/10)";
    }
    return ss.str();
}

bool Symptom::operator==(const Symptom& other) const {
    return name == other.name;
}

bool Symptom::operator==(const std::string& otherName) const {
    return name == otherName;
}