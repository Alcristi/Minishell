all:
	gcc -g src/*  -lreadline ./libft/libft.a
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp --trace-children=yes ./a.out
