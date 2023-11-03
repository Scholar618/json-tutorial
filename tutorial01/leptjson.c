#include "leptjson.h"
#include <assert.h>  /* assert() */
#include <stdlib.h>  /* NULL */

#define EXPECT(c, ch)       do { assert(*c->json == (ch)); c->json++; } while(0)

/*
 * 存储解析过程中上下文信息
 */
typedef struct {
    const char* json;
}lept_context;

/*
 * 跳过空白字符
 */
static void lept_parse_whitespace(lept_context* c) {
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}

/*
 * 判断当前JSON是否为"null"
 * 是，则返回LEPT_PARSE_OK
 * 否，则返回LEPT_PARSE_INVALID_VALUE
 */
static int lept_parse_null(lept_context* c, lept_value* v) {
    EXPECT(c, 'n');
    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 3;
    v->type = LEPT_NULL;
    return LEPT_PARSE_OK;
}

/*
 * 判断当前JSON是否为"true"
 */
static int lept_parse_true(lept_context* c, lept_value* v) {
    EXPECT(c, 't');
    if(c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 3;
    v->type = LEPT_NULL;
    return LEPT_PARSE_OK;
}


/*
 * 判断当前JSON是否为"false"
 */
static int lept_parse_false(lept_context* c, lept_value* v) {
    EXPECT(c, 'f');
    if(c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 4;
    v->type = LEPT_NULL;
    return LEPT_PARSE_OK;
}

/*
 * 解析JSON值的类型
 * LEPT_PARSE_EXPECT_VALUE:表示预期值但未找到值
 * LEPT_PARSE_INVALID_VALUE:遇到了无效的值
 */
static int lept_parse_value(lept_context* c, lept_value* v) {
    switch (*c->json) {
        case 'n':  return lept_parse_null(c, v);
        case 't':  return lept_parse_true(c, v);
        case 'f':  return lept_parse_false(c, v);
        case '\0': return LEPT_PARSE_EXPECT_VALUE;
        default:   return LEPT_PARSE_INVALID_VALUE;
    }
}

/*
 * 解析JSON
 */
int lept_parse(lept_value* v, const char* json) {
    lept_context c;
    int ret;
    assert(v != NULL);
    c.json = json;
    v->type = LEPT_NULL;
    lept_parse_whitespace(&c);
    if((ret = lept_parse_value(&c, v)) == LEPT_PARSE_OK) {
        lept_parse_whitespace(&c);
        if(*c.json != '\0') 
            ret = LEPT_PARSE_ROOT_NOT_SINGULAR;
    }
    return ret;
}

/*
 * 获取v的类型
 * assert(v != NULL)：确保传入的指针不是空指针
 */
lept_type lept_get_type(const lept_value* v) {
    assert(v != NULL);
    return v->type;
}
