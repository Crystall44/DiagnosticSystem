#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>

// Интерфейсы
#include "IDiagnosticAlgorithm.h"
#include "ISymptomChecker.h"

// Алгоритмы
#include "SimpleDiagnostic.h"
#include "WeightedDiagnostic.h"
#include "BayesianDiagnostic.h"

// Проверки
#include "CriticalSymptomChecker.h"
#include "EpidemiologicalChecker.h"

// Пациенты
#include "RegularPatient.h"
#include "ChronicPatient.h"

// Справочник
#include "DiseaseDirectory.h"
#include "Disease.h"

// Прием
#include "Appointment.h"

// Результаты
#include "DiagnosisResult.h"
#include "DiagnosisCandidate.h"

// Движок
#include "DiagnosticEngine.h"

// Прокси
#include "DiagnosticCacheProxy.h"

#include "MarkdownDecorator.h"
#include "HeaderDecorator.h"
#include "LegacyDrugDB.h"
#include "DrugAdapter.h"
#include "Symptom.h"

// Вспомогательная функция для вывода результатов
void printDiagnosisResult(const DiagnosisResult* result, const std::string& scenarioName) {
    std::cout << "Результат диагностики: " << scenarioName << std::endl;

    // Получаем список кандидатов (возможных диагнозов) из результата
    auto candidates = result->getCandidates();

    // Если список пуст - значит ничего не нашли
    if (candidates.empty()) {
        std::cout << "Диагноз не найден.\n";
    }
    else {
        std::cout << "Наиболее вероятные диагнозы:\n";
        int i = 1;

        // Перебираем всех кандидатов (они уже отсортированы по убыванию вероятности)
        for (const auto& candidate : candidates) {
            // Выводим номер, название болезни и вероятность в процентах
            std::cout << i++ << ". " << candidate.getDisease()->getName() << " - вероятность: " << std::fixed << std::setprecision(2) << candidate.getProbability() * 100 << "%\n";
            
            // Выводим список симптомов, которые совпали
            std::cout << "Совпавшие симптомы: ";
            // Получаем вектор совпавших симптомов
            auto matched = candidate.getMatchedSymptoms();

            // Перебираем все совпавшие симптомы
            for (size_t j = 0; j < matched.size(); ++j) {
                std::cout << matched[j];
                if (j < matched.size() - 1) std::cout << ", ";
            }
            std::cout << "\n";
        }
    }

    auto warnings = result->getWarnings();
    if (!warnings.empty()) {
        std::cout << "\nПРЕДУПРЕЖДЕНИЯ:\n";

        // Перебираем и выводим каждое предупреждение
        for (const auto& warning : warnings) {
            std::cout << warning << "\n";
        }
    }

    // Выводим время, затраченное на диагностику (в миллисекундах)
    std::cout << "Время обработки: " << result->getProcessingTime() << " мс\n";
}

// Вспомогательная функция для создания справочника заболеваний
void setupDiseaseDirectory(DiseaseDirectory& directory) {
    // ОРВИ
    auto orvi = std::make_unique<Disease>("ОРВИ", "J00");
    orvi->addSymptom("кашель", 0.5f);
    orvi->addSymptom("насморк", 0.9f);
    orvi->addSymptom("температура 38", 0.6f);
    orvi->addSymptom("головная боль", 0.4f);
    orvi->addSymptom("слабость", 0.5f);
    directory.addDisease(std::move(orvi));

    // Ангина
    auto angina = std::make_unique<Disease>("Ангина", "J03");
    angina->addSymptom("боль в горле", 0.9f);
    angina->addSymptom("температура 39", 0.9f);
    angina->addSymptom("головная боль", 0.4f);
    angina->addSymptom("слабость", 0.6f);
    angina->addSymptom("увеличение лимфоузлов", 0.8f);
    directory.addDisease(std::move(angina));

    // Грипп
    auto flu = std::make_unique<Disease>("Грипп", "J11");
    flu->addSymptom("температура 39", 0.9f);
    flu->addSymptom("озноб", 0.8f);
    flu->addSymptom("головная боль", 0.7f);
    flu->addSymptom("ломота в теле", 0.8f);
    flu->addSymptom("кашель", 0.6f);
    flu->addSymptom("слабость", 0.8f);
    directory.addDisease(std::move(flu));

    // Пневмония
    auto pneumonia = std::make_unique<Disease>("Пневмония", "J18");
    pneumonia->addSymptom("кашель", 0.9f);
    pneumonia->addSymptom("температура 39", 0.8f);
    pneumonia->addSymptom("одышка", 0.9f);
    pneumonia->addSymptom("боль в груди", 0.5f);
    pneumonia->addSymptom("слабость", 0.6f);
    directory.addDisease(std::move(pneumonia));

    // Аллергия
    auto allergy = std::make_unique<Disease>("Аллергия", "T78");
    allergy->addSymptom("насморк", 0.7f);
    allergy->addSymptom("чихание", 0.8f);
    allergy->addSymptom("зуд в глазах", 0.9f);
    allergy->addSymptom("слезотечение", 0.8f);
    allergy->addSymptom("сыпь", 0.5f);
    directory.addDisease(std::move(allergy));

    // Ковид
    auto covid = std::make_unique<Disease>("COVID-19", "U07.1");
    covid->addSymptom("кашель", 0.8f);
    covid->addSymptom("лихорадка", 0.8f);
    covid->addSymptom("потеря обоняния", 0.6f);       
    covid->addSymptom("потеря вкуса", 0.5f);
    covid->addSymptom("слабость", 0.9f);
    covid->addSymptom("одышка", 0.3f);                 
    covid->addSymptom("головная боль", 0.5f);
    directory.addDisease(std::move(covid));
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::cout << "МЕДИЦИНСКАЯ СИСТЕМА ДИАГНОСТИКИ\n\n";

    // 1. Создаем справочник заболеваний
    std::cout << "1. Инициализация справочника заболеваний...\n";
    DiseaseDirectory directory;
    setupDiseaseDirectory(directory);
    std::cout << "   Загружено заболеваний: " << directory.getDiseaseCount() << "\n\n";

    // 2. Создаем пациентов
    std::cout << "2. Регистрация пациентов...\n";
    auto patient1 = std::make_unique<RegularPatient>("Иванов И.И.", 35, "муж");
    patient1->addAllergy("пенициллин");

    auto patient2 = std::make_unique<ChronicPatient>("Петрова А.С.", 42, "жен");
    patient2->addChronicDisease("бронхиальная астма", "2018");
    patient2->addAllergy("аспирин");

    auto patient3 = std::make_unique<RegularPatient>("Сидоров П.Р.", 28, "муж");

    std::cout << "Зарегистрировано пациентов: 3\n\n";

    // 3. Создаем приемы с разными симптомами
    std::cout << "3. Создание приемов...\n";

    // Прием 1 - симптомы ОРВИ
    Appointment apt1(patient1.get());
    apt1.addSymptom("кашель");
    apt1.addSymptom("насморк");
    apt1.addSymptom("температура 38");
    apt1.addSymptom("слабость");

    // Прием 2 - симптомы ангины
    Appointment apt2(patient2.get());
    apt2.addSymptom("боль в горле");
    apt2.addSymptom("температура 39");
    apt2.addSymptom("головная боль");
    apt2.addSymptom("слабость");

    // Прием 3 - симптомы гриппа
    Appointment apt3(patient3.get());
    apt3.addSymptom("температура 39");
    apt3.addSymptom("головная боль");
    apt3.addSymptom("ломота в теле");
    apt3.addSymptom("кашель");
    apt3.addSymptom("слабость");

    // Прием 4 - для демонстрации кэша (повтор симптомов приема 1)
    Appointment apt4(patient1.get());
    apt4.addSymptom("кашель");
    apt4.addSymptom("насморк");
    apt4.addSymptom("температура 38");
    apt4.addSymptom("слабость");

    std::cout << "Создано приемов: 4\n\n";

    // Сценарий 1: Базовый ФАП (SimpleDiagnostic)
    std::cout << "Сценарий 1: Базовый ФАП (фельдшер-стажёр)\n";

    // Реальный движок
    IDiagnosticEngine* realEngine = new DiagnosticEngine(
        std::make_unique<SimpleDiagnostic>(0.3f),
        &directory
    );
    realEngine->addChecker(std::make_unique<CriticalSymptomChecker>());

    //Прокси создается также
    IDiagnosticEngine* proxy = new DiagnosticCacheProxy(
        std::make_unique<SimpleDiagnostic>(0.3f),  // те же параметры
        &directory,
        100  // дополнительный параметр для размера кэша
    );
    proxy->addChecker(std::make_unique<CriticalSymptomChecker>());  // так же добавляем проверки
    // Используем оба одинаково
    // 
    // Используем прокси как обычный движок
    auto result1 = realEngine->diagnose(&apt1);
    printDiagnosisResult(result1.get(), "\nПациент Иванов (симптомы ОРВИ)");

    auto result2 = proxy->diagnose(&apt2);
    printDiagnosisResult(result2.get(), "\nПациент Петрова (симптомы ангины)");

    // Сценарий 2: Опытный фельдшер 
    std::cout << "\nСценарий 2: Опытный фельдшер\n";

    IDiagnosticEngine* proxy1 = new DiagnosticCacheProxy(
        std::make_unique<WeightedDiagnostic>(true),  
        &directory,
        100 
    );
    proxy1->addChecker(std::make_unique<CriticalSymptomChecker>());
    auto epiChecker = std::make_unique<EpidemiologicalChecker>();
    epiChecker->setSeason("winter");
    proxy1->addChecker(std::move(epiChecker));

    // Диагностика третьего пациента (грипп)
    auto result3 = proxy1->diagnose(&apt3);
    printDiagnosisResult(result3.get(), "\nПациент Сидоров (симптомы гриппа)");

    // Демонстрация работы прокси (кэша)
    // Повторная диагностика с теми же симптомами (должно быть попадание в кэш)
    std::cout << "\nПовторная диагностика пациента Иванов (те же симптомы)...\n";
    auto resultCached = proxy->diagnose(&apt1);  // apt1 - это Иванов с симптомами ОРВИ
    printDiagnosisResult(resultCached.get(), "Повторно (из кэша)");

    // Сценарий 3: Консультация в ЦРБ 
    std::cout << "\nСценарий 3: Консультация в ЦРБ\n";

    // Создаем байесовский алгоритм с настройкой априорных вероятностей
    auto bayesian = std::make_unique<BayesianDiagnostic>(0.01f);
    bayesian->setPriorProbability("ОРВИ", 0.3f);
    bayesian->setPriorProbability("Грипп", 0.2f);
    bayesian->setPriorProbability("Ангина", 0.15f);
    bayesian->setPriorProbability("Пневмония", 0.05f);

    IDiagnosticEngine* proxy2 = new DiagnosticCacheProxy(
        std::move(bayesian),  
        &directory,
        100 
    );
    proxy2->addChecker(std::make_unique<CriticalSymptomChecker>());
    auto epiChecker1 = std::make_unique<EpidemiologicalChecker>();
    epiChecker1->setSeason("winter");
    proxy2->addChecker(std::move(epiChecker1));

    // Диагностика сложного случая
    auto resultBayes = proxy2->diagnose(&apt3);
    printDiagnosisResult(resultBayes.get(), "\nПациент Сидоров (байесовский алгоритм)");

    std::cout << "\n\n=== ДЕМОНСТРАЦИЯ НОВЫХ ПАТТЕРНОВ ===\n\n";
    // 1. COMPOSITE — создание группы симптомов с классами Symptom
    std::cout << "1. Composite: создание группы симптомов\n";
    auto respiratoryGroup = std::make_unique<SymptomGroup>("Респираторные симптомы");
    respiratoryGroup->addSymptom(Symptom("кашель", "сухой кашель", 6));
    respiratoryGroup->addSymptom(Symptom("насморк", "обильные выделения", 4));
    respiratoryGroup->addSymptom(Symptom("боль в горле", "острая боль", 5));

    auto intoxicationGroup = std::make_unique<SymptomGroup>("Симптомы интоксикации");
    intoxicationGroup->addSymptom(Symptom("температура 38", "субфебрильная", 7));
    intoxicationGroup->addSymptom(Symptom("слабость", "общая слабость", 5));
    intoxicationGroup->addSymptom(Symptom("головная боль", "пульсирующая", 4));

    std::cout << "\nСтруктура групп симптомов:\n";
    std::cout << "Респираторные симптомы:\n";
    for (const auto& sym : respiratoryGroup->getAllSymptoms()) {
        std::cout << "  " << sym << "\n";
    }
    std::cout << "\nСимптомы интоксикации:\n";
    for (const auto& sym : intoxicationGroup->getAllSymptoms()) {
        std::cout << "  " << sym << "\n";
    }

    auto allSymptoms = std::make_unique<SymptomGroup>("Все симптомы");
    allSymptoms->add(std::move(respiratoryGroup));
    allSymptoms->add(std::move(intoxicationGroup));

    std::cout << "Группа 'Все симптомы' содержит:\n";
    for (const auto& sym : allSymptoms->getAllSymptoms()) {
        std::cout << "  - " << sym << "\n";
    }

    // 2. Демонстрация Iterator
    std::cout << "\n2. Iterator: обход симптомов через итератор\n";
    Appointment demoApp(patient1.get());
    demoApp.setSymptomGroup(allSymptoms.get(), false); 

    SymptomsIterator* it = demoApp.createIterator();
    std::cout << "Симптомы через итератор:\n";
    while (it->hasNext()) {
        std::cout << "  - " << it->next() << "\n";
    }
    delete it;

    // 3. DECORATOR — два декоратора, один оборачивает другой
    std::cout << "\n3. Decorator: форматирование отчёта в Markdown\n";
    auto diagnosis = proxy1->diagnose(&demoApp);

    // Создаём базовый отчёт (DiagnosisResult)
    auto baseReport = std::make_unique<DiagnosisResult>(*diagnosis);

    // Оборачиваем в MarkdownDecorator
    auto mdReport = std::make_unique<MarkdownDecorator>(baseReport.get());

    // Оборачиваем Markdown в HeaderDecorator (добавляем заголовок сверху)
    auto withHeader = std::make_unique<HeaderDecorator>(
        std::move(mdReport),
        "# ОТЧЁТ ФАПА\n**Сформирован автоматически**\n"
        );

    // Выводим результат с вложенными декораторами
    std::cout << withHeader->generate() << "\n";

    // 4. ADAPTER — преобразует LegacyDrugDB в IDrugSource (возвращает Disease)
    std::cout << "\n4. Adapter: интеграция с внешней системой лекарств\n";
    LegacyDrugDB legacyDB;
    DrugAdapter drugAdapter(&legacyDB);

    // Получаем все лекарства как заболевания
    auto drugs = drugAdapter.getAllDrugs();
    std::cout << "Лекарства из внешней системы (как Disease):\n";
    for (const auto& drug : drugs) {
        std::cout << "  - " << drug->getName() << " (код: " << drug->getIcd10Code() << ")\n";
        auto symptoms = drug->getSymptoms();
        if (!symptoms.empty()) {
            std::cout << "    Противопоказания: ";
            for (const auto& sw : symptoms) {
                std::cout << sw.getSymptomName() << " ";
            }
            std::cout << "\n";
        }
    }

    // Поиск конкретного лекарства
    std::cout << "\nПоиск лекарства 'Аспирин':\n";
    Disease* aspirin = drugAdapter.findDrugByName("Аспирин");
    if (aspirin) {
        std::cout << "  Найдено: " << aspirin->getName() << "\n";
        std::cout << "  Противопоказания: ";
        for (const auto& sw : aspirin->getSymptoms()) {
            std::cout << sw.getSymptomName() << " ";
        }
        std::cout << "\n";
    }

    // Проверка противопоказаний
    std::cout << "\nПроверка противопоказаний для пациента:\n";
    auto contraindications = drugAdapter.getContraindicationsAsSymptoms("Аспирин");
    std::cout << "Противопоказания к Аспирину как симптомы:\n";
    for (const auto& symptom : contraindications) {
        std::cout << "  - " << symptom.toString() << "\n";
    }

    // Проверка аллергии
    std::cout << "\nПроверка аллергии:\n";
    bool hasAllergy = false;
    for (const auto& allergy : patient1->getAllergies()) {
        for (const auto& contra : contraindications) {
            if (contra.getName() == allergy) {
                hasAllergy = true;
                std::cout << "  ВНИМАНИЕ! У пациента аллергия на " << allergy
                    << " (противопоказание к Аспирину)\n";
            }
        }
    }
    if (!hasAllergy) {
        std::cout << "  Противопоказаний не найдено, Аспирин можно назначать\n";
    }

    // Очистка (allSymptoms уже в unique_ptr, удалится автоматически)
    std::cout << "\n\nРабота системы завершена успешно\n";

    std::cin.get();
    return 0;
}