#ifndef LEPTJSON_H__
#define LEPTJSON_H__

/* 
 * JSON中有6种数据类型，把boolean看做两种TRUE和FALSE就是7中数据类型
 * 为此声明一个枚举类型enum
 * 默认LEPT_NULL为0，后面的元素逐步往后递增
 */
typedef enum { LEPT_NULL, LEPT_FALSE, LEPT_TRUE, LEPT_NUMBER, LEPT_STRING, LEPT_ARRAY, LEPT_OBJECT } lept_type;

/*
 * JSON是一种树形结构，最终就是需要实现一个树的数据结构
 * 每个节点使用 lept_value 结构体表示，我们会称它为一个 JSON 值（JSON value）
 * 在此单元中，我们只需要实现 null, true 和 false 的解析，因此该结构体只需要存储一个 lept_type。之后的单元会逐步加入其他数据。
 */
typedef struct {
    lept_type type;
}lept_value;

/*
 * 返回值是以下枚举值，无错误会返回LEPT_PARSE_OK
 */
enum {
    LEPT_PARSE_OK = 0,
    LEPT_PARSE_EXPECT_VALUE,
    LEPT_PARSE_INVALID_VALUE,
    LEPT_PARSE_ROOT_NOT_SINGULAR
};

/*
 * 解析 JSON 的API函数
 */
int lept_parse(lept_value* v, const char* json);

/*
 * 获取访问结果类型的函数
 */
lept_type lept_get_type(const lept_value* v);

#endif /* LEPTJSON_H__ */
