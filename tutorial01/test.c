#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leptjson.h"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;
/*
 * 检查预期值和实际值是否相等
 * test_count:测试用例加一
 * 为真：test_pass:通过的测试用例加一
 * 为假：输出错误信息
 * main_ret = 1:表示测试不通过
 */
#define EXPECT_EQ_BASE(equality, expect, actual, format) \
    do {\
        test_count++;\
        if (equality)\
            test_pass++;\
        else {\
            fprintf(stderr, "%s:%d: expect: " format " actual: " format "\n", __FILE__, __LINE__, expect, actual);\
            main_ret = 1;\
        }\
    } while(0)

/*
 *调用EXPECT_EQ_BASE宏，并将 (expect) == (actual) 作为第一个参数传递给它
*/
#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")

/*
 * 测试JSON中字符串中"null"情况
 * 第一个EXPECT_EQ_INT:将"null"解析到v，解析成功返回LEPT_PARSE_OK
 * 第二个EXPECT_EQ_INT:获取v的类型，并与LEPT_NULL比较
 */
static void test_parse_null() {
    lept_value v;
    v.type = LEPT_TRUE;
    EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, "null")); // 返回LEPT_PARSE_OK
    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v)); 
}

/*
 * 测试JSON中字符串中"true"情况
 */
static void test_pause_true() {
    lept_value v;
    v.type = LEPT_TRUE;
    EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, "true")); // 返回LEPT_PARSE_OK
    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v)); 
}

/*
 * 测试JSON中字符串中"false"情况
 */
static void test_pause_false() {
    lept_value v;
    v.type = LEPT_TRUE;
    EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, "false")); // 返回LEPT_PARSE_OK
    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v)); 
}

/*
 * 测试解析函数遇到期望值错误的情况
 * 第一个EXPECT_EQ_INT:将空字符""解析到v，解析成功返回LEPT_PARSE_EXPECT_VALUE
 * 第二个EXPECT_EQ_INT:获取v的类型，并与LEPT_NULL比较
 * 后面两个同理
 */
static void test_parse_expect_value() {
    lept_value v;

    v.type = LEPT_FALSE;
    EXPECT_EQ_INT(LEPT_PARSE_EXPECT_VALUE, lept_parse(&v, ""));
    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));

    v.type = LEPT_FALSE;
    EXPECT_EQ_INT(LEPT_PARSE_EXPECT_VALUE, lept_parse(&v, " "));
    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));
}

/*
 * 测试解析函数遇到无效值的行为
 * 第一个EXPECT_EQ_INT:将"nul"解析到v，解析成功应返回LEPT_PARSE_INVALID_VALUE
 * 第二个EXPECT_EQ_INT:获取v的类型，并与LEPT_NULL比较
 * 后面两个同理
 */
static void test_parse_invalid_value() {
    lept_value v;
    v.type = LEPT_FALSE;
    EXPECT_EQ_INT(LEPT_PARSE_INVALID_VALUE, lept_parse(&v, "nul"));
    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));

    v.type = LEPT_FALSE;
    EXPECT_EQ_INT(LEPT_PARSE_INVALID_VALUE, lept_parse(&v, "?"));
    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));
}

/*
 * 测试解析函数遇到多余字符的情况
 * 第一个EXPECT_EQ_INT:将"null x"解析到v， 解析成功返回LEPT_PARSE_ROOT_NOT_SINGULAR
 * 第二个EXPECT_EQ_INT:获取v的类型，并与LEPT_NULL比较
 */
static void test_parse_root_not_singular() {
    lept_value v;
    v.type = LEPT_FALSE;
    EXPECT_EQ_INT(LEPT_PARSE_ROOT_NOT_SINGULAR, lept_parse(&v, "null x"));
    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));
}

/*
 * 运行整个测试套件，即执行各个具体的测试函数。
 */
static void test_parse() {
    test_parse_null();
    test_pause_true();
    test_pause_false();
    test_parse_expect_value();
    test_parse_invalid_value();
    test_parse_root_not_singular();
}

int main() {
    test_parse();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
}
