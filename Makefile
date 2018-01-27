project=proj2
CFLAGS=-std=c99 -Wall -Wextra -Werror
$(project): -lm $(project).o
clean:
	-rm $(project) $(project).o
