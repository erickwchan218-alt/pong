#ifndef PONG_LOGGER_H
#define PONG_LOGGER_H

#include <cstdio>

static void CustomLogCallback(int msgType, const char *text, va_list args);

void InitLogger(const char* logFilePath = "logs/game.log");

void CloseLogger();

#endif