#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char** argv) {
  if (argc != 4) {
    printf("You must provide a starting range, ending range, and process count\n");
    return 1;
  }

  int start = atoi(argv[1]);
  int end = atoi(argv[2]);
  int proc_count = atoi(argv[3]);

  if (start < 0) {
    printf("range start must be non-negative\n");
    return 1;
  }

  if (end <= start) {
    printf("range end must be greater than start\n");
    return 1;
  }

  int range = end - start;

  if (proc_count < 1 || proc_count > range) {
    printf("process count must be between 1 and %d\n", range);
    return 1;
  }

  int base_batch_size = range / proc_count;
  int remainder = range % proc_count;
  
  if (base_batch_size > 255) {
    printf("WARNING: per-process range >255, which may cause exit-code overflow\n");
  }

  pid_t processes[proc_count];
  for (int i = 0; i < proc_count; i++) {
    int extra = (i < remainder) ? 1 : 0;

    int batch_start = start + i * base_batch_size
      + (i < remainder ? i : remainder);

    int batch_end = batch_start + base_batch_size + extra;

    pid_t pid = fork();

    if (pid < 0) {
      fprintf(stderr, "fork failed\n");
      return 1;
    }

    if (pid == 0) {
      char start_str[16];
      char end_str[16];

      snprintf(start_str, sizeof(start_str), "%d", batch_start);
      snprintf(end_str, sizeof(end_str), "%d", batch_end);

      char* args[] = { "./countprimes", start_str, end_str, NULL };

      execvp(args[0], args);
      return 1;
    }

    processes[i] = pid;
  }

  int total = 0;

  for (int i = 0; i < proc_count; i++) {
    int status;

    if (waitpid(processes[i], &status, 0) < 0) {
      fprintf(stderr, "waitpid failed\n");
      return 1;
    }

    if (!WIFEXITED(status)) {
      fprintf(stderr, "child %d didn't exit normally\n", i);
      return 1;
    }

    total += WEXITSTATUS(status);
  }

  printf("total primes: %d\n", total);

  return 0;
}
