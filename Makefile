all:
	gcc -g src/*.c builtins/*.c -lreadline ./libft/libft.a
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp --trace-children=yes ./a.out
run:
	gcc -g src/*.c builtins/*.c -lreadline ./libft/libft.a
	./a.out
