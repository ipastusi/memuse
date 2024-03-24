TARGET_EXEC := memuse

default: clean
	clang src/$(TARGET_EXEC).c -o $(TARGET_EXEC) -std=c17 -pedantic -Wall -Wextra -Werror -O3 -DNDEBUG

clean:
	rm -f $(TARGET_EXEC)
