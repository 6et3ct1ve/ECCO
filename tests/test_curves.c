#include "../include/ecco/curves.h"
#include <gmp.h>
#include <stdio.h>

int main() {
  struct curve cur;
  if (curve_populate(&cur, "P-256")) {
    curve_print(&cur);
  } else {
    printf("error");
  }
  printf("\n\n");
  struct curve cur1;
  if (curve_populate(&cur1, "P-384")) {
    curve_print(&cur1);
  } else {
    printf("error");
  }
}