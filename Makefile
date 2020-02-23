CFLAGS = -Wall -g
CC     = gcc $(CFLAGS)

commando: cmd.o cmdcol.o commando.o
	$(CC) -o commando cmd.o cmdcol.o commando.o
	@echo commando is ready

commando.o : commando.c commando.h
	$(CC) -c commando.c

cmd.o : cmd.c commando.h
	$(CC) -c $<

cmdcol.o : cmdcol.c commando.h
	$(CC) -c $<

test_cmd : test_cmd.c cmd.o cmdcol.o commando.h
	$(CC) -o $@ $^

test : test_cmd commando
	@chmod u+x testy
	./testy test_commando.org $(testnum)

clean: 
	@echo Cleaning up object files
	rm -f commando test_cmd *.o

clean-tests:
	@echo Removing temporary test files
	rm -f *.o test_cmd test-results

# collatz_main : collatz_funcs.o collatz_main.o
# 	$(CC) -o collatz_main collatz_funcs.o collatz_main.o
# 	@echo collatz_main is ready

# collatz_main.o : collatz_main.c collatz.h
# 	$(CC) -c collatz_main.c

# collatz_funcs.o : collatz_funcs.c collatz.h
# 	$(CC) -c $<

# test_collatz : test_collatz.c collatz_funcs.o collatz.h
# 	$(CC) -o $@ $^

# test : test_collatz collatz_main
# 	@chmod u+x testy
# 	./testy test_collatz.org $(testnum)

# clean:
# 	rm -f collatz_main test_collatz *.o

# clean-tests:
# 	@echo Removing temporary test files
# 	rm -f *.o test_collatz test-results

