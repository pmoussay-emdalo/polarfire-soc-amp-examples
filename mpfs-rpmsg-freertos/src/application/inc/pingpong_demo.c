#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/demo_main.h"

#define MSG_LIMIT	100U

#ifdef RPMSG_MASTER
#define RPMSG_PINGPONG_EPT_ADDR (1U)
#else
#define RPMSG_PINGPONG_EPT_ADDR (0u)
#endif

#define RPMSG_CHANNEL_NAME "rpmsg-amp-demo-channel"

#define RPMSG_HEADER_LEN 16
#define MAX_RPMSG_BUFF_SIZE (256 - RPMSG_HEADER_LEN)
#define PAYLOAD_MIN_SIZE	1
#define PAYLOAD_MAX_SIZE	(MAX_RPMSG_BUFF_SIZE - 24)
#define NUM_PAYLOADS		(PAYLOAD_MAX_SIZE/PAYLOAD_MIN_SIZE)

struct _payload {
    unsigned long num;
    unsigned long size;
    uint8_t data[(2 * sizeof(unsigned long)) + PAYLOAD_MAX_SIZE];
};

static struct _payload i_payload;
static struct _payload r_payload;

static bool pingpong_initialized = false;

void rpmsg_pingpong_demo_setup(rpmsg_comm_stack_handle_t handle)
{
    rpmsg_comm_stack_t *rpmsgHandle;
    rpmsgHandle = (rpmsg_comm_stack_t *)handle;

    rpmsgHandle->ctrl_ept = rpmsg_lite_create_ept(rpmsgHandle->my_rpmsg, RPMSG_PINGPONG_EPT_ADDR, rpmsg_queue_rx_cb, rpmsgHandle->ctrl_q);

#ifndef RPMSG_MASTER
    printf_to(UART_APP, "\r\nSending name service announcement\r\n");
    rpmsg_ns_announce(rpmsgHandle->my_rpmsg, rpmsgHandle->ctrl_ept, RPMSG_CHANNEL_NAME, RL_NS_CREATE);
#endif
}

void rpmsg_pingpong_demo(rpmsg_comm_stack_handle_t handle)
{
    uint32_t len;
    uint32_t remote_addr;
    int i;
#ifdef RPMSG_MASTER
    int j, size;
    int err_cnt;
    int ntimes = 1;

    err_cnt = 0;
#endif

    rpmsg_comm_stack_t *rpmsgHandle;
    rpmsgHandle = (rpmsg_comm_stack_t *)handle;

    if (!pingpong_initialized) {
        rpmsg_pingpong_demo_setup(handle);
        pingpong_initialized = true;
    }

    printf_to(UART_APP, "\r\nRunning Ping Pong demo\r\n");

#ifdef RPMSG_MASTER

    for (j=0; j < ntimes; j++)
    {
        printf_to(UART_APP, "\r\n **********************************");
        printf_to(UART_APP, "****\r\n");
        printf_to(UART_APP, "\r\n  Echo Test Round %d \r\n", j);
        printf_to(UART_APP, "\r\n **********************************");
        printf_to(UART_APP, "****\r\n");

        for (i = 0, size = PAYLOAD_MIN_SIZE; i < NUM_PAYLOADS; i++, size++)
        {
            int k;

            i_payload.num = i;
            i_payload.size = size;

            /* Mark the data buffer. */
            memset(&(i_payload.data[0]), 0xA5, size);

            printf_to(UART_APP, "\r\n sending payload number");
            printf_to(UART_APP, " %ld of size %d\r\n", i_payload.num, (2 * sizeof(unsigned long)) + size);

            (void)rpmsg_lite_send(rpmsgHandle->my_rpmsg, rpmsgHandle->ctrl_ept, 
                rpmsgHandle->remote_addr, (char *) &i_payload, (2 * sizeof(unsigned long)) + size, RL_BLOCK);

            r_payload.num = 0;

            rpmsg_queue_recv(rpmsgHandle->my_rpmsg, rpmsgHandle->ctrl_q, 
                            (uint32_t *)&remote_addr, (char *) &r_payload,
                            (2 * sizeof(unsigned long)) + PAYLOAD_MAX_SIZE, &len, RL_BLOCK);

            printf_to(UART_APP, " received payload number ");
            printf_to(UART_APP, "%ld of size %d\r\n", r_payload.num, len);

            /* Validate data buffer integrity. */
            for (k = 0; k < r_payload.size; k++) {
                if (r_payload.data[k] != 0xA5) {
                    printf_to(UART_APP, "\r\n Data corruption at index %d \r\n", k);
                    err_cnt++;
                    break;
                }
            }
        }
        printf_to(UART_APP, "\r\n **********************************");
        printf_to(UART_APP, "****\r\n");
        printf_to(UART_APP, "\r\n Echo Test Round %d Test Results: Error count = %d\r\n", j, err_cnt);
        printf_to(UART_APP, "\r\n **********************************");
        printf_to(UART_APP, "****\r\n");
    }

#else
    for (i = 0; i < NUM_PAYLOADS; i++)
    {
        rpmsg_queue_recv(rpmsgHandle->my_rpmsg, rpmsgHandle->ctrl_q, (uint32_t *)&remote_addr, 
            (char *)&r_payload, (2 * sizeof(unsigned long)) + PAYLOAD_MAX_SIZE, &len, RL_BLOCK);

        printf_to(UART_APP, " received payload number ");
        printf_to(UART_APP, "%ld of size %d\r\n", r_payload.num, len);

        rpmsg_lite_send(rpmsgHandle->my_rpmsg, rpmsgHandle->ctrl_ept, 
            remote_addr, (char *)&r_payload, len, RL_BLOCK);
    }
#endif
}
