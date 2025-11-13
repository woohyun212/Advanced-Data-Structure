CC=gcc
SRC=Diablo-m-test.c
TARGET=Diablo-m-run

ifeq ($(OS),Windows_NT)
  HOST_OS := Windows
else
  # 그렇지 않으면 uname -s로 확인
  UNAME_S := $(shell uname -s)
  ifeq ($(UNAME_S),Darwin)
	HOST_OS := macOS
  else ifeq ($(UNAME_S),Linux)
	HOST_OS := Linux
  else
	HOST_OS := $(UNAME_S)
  endif
endif

# 컴파일 옵션/링크 옵션 초기화
CFLAGS  := -x c -std=c99 -Wall
LDFLAGS :=

ifeq ($(HOST_OS),Windows)
  # 실행 파일 이름만 ".exe"로 붙이기 위해 TARGET 재정의
  TARGET := $(TARGET).exe
  REMOVE_CMD := del
else ifeq ($(HOST_OS),macOS)
  # macOS 전용 설정
  TARGET := $(TARGET)
  REMOVE_CMD := rm
else ifeq ($(HOST_OS),Linux)
  # Linux 전용 설정
  TARGET := $(TARGET)
  REMOVE_CMD := rm
endif

clean_build: clean all

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	-$(REMOVE_CMD) $(TARGET)