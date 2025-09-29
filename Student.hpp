//Alona Nicolau 249664

#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <string>
#include <ctime>
#include <cstring>
#include "nlohmann/json.hpp"

class Student {
private:
    std::string CPF;
    std::string id;
    std::string name;
    std::string address;
    struct tm enrollmentDate;
    int creditHours;

public:
    Student();
    Student(const std::string& CPF, const std::string& id, const std::string& name, const std::string& address, const unsigned& day, const unsigned& month, const unsigned& year);
    
    std::string getCPF() const ;
    std::string getId() const ;
    std::string getName() const ;
    std::string getAddress() const ;   
    nlohmann::json getJson() const;
    struct tm getDate() const ;
    int getCreditHours() const ;
    
    void addCredits(int credits);
};

#endif
