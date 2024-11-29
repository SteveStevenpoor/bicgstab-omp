BIN_NAME	= bcgstab

SRCS		= main.cpp\
			  src/ParallelOperations.cpp\
			  src/ParallelOperations_test.cpp

HEADERS		= include/ParallelOperations.h

OBJ			= $(SRCS:.cpp=.o)

CC			= g++ -fopenmp -O0

# DBGFLAGS	= -fsanitize=address
DBGFLAGS 	=

CFLAGS		= -Iinclude -I/usr/include/eigen3/Eigen/

LDFLAGS		= -lgtest -pthread

all: $(BIN_NAME) clean

clean:
	@rm -rf *.o src/*.o

.cpp.o:
	$(CC) $(CFLAGS) $(DBGFLAGS) -c $< -o $@

$(BIN_NAME): $(OBJ)
	$(CC) $(LDFLAGS) $(DBGFLAGS) $(OBJ) -o $@
