src=$(wildcard *.c)
obj=$(patsubst  %.c ,%.o ,$(src))

args=  -g

ALL:walkdir.out


walkdir.out:$(obj)
	gcc $^ -o $@  $(args) 


%.o:%.c
	gcc -c $^ -o $@ $(args)


clean:
	-rm -rf *.o *.out


