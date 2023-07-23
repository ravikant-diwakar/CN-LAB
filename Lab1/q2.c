#include <stdio.h>
#include <string.h>

struct dob {
    int day;
    int month;
    int year;
};

struct student_info {
    int roll_no;
    char name[50];
    float CGPA;
    struct dob age;
};

void printStudentInfoByValue(struct student_info student);
void printStudentInfoByAddress(struct student_info *student);

int main() {
    struct student_info student1;
    student1.roll_no = 1499;
    strcpy(student1.name, "Ravikant");
    student1.CGPA = 9.2;
    student1.age.day = 10;
    student1.age.month = 05;
    student1.age.year = 2002;

    printf("student information using call-by-value:\n");
    printStudentInfoByValue(student1);

    printf("\nstudent information using call-by-address:\n");
    printStudentInfoByAddress(&student1);

    return 0;
}

void printStudentInfoByValue(struct student_info student) {
    printf("Roll No: %d\n", student.roll_no);
    printf("Name: %s\n", student.name);
    printf("CGPA: %.2f\n", student.CGPA);
    printf("Date of Birth: %d/%d/%d\n", student.age.day, student.age.month, student.age.year);
}

void printStudentInfoByAddress(struct student_info *student) {
    printf("Roll No: %d\n", student->roll_no);
    printf("Name: %s\n", student->name);
    printf("CGPA: %.2f\n", student->CGPA);
    printf("Date of Birth: %d/%d/%d\n", student->age.day, student->age.month, student->age.year);
}

