#pragma once
#include <string>
#include <vector>

// Базовый класс пациента (хранит историю болезней и аллергии)
class Patient {
protected:
    std::string name; // имя пациента
    int age; // возраст
    std::string gender; // пол
    std::vector<std::string> medicalHistory; // история болезней
    std::vector<std::string> allergies; // аллергии

public:
    Patient(const std::string& name, int age, const std::string& gender);
    virtual ~Patient() {}

    // Геттеры для базовой информации
    virtual std::string getName() const;
    virtual int getAge() const;
    virtual std::string getGender() const;

    // Работа с историей болезней
    virtual std::vector<std::string> getMedicalHistory() const;
    virtual void addToMedicalHistory(const std::string& record);

    // Работа с аллергиями
    virtual std::vector<std::string> getAllergies() const;
    virtual void addAllergy(const std::string& allergy);

    // Получить хронические заболевания (переопределяется в наследниках)
    virtual std::vector<std::string> getChronicDiseases() const;

    // Получить информацию о пациенте (для вывода)
    virtual std::string getPatientInfo() const;
};
