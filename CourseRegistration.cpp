//Alona Nicolau 249664

#include "CourseRegistration.hpp"

CourseRegistration::CourseRegistration() = default;

CourseRegistration::CourseRegistration(const char* id, const char* studentId, int credit, double grade)
    : creditHours(credit), grade(grade) 
    {
        strcpy(this->courseId, id);
        strcpy(this->studentId, studentId);
    }

const char* CourseRegistration::getId() {
    return this->courseId;
}

const char* CourseRegistration::getStudentId() {
    return this->studentId;
}

int CourseRegistration::getCreditHours() {
    return this->creditHours;
}

double CourseRegistration::getGrade() {
    return this->grade;
}

void CourseRegistration::setId(const char* id) {
    strcpy(this->courseId, id);
}

void CourseRegistration::setStudentId(const char* id) {
    strcpy(this->studentId, id);
}

void CourseRegistration::setCreditHours(int creditHours) {
    this->creditHours = creditHours;
}

void CourseRegistration::setGrade(double grade) {
    this->grade = grade;
}

nlohmann::json CourseRegistration::getJson() const{
    nlohmann::json courseRegister = {
        {"id", this->courseId},
        {"studentId", this->studentId},
        {"credit", this->creditHours},
        {"grade", this->grade}
    };
    
    return courseRegister;
}

void CourseRegistration::show()
{
    std::cout<<"ID Curso: "<<this->courseId<<std::endl;
    std::cout<<"ID Estudante: "<<this->studentId<<std::endl;
    std::cout<<"Creditos: "<<this->creditHours<<std::endl;
    std::cout<<"Nota: "<<this->grade<<std::endl;
}

