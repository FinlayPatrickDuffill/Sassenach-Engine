#include <iostream>
#include <fstream>
#include <stdio.h>
using namespace std;

FILE* logfile;

void add_to_log(const char* content_to_add)
{
  logfile = fopen ("log.txt","a");

  // Write to the file
  fputs(content_to_add, logfile);

  // Close the file
  fclose(logfile);
}
