#include "pipe_networking.h"

static void sighandler(int signo) {
  if (signo == SIGINT) {
    remove("luigi");
    exit(0);
  }
}

void process(char * s) {
  while ( (*s = toupper(*s)) ) {
    s++;
  }
}

void subserver(int from_client) {
  int to_client = server_connect(from_client);
  printf("[subserver %d] connected successfully\n", getpid());
  char buffer[BUFFER_SIZE];
  while (read(from_client, buffer, sizeof(buffer))) {
    printf("[subserver %d] received: %s\n", getpid(), buffer);
    process(buffer);
    write(to_client, buffer, sizeof(buffer));
  }
  exit(0);
}

int main() {
  signal(SIGINT, sighandler);
  while(1) {
    int from_client = server_setup();
    if (!fork()) subserver(from_client);
  }
}
