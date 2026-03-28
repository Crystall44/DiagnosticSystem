#include "Appointment.h"
#include "SymptomGroup.h"
#include <algorithm> // нужен для std::find

// Конструктор
Appointment::Appointment(Patient* patient)
    : patient(patient), symptomsRoot(nullptr), ownsRoot(true) {
    date = std::time(nullptr);
    // Создаём корневую группу по умолчанию
    symptomsRoot = new SymptomGroup("root");
}

// Деструктор
Appointment::~Appointment() {
    if (ownsRoot && symptomsRoot) {
        delete symptomsRoot;
    }
}

void Appointment::setSymptomGroup(SymptomComponent* root, bool takeOwnership) {
    if (ownsRoot && symptomsRoot) {
        delete symptomsRoot;
    }
    symptomsRoot = root;
    ownsRoot = takeOwnership;
}

SymptomComponent* Appointment::getSymptomsRoot() const {
    return symptomsRoot;
}

SymptomsIterator* Appointment::createIterator() const {
    return new SymptomsIterator(symptomsRoot);
}

void Appointment::setDate(std::time_t newDate) {
    date = newDate;
}

std::time_t Appointment::getDate() const {
    return date;
}

Patient* Appointment::getPatient() const {
    return patient;
}

void Appointment::setPatient(Patient* newPatient) {
    patient = newPatient;
}

void Appointment::addSymptom(const std::string& symptom) {
    if (!symptomsRoot) {
        symptomsRoot = new SymptomGroup("root");
        ownsRoot = true;
    }
    // Пытаемся добавить в корневую группу как SymptomGroup
    SymptomGroup* rootGroup = dynamic_cast<SymptomGroup*>(symptomsRoot);
    if (rootGroup) {
        rootGroup->addSymptom(symptom);
    }
}

void Appointment::removeSymptom(const std::string& symptom) {
    if (symptomsRoot) {
        // Пытаемся удалить симптом из корневой группы
        SymptomGroup* rootGroup = dynamic_cast<SymptomGroup*>(symptomsRoot);
        if (rootGroup) {
            rootGroup->remove(symptom);  // SymptomGroup имеет метод remove(string)
        }
    }
}

std::vector<std::string> Appointment::getSymptoms() const {
    if (!symptomsRoot) {
        return {};
    }
    return symptomsRoot->getAllSymptoms();
}