//
// student.c
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "student.h"
#include "encrypt.h"


const char* STUFILE = "studentdata.txt";

// keeps track of the number of students in the students array
int numStudents = 0;

// holds the student pointer array
Student* students[100] = {0};


void createStudent(char* fname, char* lname, int age, int id)
{
  // createStudent dynamically creates a Student on the heap and adds that
  // student to the student array
  //  - the firstName and lastName strings should be dynamically created
  //    based on the size of the fname and lname args
  Student* st = (Student*)malloc(sizeof(Student));
  st -> firstName = (char*)malloc((strlen(fname)+1)*sizeof(char));
  st -> lastName = (char*)malloc((strlen(lname)+1)*sizeof(char));
  strcpy(st->firstName, fname);
  strcpy(st->lastName, lname);
  st -> id = id;
  students[numStudents] = st;
  numStudents = numStudents + 1;
}


void deleteStudent(Student* student)
{
  // free the memory associated with a student including the strings
  student -> id = 0;
  student -> age = 0;
  free(student -> firstName);
  free(student -> lastName);
  free(student);
}


void deleteStudents()
{
  // iterate over the students array deleting every student and setting te pointer
  // values to 0 and adjusting the numStudents to 0
  for (int i = 0; i < numStudents; i++)
  {
    deleteStudent(students[i]);
  }
  numStudents = 0;
}


void saveStudents(int key)
{
  // save all students in the student array to a file 'studentdata.txt' overwriting
  // any existing file
  //   - the format of the file is one line per student as follows fname lname age id:
  //       tom thumb 15 1234 
  //       james dean 21 2345 
  //       katy jones 18 4532
  FILE* fp;
  fp = fopen(STUFILE, "w");
  if (fp) {
    char buff[256];
    for (int i = 0; i < numStudents; i++)
    {
      sprintf(buff, "%s %s %d %ld", students[i] -> firstName, students[i] -> lastName, students[i] -> age, students[i] -> id);
      if (key != 0) {
        caesarEncrypt(buff, key);
      }
      fprintf(fp, "%s\n", buff);
      printf("saving %s\n", buff);
    }
    fclose(fp);
    printf("saved %d students", numStudents);
  }
}


void loadStudents(int key)
{
  // load the students from the data file overwriting all exisiting students in memory
  char buff1[256];
  char buff2[256];
  char buff3[256];
  char buff4[256];
  int age;
  long id;
  if (numStudents > 0) {
    deleteStudents(0);
  }
  FILE* fp;
  fp = fopen(STUFILE, "r");
  if (fp) {
    int match;
    while (1) {
      match = fscanf(fp, "%s %s %s %s", buff1, buff2, buff3, buff4);
      if (match == 4) {
        if (key != 0) {
          caesarDecrypt(buff1, key);
          caesarDecrypt(buff2, key);
          caesarDecrypt(buff3, key);
          caesarDecrypt(buff4, key);
        }
        sscanf(buff3, "%d", &age);
        sscanf(buff4, "%ld", &id);
        createStudent(buff1, buff2, age, id);
      }
      else {
        fclose(fp);
        printf("loaded %d students", numStudents);
        return;
      }
    }
  }
  else {
    printf("could not find the file you are looking for!");
    return;
  }
}


void printStudent(Student* student)
{
  printf("  Student: %s %s\n", student->firstName, student->lastName);
  printf("    age: %d\n", student->age);
  printf("    id: %ld\n", student->id); 
}


void printStudents()
{
  for (int i = 0; i < numStudents; i++) {
    printf("\n----- student %d ------\n", i);
    printStudent(students[i]);
  }
}




