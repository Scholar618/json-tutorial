本文是学习github大佬miloyip而做的读书笔记，项目[点此进入](https://github.com/miloyip/json-tutorial)

[TOC]

### 1、JSON是什么

JSON（JavaScript Object Notation）是一个用于数据交换的文本格式

例如，一个动态网页想从服务器获得数据时，服务器会从数据库查找数据，然后把数据转换成 JSON 文本格式，JSON库通常提供了一些常用的功能，例如将对象、数组、字符串和数字转换为JSON格式，以及将JSON数据解析为对象、数组等。

JSON是树状结构，而 JSON 只包含 6 种数据类型：

- null: 表示为 null
- boolean: 表示为 true 或 false
- number: 一般的浮点数表示方式，在下一单元详细说明
- string: 表示为 "..."
- array: 表示为 [ ... ]
- object: 表示为 { ... }

我们要实现的 JSON 库，主要是完成 3 个需求：

1. 把 JSON 文本解析为一个树状数据结构（parse）。
2. 提供接口访问该数据结构（access）。
3. 把数据结构转换成 JSON 文本（stringify）。

![img](https://img-blog.csdnimg.cn/90f4d534319a47d1bb275cec743c8494.png)![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

### 2、搭建编译环境

安装cmake环境

### 3、头文件与API设计

已知JSON中只包含6种数据类型，如果把 true 和 false 当作两个类型就是 7 种，为此声明一个枚举类型：

```cpp
typedef enum { LEPT_NULL, LEPT_FALSE, LEPT_TRUE, LEPT_NUMBER, LEPT_STRING, LEPT_ARRAY, LEPT_OBJECT } lept_type;
```

因为 C 语言没有 C++ 的命名空间（namespace）功能，一般会使用项目的简写作为标识符的前缀。通常**枚举值用全大写**（如 `LEPT_NULL`），而**类型及函数则用小写**（如 `lept_type`）。



接下来，声明 JSON 的数据结构。JSON 是一个树形结构，我们最终需要实现一个树的数据结构，每个节点使用 `lept_value` 结构体表示，我们会称它为一个 JSON 值（JSON value）。 

```cpp
typedef struct {
    lept_type type;
}lept_value;
```

> 在此单元中，我们只需要实现 null, true 和 false 的解析，因此该结构体只需要存储一个 lept_type。之后的单元会逐步加入其他数据。

然后，我们现在只需要两个 API 函数，一个是解析 JSON，一个是获取访问结果类型的函数

```cpp
int lept_parse(lept_value* v, const char* json);

lept_type lept_get_type(const lept_value* v);
```

> 由于传入的JSON文本是一个C字符串，我们不用改动该字符串，所以使用const类型
>
> 传入的v是由使用方负责分配的，类型即是lept_value
>
> 使用方法如下：
>
> ```c
> lept_value v;
> const char json[] = ...;
> int ret = lept_parse(&v, json);
> ```

> lept_get_type，即是获取访问结果的函数，具体就是获取其类型

### 4、JSON的语法子集

下面是此单元的 JSON 语法子集，使用 [RFC7159](https://tools.ietf.org/html/rfc7159) 中的 [ABNF](https://tools.ietf.org/html/rfc5234) 表示：

```
JSON-text = ws value ws
ws = *(%x20 / %x09 / %x0A / %x0D)
value = null / false / true 
null  = "null"
false = "false"
true  = "true"
```

> 那么第一行的意思是，JSON 文本由 3 部分组成，首先是空白（whitespace），接着是一个值，最后是空白。
>
> 当中 `%xhh` 表示以 16 进制表示的字符，`/` 是多选一，`*` 是零或多个，`()` 用于分组。
> 第二行告诉我们，所谓空白，是由零或多个*空格符*（space U+0020）、*制表符*（tab U+0009）、*换行符*（LF U+000A）、*回车符*（CR U+000D）所组成。
>
> 第三行是说，我们现时的值只可以是 null、false 或 true，它们分别有对应的字面值（literal）。

我们的解析器应**能判断输入是否一个合法的 JSON**。如果输入的 JSON 不合符这个语法，我们要产生**对应的错误码**，方便使用者追查问题。

在这个 JSON 语法子集下，我们定义 3 种错误码：

- 若一个 JSON 只含有空白，传回 `LEPT_PARSE_EXPECT_VALUE`。例如：" "
- 若一个值之后，在空白之后还有其他字符，传回 `LEPT_PARSE_ROOT_NOT_SINGULAR`。例如：“true abc”
- 若值不是那三种字面值，传回 `LEPT_PARSE_INVALID_VALUE`。例如：“hello”

### 5、单元测试

单元测试概念

> 一般我们在做练习题时，都是以printf / cout打印结果，再用肉眼对比结果看是否符合预期，但当软件项目越来越复杂，这总做法会越来越低效，于是我们引入了一种新的自动的测试方法。**单元测试**，单元测试也能确保其他人修改代码后，原来的功能维持正确，这称为**回归测试** / regression testing

常见的单元测试框架有xUnit系列，如C++的Google Test、C#的NUnit。而我们为了举例，会编写一个极其简单的单元测试方法。

一般来说，软件开发是以**周期进行**的。例如，<u>加入一个功能，再写关于该功能的单元测试</u>。但也有另一种软件开发方法论，称为**测试驱动开发**（test-driven development, **TDD**），它的主要循环步骤是：

1. 加入一个测试。
2. 运行所有测试，新的测试应该会失败。
3. 编写实现代码。
4. 运行所有测试，若有测试失败回到3。
5. 重构代码。
6. 回到 1。

TDD 是**先写测试，再实现功能**。好处是实现只会刚好满足测试，而不会写了一些不需要的代码，或是没有被测试的代码。

但是无论是采用TOD还是先实现后测试，都要尽量加入足够覆盖率的单元测试

### 6、宏的编写技巧

如果宏里有多过一个语句（statement），就需要用 `do { /*...*/ } while(0)` 包裹成单个语句

### 7、实现解析器

有了 API 的设计、单元测试，终于要实现解析器了。

首先为了减少解析函数之间传递多个参数，我们把这些数据都放进一个 `lept_context` 结构体：

```C
typedef struct {
    const char* json;
}lept_context;

/* ... */

/* 提示：这里应该是 JSON-text = ws value ws */
/* 以下实现没处理最后的 ws 和 LEPT_PARSE_ROOT_NOT_SINGULAR */
int lept_parse(lept_value* v, const char* json) {
    lept_context c;
    assert(v != NULL);
    c.json = json;
    v->type = LEPT_NULL;
    lept_parse_whitespace(&c);
    return lept_parse_value(&c, v);
}
```

由于 JSON 语法特别简单，我们不需要写分词器（tokenizer），只需检测下一个字符，便可以知道它是哪种类型的值，然后调用相关的分析函数。对于完整的 JSON 语法，跳过空白后，只需检测当前字符：

- n ➔ null
- t ➔ true
- f ➔ false
- " ➔ string
- 0-9/- ➔ number
- [ ➔ array
- { ➔ object

### 8、关于断言

断言（assertion）是 C 语言中常用的<u>防御式编程方式</u>，减少编程错误。最常用的是在函数开始的地方，检测所有参数。有时候也可以在调用函数后，检查上下文是否正确。

C 语言的标准库含有 [`assert()`](https://en.cppreference.com/w/c/error/assert) 这个宏（需 `#include <assert.h>`），提供断言功能。当程序以 release 配置编译时（定义了 `NDEBUG` 宏），`assert()` 不会做检测；而当在 debug 配置时（没定义 `NDEBUG` 宏），则会在运行时检测 `assert(cond)` 中的条件是否为真（非 0），断言失败会直接令程序崩溃。

**何时使用断言？何时处理运行时错误？**

简单的答案是，如果那个错误是由于程序员错误编码所造成的（例如传入不合法的参数），那么应用断言；如果那个错误是程序员无法避免，而是由运行时的环境所造成的，就要处理运行时错误（例如开启文件失败）。

