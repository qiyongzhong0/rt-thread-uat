/*
 * uat.c
 *
 * Change Logs:
 * Date           Author            Notes
 * 2023-05-18     qiyongzhong       first version
 */

#include <uat.h>
#include <rs485.h>
#include <typedef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DBG_TAG "uat"
#define DBG_LVL DBG_INFO //DBG_LOG //
#include <rtdbg.h>

struct uat_inst{
    rs485_inst_t *rs485;
    const char *end_sign;
    char *buf;
    int bufsize;
};

#ifdef UAT_USING_PRT_RAW
static void uat_print_raw(const char *buf, int len, bool is_send)
{
    if (len <= 0) return;
    rt_kprintf("uat %s ", is_send ? "<<" : ">>");
    for (int i=0; i<len; i++)
    {
        rt_kprintf("%02X ", buf[i]);
    }
    rt_kprintf("\n");
}
#endif

uat_inst_t * uat_inst_create(const char *serial, int baudrate, int byte_tmo_ms, int bufsize)
{
    uat_inst_t *hinst = calloc(1, sizeof(uat_inst_t));
    if (hinst == RT_NULL)
    {
        LOG_E("create uat instance fail.");
        return(RT_NULL);
    }

    hinst->bufsize = bufsize;
    hinst->buf = calloc(1, bufsize);
    if (hinst->buf == RT_NULL)
    {
        LOG_E("create buffer fail.");
        uat_inst_destory(hinst);
        return(RT_NULL);
    }

    hinst->rs485 = rs485_create(serial, baudrate, 0, -1, 0);
    if (hinst->rs485 == RT_NULL)
    {
        LOG_E("create rs485 instance fail.");
        uat_inst_destory(hinst);
        return(RT_NULL);
    }

    rs485_set_byte_tmo(hinst->rs485, byte_tmo_ms);

    LOG_D("uat create success.");
    return(hinst);
}

void uat_inst_destory(uat_inst_t *hinst)
{
    RT_ASSERT(hinst != RT_NULL);

    if (hinst->rs485 != RT_NULL)
    {
        rs485_destory(hinst->rs485);
    }

    if (hinst->buf != RT_NULL)
    {
        free(hinst->buf);
        hinst->buf = RT_NULL;
        hinst->bufsize = 0;
    }

    free(hinst);

    LOG_D("uat destory success.");
}

char *uat_get_buf(uat_inst_t *hinst)
{
    RT_ASSERT(hinst != RT_NULL);
    return(hinst->buf);
}

int uat_data_recv(uat_inst_t *hinst, char *buf, int size, int tmo_ms)
{
    RT_ASSERT(hinst != RT_NULL);
    RT_ASSERT(buf != RT_NULL);

    rs485_connect(hinst->rs485);
    rs485_set_recv_tmo(hinst->rs485, tmo_ms);
    int recv_len = rs485_recv(hinst->rs485, buf, size);
    #ifdef UAT_USING_PRT_RAW
    uat_print_raw(buf, recv_len, false);
    #endif
    return(recv_len);
}

int uat_data_send(uat_inst_t *hinst, const char *buf, int size)
{
    RT_ASSERT(hinst != RT_NULL);
    RT_ASSERT(buf != RT_NULL);
    
    rs485_connect(hinst->rs485);
    int rst = rs485_send(hinst->rs485, (void *)buf, size);
    if (rst < 0)
    {
        rs485_disconn(hinst->rs485);
    }
    #ifdef UAT_USING_PRT_RAW
    uat_print_raw(buf, rst, true);
    #endif
    return(rst);
}

const char * uat_search_keyword(const char *buf, int size, const char *kw)
{
    RT_ASSERT(buf != RT_NULL);
    RT_ASSERT(kw != RT_NULL);

    int kw_size = strlen(kw);

    if (size < kw_size)
    {
        return(RT_NULL);
    }

    for (int i=0; i<=(size - kw_size); i++)
    {
        if (memcmp(buf, kw, kw_size) == 0)
        {
            return(buf);
        }
        buf++;
    }

    return(RT_NULL);
}

const char * uat_get_line_by_kw(const char *buf, int size, const char *kw)
{
    RT_ASSERT(buf != RT_NULL);
    RT_ASSERT(kw != RT_NULL);

    char *line = (char *)uat_search_keyword(buf, size, kw);
    if (line == RT_NULL)
    {
        return(RT_NULL);
    }

    size -= (int)(line - buf);
    for (int i=0; i<size; i++)
    {
        if ((line[i] == '\r') || (line[i] == '\n'))
        {
            line[i] = 0;
            break;
        }
    }

    return((const char *)line);
}

int uat_parse_line_args(const char *line, const char *expr, ...)
{
    RT_ASSERT(line != RT_NULL);
    RT_ASSERT(expr != RT_NULL);

    int args_num;
    va_list args;

    va_start(args, expr);
    args_num = vsscanf(line, expr, args);
    va_end(args);

    return(args_num);
}

int uat_execute_at_cmd(uat_inst_t *hinst, int tmo_ms, const char *cmd_expr, ...)
{
    RT_ASSERT(hinst != RT_NULL);
    RT_ASSERT(cmd_expr != RT_NULL);

    int len;
    va_list args;

    va_start(args, cmd_expr);
    len = vsprintf(hinst->buf, cmd_expr, args);
    va_end(args);

    strcpy(hinst->buf + len, "\r\n");
    len += 2;

    LOG_D(">>%s", hinst->buf);

    rs485_connect(hinst->rs485);
    rs485_set_recv_tmo(hinst->rs485, tmo_ms);
    len = rs485_send_then_recv(hinst->rs485, hinst->buf, len, hinst->buf, hinst->bufsize);
    if (len < 0)
    {
        rs485_disconn(hinst->rs485);
        return(len);
    }
    if (len == 0)
    {
        return(0);
    }

    hinst->buf[len] = 0;
    LOG_D("<<%s", hinst->buf);

    if (uat_search_keyword(hinst->buf, len, "OK") != RT_NULL)
    {
        return(len);
    }
    if (hinst->end_sign != RT_NULL)
    {
        if (uat_search_keyword(hinst->buf, len, hinst->end_sign) != RT_NULL)
        {
            return(len);
        }
    }

    return(-RT_ERROR);
}
