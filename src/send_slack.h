/**
 * @file send_slack.h
 * @brief use curl to send slack messages
 * @version 0.1
 * @date 2023-03-20
 *
 */
#ifndef SEND_SLACK_H
#define SEND_SLACK_H

#ifdef USE_SLACK
#ifndef SLACK_WEBHOOK_URL
#error cannot find webhook url definition, aborted
#endif

#define MAX_SLACK_MESSAGE_LENGTH 1000
char slack_message_buffer[MAX_SLACK_MESSAGE_LENGTH];

void init_send_slack();
void send_message(char *);

#endif  // USE_SLACK
#endif