#include "BotHandle.h"
using namespace std;

BotHandle::BotHandle(string name, int matchId, int playerId, int timeBank)
  : TimeBank(timeBank) , name(name) {
  int parent_child[2];
  int child_parent[2];
  if (pipe(parent_child) || pipe(child_parent)) {
    perror("Pipe failed");
    return;
  }

  int childPID = fork();
  if (childPID >= 0) {
    if (childPID == 0) {
      close(child_parent[0]); // close read end
      close(parent_child[1]); // close write end
      dup2(parent_child[0], STDIN_FILENO);
      dup2(child_parent[1], STDOUT_FILENO);
      std::string logFileName = "log/" + to_string(matchId) + "-" + to_string(playerId) + "-" + name;
      int logFile = open(logFileName.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
      dup2(logFile, STDERR_FILENO);
      string path = "./bots/" + name;
      execl(path.c_str(), "", nullptr);
      printf("%s\n", "Something went wrong");
    } else {
      pid = childPID;
      close(parent_child[0]); // close read end
      close(child_parent[1]); // close write end
      reader = child_parent[0];
      writer = parent_child[1];
      fcntl(reader, F_SETFL, O_NONBLOCK);
    }
  } else {
    cout << "Something went wrong with bot creation" << endl;
    exit(-1);
  }
}

void BotHandle::Send(string msg) {
  msg = string(msg) + "\n";
  if (write(writer, msg.c_str(), msg.length()) != (int)msg.length()) {
    cerr << "BotHandle write failure" << endl;
    exit(-1);
  }
}

void BotHandle::Kill() {
  close(writer);
  close(reader);
  kill(pid, SIGTERM);
}

string BotHandle::Receive() {
  static const int buffer_size = 64;
  static const int sleep_ms = 64;
  char buffer[buffer_size];
  int size;
  string msg;

  while (true) {
    size = read(reader, buffer, buffer_size);
    switch (size) {
      case 0:
        return "#NONE - CLOSED";
      case -1:
        if (errno == EAGAIN) {
          if (msg.empty()) {
            nanosleep((const struct timespec[]){{0, sleep_ms*1000000}}, nullptr);
          }
          else return msg;
        }
        else {
          return "#NONE A - CLOSED";
        }
      default:
        if (size > 0) msg += string(buffer, size);
        break;
    }
  }
}
