#pragma once
#include <string>
#include <map>
#include <vector>

// »митаци€ устаревшей внешней системы хранени€ данных о лекарствах
class LegacyDrugDB {
private:
    // —имулируем базу данных лекарств
    std::map<std::string, std::string> drugNames;      // код -> название
    std::map<std::string, std::vector<std::string>> contraindications; // название -> противопоказани€

public:
    LegacyDrugDB();

    // Ќесовместимый интерфейс - работает со специальными кодами
    std::string queryDrugData(const std::string& query);
    std::string fetchRecord(int id);

    // ѕолучить все противопоказани€ дл€ лекарства (внутренний метод)
    std::vector<std::string> getContraindicationsInternal(const std::string& drugCode);
};