#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct CourseInfo
{
  char course_code[10];
  char description[100];
  float grade; // both percentages
  float completed;
  float units;
} course;

typedef struct CourseList
{
  course info[10];
  int count;
} courses;

void deleteEndl(char *str)
{
  int maxStr = 100;
  for (int i = 0; i < maxStr; i++)
  {
    if (str[i] == '\0')
      break;
    if (str[i] == '\n')
      str[i] = '\0';
  }
}

char *gradeToLetter(float grade)
{
  FILE *fp = fopen("gpatable.txt", "r");
  char numStr[10];
  static char letterGrade[10];
  int numInt;

  while (1)
  {
    fgets(numStr, 10, fp);
    numInt = atoi(numStr);
    if (grade * 100 < numInt)
    {
      fgets(numStr, 10, fp);
      continue;
    }
    else
    {
      fgets(letterGrade, 10, fp);
      deleteEndl(letterGrade);
      fclose(fp);
      return letterGrade;
    }
  }
}

float highestGradePossible(float grade, float completed)
{
  return (1 - completed) + grade * completed;
}

void printCourseInfo(course info, int i)
{
  double highestPossible = highestGradePossible(info.grade, info.completed);

  printf("%d:\n", i + 1);
  printf("\t%s\n\t%s\n", info.course_code, info.description);
  printf("\tGrade: %.2lf%% after %.0lf%% ", info.grade * 100, info.completed * 100);
  printf("(%s)\n", gradeToLetter(info.grade));
  printf("\tHighest grade possible is %.2lf%% ", 100 * highestPossible);
  printf("(%s)\n", gradeToLetter(highestPossible));
}

int main(int argc, char *argv[])
{
  course apsc100 = {0};
  strcpy(apsc100.course_code, "APSC 100");
  strcpy(apsc100.description, "Intro to Engineering");
  apsc100.grade = 0.8192;
  apsc100.completed = 0.41;

  course mren203 = {0};
  strcpy(mren203.course_code, "MREN 203");
  strcpy(mren203.description, "Intro to Mechatronics");
  mren203.grade = 0.5552;
  mren203.completed = 0.12;

  courses CourseList = {0};
  CourseList.info[0] = apsc100;
  CourseList.info[1] = mren203;
  CourseList.count = 2;

  for (int i = 0; i < CourseList.count; i++)
  {
    printCourseInfo(CourseList.info[i], i);
  }
}
