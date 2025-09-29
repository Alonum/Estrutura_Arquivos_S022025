//Alona Nicolau 249664

#ifndef COURSEREGISTRATION_HPP
#define COURSEREGISTRATION_HPP

#include <iostream>
#include <string>
#include <cstring>
#include "nlohmann/json.hpp"

class CourseRegistration {
private:
    char courseId[7];
    char studentId[7];
    int creditHours;
    double grade;

public:
    CourseRegistration();
    CourseRegistration(const char* id, const char* studentId, int credit, double grade);
    
    const char* getId();
    const char* getStudentId();
    int getCreditHours();
    double getGrade();
    nlohmann::json getJson() const;


    
    void setId(const char* id);
    void setStudentId(const char* id);
    void setCreditHours(int creditHours);
    void setGrade(double grade);
    void show();

};

#endif
