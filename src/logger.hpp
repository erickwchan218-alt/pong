#ifndef PONG_LOGGER_H
#define PONG_LOGGER_H

// Initializes the logger (opens game.log and sets raylib callback)
void InitLogger(const char* logFilePath = "game.log");

// Cleans up and closes the log file on exit
void CloseLogger();

#endif