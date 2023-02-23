# developer tools
### 目的

本仓库包含项目开发中，经常使用的一些基础的工具，如日志、线程池、内存池、通信库等。

### commit 格式

```shell
<type>(<scope>): <subject>
<commit类型>(影响范围): 具体描述
```

举例：

 `fix(log): fix the format error of log when printing`

`doc: increase the readme of each folder in this repo`

**type（必须）**

type指明git commit的类别，应该使用以下类型，也可根据团队自行增减

- feat: 新增功能
- fix: 修复 bug
- doc: 仅仅修改了文档，比如 README, CHANGELOG等等
- test: 增加/修改测试用例，包括单元测试、集成测试等
- style: 修改了空行、缩进格式、引用包排序等等（不改变代码逻辑）
- perf: 优化相关内容，比如提升性能、体验、算法等
- refactor: 代码重构，「没有新功能或者bug修复」
- chore: 改变构建流程、或者增加依赖库、工具等
- revert: 回滚到上一个版本
- merge: 代码合并

**scope（可选）**

scope用于说明 commit 影响的范围，根据不同项目有不同层次描述。若没有特殊规定，也可以描述影响的哪些功能等。

**subject（必须）**

subject是commit目的的简短描述，不超过50/80个字符，一般git提交的时候会有颜色提示。

- 若是开源代码，一律推荐统一英文，英文不行可以翻译软件用起来
- 若是开源代码，可以再附加对应的issue地址
- 结尾不加标点符号
# 代码格式推荐
1. 头文件使用#define 保护，防止头文件被多重包含
2. 尽可能地避免使用前置声明
3. 只有当函数只有10行升值更少时候才将其定义为内联函数
4. #include 顺序  eg： 若源文件为dir/foo.cc   #include的优先顺序为:dir2/foo2.h (优先位置, 详情如下) C系统文件 C++系统文件 其他库的 .h 文件  本项目内 .h 文件
5. 函数命名，变量命名，文件命名要有描述性，少用缩写
6. 文件名要全部小写，可以包含下划线或连字符，依照项目的约定，如果没有约定，那么下划线更好
7. 类型命名的每个单词首字母均大写，不包含下划线（类，结构体，类型定义（typedef），枚举，类型模板参数）
8. 变量命名（包含函数参数）和数据成员名一律小写，单词之间用下划线连接。类的成员变量以下划线结尾，*但结构体不需要*。如a_local_variable,a_struct_data_member,a_class_data_member_
9. 常量命名：声明为constexpr或const的变量，或在程序运行期间其值始终保持不变的，命名时以“k”开头，大小写混合。如 const int kDaysInAWeek = 7; 所有具有静态存储类型的变量（例如静态变量或全局变量）都应当以此方式命名。
10. 函数命名：一般来说函数名的每个单词首字母大写，没有下划线,如AddTableEntry()。但是取值和设置函数的命名与变量一直，一般来说他们的名称与实际的成员变量对应，但并不强制要求如int count()与void set_count(int count)
11. 枚举命名应该和常量或宏一致。如：kEnumName或ENUM_NAME
