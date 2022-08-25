all:
	gcc -g src/*.c builtins/*.c -lreadline ./libft/libft.a
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp ./a.out
run:
	gcc -g src/*.c builtins/*.c -lreadline ./libft/libft.a
	./a.out
