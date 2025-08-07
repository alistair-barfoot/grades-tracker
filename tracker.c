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

char *extractValue(char *line, int column)
{
  int commas_seen = 0;
  int i = 0;

  // Move index forward until the desired number of commas have been seen
  while (line[i] != '\0' && commas_seen < column)
  {
    if (line[i] == ',')
      commas_seen++;
    i++;
  }

  // Extract characters starting at the column of interest until the next comma/newline
  static char value_str[50]; // Buffer to hold the numeric string
  int j = 0;
  while (line[i] != ',' && line[i] != '\n' && line[i] != '\0' && j < 49)
  {
    value_str[j++] = line[i++];
  }
  value_str[j] = '\0';

  // Convert string to a floating point number
  return value_str;
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
  fclose(fp);
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

void parseCourseInfo(courses *info)
{
  FILE *fp = fopen("storage.csv", "r");
  char buffer[100];
  fgets(buffer, 100, fp); // skip the first line
  char str_buf[50];
  int i = 0;

  course temp = {0};

  while (fgets(buffer, 100, fp) != NULL)
  {
    strcpy(temp.course_code, extractValue(buffer, 0));
    strcpy(temp.description, extractValue(buffer, 1));
    temp.grade = atof(extractValue(buffer, 2));
    temp.completed = atof(extractValue(buffer, 3));
    info->info[i] = temp;
    // printf("%s\n", info[i].info->course_code);
    i++;
  }

  info->count = i;
  fclose(fp);
}

void printOptions(course info)
{
  FILE *fp = fopen("storage.csv", "r");
  char line[100];
  fgets(line, 100, fp);

  char title[20];

  for (int i = 0; i < 5; i++)
  {
    strcpy(title, extractValue(line, i));
    printf("%d:\t%s\n", i + 1, title);
  }

  printf("Type the number for the value you would like to modify...\n");
}

void addAssessment(course *info)
{
  float grade;
  printf("What grade did you get?");
  scanf("%f", &grade);
  if (grade >= 1)
    grade /= 100;

  printf("How much was this assessment worth?");
  float worth;
  scanf("%f", &worth);
  if (worth >= 1)
    worth /= 100;

  float p1, p2, c1, c2;
  p1 = grade;
  p2 = info->grade;
  c1 = worth;
  c2 = info->completed;

  info->completed += worth;
  info->grade = (p1 * c1 + p2 * c2) / (c1 + c2);
}

void printToFile(courses info)
{
  FILE *fp = fopen("storage.csv", "w");
  fprintf(fp, "Course Code,Description,Grade,Completed,Units\n");

  for (int i = 0; i < info.count; i++)
  {
    fprintf(fp, "%s,", info.info[i].course_code);
    fprintf(fp, "%s,", info.info[i].description);
    fprintf(fp, "%.5f,", info.info[i].grade);
    fprintf(fp, "%.5f\n", info.info[i].completed);
  }

  fclose(fp);
}

int main(int argc, char *argv[])
{
  // course apsc100 = {0};
  // strcpy(apsc100.course_code, "APSC 100");
  // strcpy(apsc100.description, "Intro to Engineering");
  // apsc100.grade = 0.8192;
  // apsc100.completed = 0.41;

  // course mren203 = {0};
  // strcpy(mren203.course_code, "MREN 203");
  // strcpy(mren203.description, "Intro to Mechatronics");
  // mren203.grade = 0.5552;
  // mren203.completed = 0.12;

  courses CourseList = {0};
  // CourseList.info[0] = apsc100;
  // CourseList.info[1] = mren203;
  // CourseList.count = 2;

  parseCourseInfo(&CourseList);

  for (int i = 0; i < CourseList.count; i++)
  {
    printCourseInfo(CourseList.info[i], i);
  }
  printToFile(CourseList);

  while (1)
  {
    printf("Type the number for the course you would like to modify...\n");
    int index = getc(stdin) - '0' - 1;
    getc(stdin);

    printf("Would you like to add a grade (a) or edit (e)?\n");
    switch (getc(stdin))
    {
    case 'a':
      addAssessment(&CourseList.info[index]);
      break;
    case 'e':
      printOptions(CourseList.info[index]);
      break;
    default:
      printf("Try again\n");
    }
    getc(stdin);
    printToFile(CourseList);
    for (int i = 0; i < CourseList.count; i++)
    {
      printCourseInfo(CourseList.info[i], i);
    }
  }
}
