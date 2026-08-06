#ifndef PONG_LOGGER_HPP
#define PONG_LOGGER_HPP

#include <cstdio>

void InitLogger(const char* logFilePath = "logs/game.log");

void CloseLogger();

#endif // PONG_LOGGER_HPP
