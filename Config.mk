BASE_CFLAGS := -std=c23 -Wall -Wextra -Wpedantic -Werror
DEBUG_CFLAGS := -g -Og
RELEASE_CFLAGS := -O2

CC := clang
INCLUDES_DIR := ./includes
SOURCES_DIR := ./src
BUILD_DIR := ./build

SOURCES := main.c
PROGRAM := $(BUILD_DIR)/derkbasic
