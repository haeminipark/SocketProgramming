PROJECT_NAME=client

#컴파일러
CC=gcc

# 컴파일러 옵션
CFLAGS=-g -W -Wall -O

INCLUDE=-Iinc/
SRC_DIR=./src
OBJ_DIR=./obj

# wildcard로 SRC_DIR에서 *.c로 된 파일 목록을 뽑아낸 후 notdir로 파일이름만 뽑아낸다
# wildcard - ./Src/main.c ./Src/plus.c ./Src/mul.c
# notdir - 경로삭제, main.c plus.c mul.c
SRCS=$(notdir $(wildcard $(SRC_DIR)/*.c))

# OBJS = main.o plus.o mul.o
OBJS=$(SRCS:.c=.o)

# patsubst를 이용해 파일 경로 생성
# $(OBJS)안에 있는 모든 %.o 패턴을 $(OBJS_DIR)/%.o로 치환
# OBJECTS - ./Obj/main.o ./Obj/plus.o ./Obj/mul.o
OBJECTS=$(patsubst %.o,$(OBJ_DIR)/%.o,$(OBJS))

TARGET = $(PROJECT_NAME)

all: $(TARGET)

clean:
	rm -rf $(TARGET)* $(OBJECTS)


# 링크
$(TARGET):$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

# 컴파일
$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@