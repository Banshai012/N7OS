#include <stdio.h>
#include <n7OS/processus.h>
#include <n7OS/cpu.h>

void fork2() {
  printf("Hello, world from fork\n");
  sleep(7);
  printf("Hello, world from fork2\n");
  sleep(12);
  printf("Hello, world from fork3\n");
  for (;;);
}