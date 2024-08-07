#Server
.PHONY: test clean all
CFLAGS = -I ../src -Wall -g -DSENTINEL
SRC_DIR = ../src
all: test 
# test 타겟을 빌드하기 전에 src 디렉토리의 rbtree.o를 빌드합니다.
test: test-rbtree
	./test-rbtree
	valgrind ./test-rbtree
test-rbtree: test-rbtree.o $(SRC_DIR)/rbtree.o
	cc $(CFLAGS) test-rbtree.o $(SRC_DIR)/rbtree.o -o test-rbtree
test-rbtree.o: test-rbtree.c $(SRC_DIR)/rbtree.h
	cc $(CFLAGS) -c test-rbtree.c -o test-rbtree.o
# src 디렉토리의 rbtree.o를 빌드합니다.
$(SRC_DIR)/rbtree.o: $(SRC_DIR)/rbtree.c $(SRC_DIR)/rbtree.h
	$(MAKE) -C $(SRC_DIR) rbtree.o
clean:
	rm -f test-rbtree *.o
	$(MAKE) -C $(SRC_DIR) clean





# .PHONY: test
# CC = gcc
# CFLAGS=-I ../src -Wall -g -DSENTINEL

# test: test-rbtree
# 	./test-rbtree
# 	valgrind ./test-rbtree

# test-rbtree: test-rbtree.o ../src/rbtree.o

# ../src/rbtree.o:
# 	$(MAKE) -C ../src rbtree.o

# clean:
# 	rm -f test-rbtree *.o
