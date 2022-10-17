#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <readline/readline.h>
#include <readline/history.h>


int main(int argc, char** argv) {
  printf("Welcome! You can exit by pressing Ctrl+C at any time...\n");

  if (argc > 1 && std::string(argv[1]) == "-d") {
    rl_bind_key('\t', rl_insert);
  }

  char* buf;
  while ((buf = readline(">> ")) != nullptr) {
    if (strlen(buf) > 0) {
      add_history(buf);
    }

    printf("[%s]\n", buf);

    free(buf);
  }

  return 0;
}
