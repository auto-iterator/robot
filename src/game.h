#include"command.h"
#include"level.h"

constexpr int inf = 0x7fffffff;

#define CURRENT_BLOCK_NOT_ZERO 1
#define INBOX_EMPTY 2
#define OUTBOX_EMPTY 3
#define NO_CURRENT_BLOCK 4
#define NO_SUCH_AREA 5
#define NO_SUCH_COMMAND 6
#define AREA_EMPTY 7

#define INBOX_NOT_EMPTY 8
#define TARGET_NOT_ACHIEVED 9

class Game {
    private:
    std::vector<int> in, out, target, area;
    std::vector<Command> command;
    std::map<std::string, int> ava;
    int block;
    typedef std::vector<Command>::iterator iter;

    public:
    int inbox();
    int outbox();
    int add(int);
    int sub(int);
    int copyto(int);
    int copyfrom(int);
    int jump(iter&, int);
    int jumpifzero(iter&, int);
    int success();
    void run();
};