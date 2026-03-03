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
    printf("starting range must be a positive integer\n");
    return 1;
  }

  if (end <= start) {
    printf("ending range must be greater than starting range\n");
    return 1;
  }

  if (proc_count < 1 && proc_count <= end - start) {
    printf("process count cannot be less than 1 or greater than the range size\n");
    return 1;
  }

  if ((end - start) % proc_count != 0) {
    printf("process count must be a multiple of the range size to guaruntee the correct result\n");
    return 1;
  }

  int batch_size = (end - start) / proc_count;
  
  if (batch_size > 255) {
    printf("WARNING: The batch size (N per-process) is greater than 255. If there are more than 255 primes, the return code will overflow and the result will be incorrect.\n");
  }

  pid_t processes[proc_count];
  for (int i = 0; i < proc_count; i++) {
    pid_t pid = fork();

    if (pid == -1) {
      fprintf(stderr, "fork failed\n");
      return 1;
    }

    if (pid == 0) {
      char batch_start[11];
      char batch_end[11];

      sprintf(batch_start, "%d", start + batch_size * i);
      sprintf(batch_end, "%d", start + batch_size * i + batch_size);

      char* args[] = { "./countprimes", batch_start, batch_end, NULL };

      execvp("./countprimes", args);

      fprintf(stderr, "execvp failed\n");
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

    if (WIFEXITED(status)) {
      total += WEXITSTATUS(status);
    } else {
      fprintf(stderr, "child %d did not exit normally\n", i);
      return 1;
    }
  }

  printf("total primes: %d\n", total);

  return 0;
}
