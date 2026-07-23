**编译与运行说明**：

在 `src` 目录下打开终端，运行命令 `g++ archive.cpp command.cpp console.cpp game.cpp interface.cpp main.cpp simulator.cpp -o main -std=c++17 -O2` 进行编译，然后输入 `./main` 即可运行。

由于游戏会生成两个文件 `levels.dat` 和 `completion.dat` 用于存储关卡信息，所以为了防止污染源码目录，推荐将可执行文件 `main` 放在单独目录下运行。