#include<vector>
#include<cstdlib>
#include<iostream>
#include<sstream>
#include<map>
 
struct Command {
    std::string name;
    std::vector<int> arg;
    Command() = default;
    Command(const Command &c) = default;
    Command(std::string &s, std::vector<int> &a) :
        name(s), arg(a) {}
};
 
std::vector<int> in, out, target, area;
std::vector<Command> command;
std::map<std::string, int> ava;
int block;
constexpr int inf = 0x7fffffff;
typedef std::vector<Command>::iterator iter;

void error(iter cur) {
    int id = cur - command.begin() + 1;
    std::cout << "Error on instruction " << id << std::endl;
}
 
#define CURRENT_BLOCK_NOT_ZERO 1
#define INBOX_EMPTY 2
#define OUTBOX_EMPTY 3
#define NO_CURRENT_BLOCK 4
#define NO_SUCH_AREA 5
#define NO_SUCH_COMMAND 6
#define AREA_EMPTY 7
 
int inbox() {
    if(in.empty()) return INBOX_EMPTY;
    block = *in.begin();
    in.erase(in.begin());
    return 0;
}
 
int outbox() {
    if(block == inf) return NO_CURRENT_BLOCK;
    out.push_back(block);
    block = inf;
    return 0;
}
 
int add(int x) {
    if(x < 0 || area.size() <= x) return NO_SUCH_AREA;
    if(block == inf) return NO_CURRENT_BLOCK;
    if(area[x] == inf) return AREA_EMPTY;
    block += area[x];
    return 0;
}
 
int sub(int x) {
    if(x < 0 || area.size() <= x) return NO_SUCH_AREA;
    if(block == inf) return NO_CURRENT_BLOCK;
    if(area[x] == inf) return AREA_EMPTY;
    block -= area[x];
    return 0;
}
 
int copyto(int x) {
    if(x < 0 || area.size() <= x) return NO_SUCH_AREA;
    if(block == inf) return NO_CURRENT_BLOCK;
    area[x] = block;
    return 0;
}
 
int copyfrom(int x) {
    if(x < 0 || area.size() <= x) return NO_SUCH_AREA;
    if(area[x] == inf) return AREA_EMPTY;
    block = area[x];
    return 0;
}
 
int jump(iter &cur, int x) {
    if(x <= 0 || command.size() < x) return NO_SUCH_COMMAND;
    cur = command.begin() + x - 1;
    return 0;
}
 
int jumpifzero(iter &cur, int x) {
    if(x <= 0 || command.size() < x) return NO_SUCH_COMMAND;
    if(block == inf) return NO_CURRENT_BLOCK;
    if(block != 0) return CURRENT_BLOCK_NOT_ZERO;
    cur = command.begin() + x - 1;
    return 0;
}
 
#define INBOX_NOT_EMPTY 8
#define TARGET_NOT_ACHIEVED 9
 
int success() {
    if(!in.empty()) return INBOX_NOT_EMPTY;
    if(out != target) return TARGET_NOT_ACHIEVED;
    return 0;
}
 
void run() {
    block = inf;
    auto cur = command.begin();
    while(cur != command.end()) {
        if(ava.find(cur->name) == ava.end())
            return error(cur);
        if(ava[cur->name] != cur->arg.size())
            return error(cur);
        if(cur->name == "inbox") {
            if(inbox())
                break;
            ++cur;
        }
        else if(cur->name == "outbox") {
            if(outbox())
                return error(cur);
            ++cur;
        }
        else if(cur->name == "add") {
            if(add(cur->arg[0]))
                return error(cur);
            ++cur;
        }
        else if(cur->name == "sub") {
            if(sub(cur->arg[0]))
                return error(cur);
            ++cur;
        }
        else if(cur->name == "copyto") {
            if(copyto(cur->arg[0]))
                return error(cur);
            ++cur;
        }
        else if(cur->name == "copyfrom") {
            if(copyfrom(cur->arg[0]))
                return error(cur);
            ++cur;
        }
        else if(cur->name == "jump") {
            if(jump(cur, cur->arg[0]))
                return error(cur);
        }
        else if(cur->name == "jumpifzero") {
            int o = jumpifzero(cur, cur->arg[0]);
            if(o) {
                if(o == CURRENT_BLOCK_NOT_ZERO) ++cur;
                else return error(cur);
            }
        }
    }
    if(success() == 0) std::cout << "Success\n";
    else std::cout << "Fail\n";
}
 
int main() {
    int T; 
    std::cin >> T;
 
    if(T == 1) {
        in = {1, 2};
        target = {1, 2};
        area.resize(0);
        ava.emplace("inbox", 0);
        ava.emplace("outbox", 0);
    }
    else if(T == 2) {
        in = {3, 9, 5, 1, -2, -2, 9, -9};
        target = {-6, 6, 4, -4, 0, 0, 18, -18};
        area = {inf, inf, inf};
        ava.emplace("inbox", 0);
        ava.emplace("outbox", 0);
        ava.emplace("add", 1);
        ava.emplace("sub", 1);
        ava.emplace("copyto", 1);
        ava.emplace("copyfrom", 1);
        ava.emplace("jump", 1);
        ava.emplace("jumpifzero", 1);
    }
    else if(T == 3) {
        in = {6, 2, 7, 7, -9, 3, -3, -3};
        target = {7, -3};
        area = {inf, inf, inf};
        ava.emplace("inbox", 0);
        ava.emplace("outbox", 0);
        ava.emplace("add", 1);
        ava.emplace("sub", 1);
        ava.emplace("copyto", 1);
        ava.emplace("copyfrom", 1);
        ava.emplace("jump", 1);
        ava.emplace("jumpifzero", 1);
    }
 
    int m;
    std::cin >> m;
    std::string line;
    std::getline(std::cin, line);
    for(int i = 1; i <= m; ++i) {
        std::getline(std::cin, line);
        std::stringstream ss(line);
        std::string name;
        ss >> name;
        std::vector<int> arg;
        int x;
        while(ss >> x) arg.emplace_back(x);
        command.emplace_back(name, arg);
    }
 
    run();
}