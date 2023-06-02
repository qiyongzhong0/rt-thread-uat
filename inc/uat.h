/*
 * uat.h
 *
 * Change Logs:
 * Date           Author            Notes
 * 2023-05-18     qiyongzhong       first version
 */

#ifndef __UAT_H__
#define __UAT_H__

#include <rtthread.h>

//#define UAT_USING_PRT_RAW       //使用原始数据打印输出

struct uat_inst;
typedef struct uat_inst uat_inst_t;

uat_inst_t * uat_inst_create(const char *serial, int baudrate, int byte_tmo_ms, int bufsize);
void uat_inst_destory(uat_inst_t *hinst);
char *uat_get_buf(uat_inst_t *hinst);
void *uat_set_end_sign(uat_inst_t *hinst, const char *end_sign);
int uat_data_recv(uat_inst_t *hinst, char *buf, int size, int tmo_ms);
int uat_data_send(uat_inst_t *hinst, const char *buf, int size);
const char * uat_search_keyword(const char *buf, int size, const char *kw);
const char * uat_get_line_by_kw(const char *buf, int size, const char *kw);
int uat_parse_line_args(const char *line, const char *expr, ...);
int uat_execute_at_cmd(uat_inst_t *hinst, int tmo_ms, const char *cmd_expr, ...);

#endif
