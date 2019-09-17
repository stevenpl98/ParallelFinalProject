#CC= gcc
#CC= mpicc
CC= tau_cc.sh
LD_FLAGS= -lstdc++ -lopencv_core -lopencv_highgui -lopencv_imgproc -fopenmp
FLAGS= -I/usr/include/opencv -fopenmp
PROG1= serial.x
PROG2= parallel.x
OBJS= serial.o
OBJS2= parallel.o

#all rule
all: $(PROG1) $(PROG2)

$(PROG1): $(OBJS)
	$(CC) $^ $(LD_FLAGS) -o $@
$(PROG2): $(OBJS2)
	$(CC) $^ $(LD_FLAGS) -o $@

%.o: %.c
	$(CC) $(FLAGS) -c $^ -o $@

# clean rule
clean:
	$(RM) -rf $(PROG1) $(OBJS)
	$(RM) -rf $(PROG2) $(OBJS2)