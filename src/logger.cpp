#include "logger.hpp"
#include "raylib.h"
#include <cstdio>
#include <cstdarg>
#include <ctime>

static FILE *g_logFile = nullptr;

static void CustomLogCallback(int msgType, const char *text, va_list args) {
    if (!g_logFile) return;

    time_t now = time(nullptr);
    struct tm *timeinfo = localtime(&now);
    char timeBuffer[20];
    strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", timeinfo);

    fprintf(g_logFile, "[%s] ", timeBuffer);
    switch (msgType) {
        case LOG_INFO:    fprintf(g_logFile, "[INFO] : "); break;
        case LOG_ERROR:   fprintf(g_logFile, "[ERROR]: "); break;
        case LOG_WARNING: fprintf(g_logFile, "[WARN] : "); break;
        case LOG_DEBUG:   fprintf(g_logFile, "[DEBUG]: "); break;
        default: break;
    }

    vfprintf(g_logFile, text, args);
    fprintf(g_logFile, "\n");
    fflush(g_logFile);
}

void InitLogger(const char* logFilePath) {
    g_logFile = fopen(logFilePath, "w");
    if (g_logFile) {
        SetTraceLogCallback(CustomLogCallback);
    }
}

void CloseLogger() {
    if (g_logFile) {
        fclose(g_logFile);
        g_logFile = nullptr;
    }
}