#ifndef PONG_LOGGER_H
#define PONG_LOGGER_H

#include <cstdio>

void InitLogger(const char* logFilePath = "logs/game.log");

void CloseLogger();

#endif