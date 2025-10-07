//Alona Nicolau 249664

#include "Student.hpp"
#include <stdexcept>

Student::Student() = default;

Student::Student(const std::string& CPF, const std::string& id, const std::string& name, const std::string& address, const unsigned& day, const unsigned& month, const unsigned& year)
{
    if (day < 1 || day > 31)
        throw std::invalid_argument("Dia invalido!");
    if (month < 1 || month > 12)
        throw std::invalid_argument("Mes invalido!");

    this->CPF = CPF;
    this->id = id;
    this->name = name;
    this->address = address;
    this->enrollmentDate.tm_mday = day;
    this->enrollmentDate.tm_mon = month;
    this->enrollmentDate.tm_year = year;
}

std::string Student::getCPF() const {
    return this->CPF;
}

std::string Student::getId() const {
    return this->id;
}

std::string Student::getName() const {
    return this->name;
}

std::string Student::getAddress() const {
    return this->address;
}

struct tm Student::getDate() const {
    return this->enrollmentDate;
}

int Student::getCreditHours() const {
    return this->creditHours;
}

void Student::addCredits(int credits) {
    this->creditHours += credits;
}

nlohmann::json Student::getJson() const{
    nlohmann::json student = {
        {"id", this->id},  
        {"name", this->name}, 
        {"CPF", this->CPF}, 
        {"address", this->address}, 
        {"dayDate", this->enrollmentDate.tm_mday},
        {"monthDate", this->enrollmentDate.tm_mon},
        {"yearDate", this->enrollmentDate.tm_year}, 
        {"credits", this->creditHours}   
    };

    return student;
}

void Student::show()
{
    std::cout<<"ID: "<<this->name<<std::endl;
    std::cout<<"CPF: "<<this->name<<std::endl;
    std::cout<<"Nome: "<<this->name<<std::endl;
    std::cout<<"Creditos: "<<this->name<<std::endl;
    std::cout<<"Endereco: "<<this->name<<std::endl;
    std::cout<<"Data de Ingresso: "<<this->enrollmentDate.tm_mday<<"/"<<this->enrollmentDate.tm_mon<<"/"<<enrollmentDate.tm_year<<std::endl;

}
