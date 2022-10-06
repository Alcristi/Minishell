#!/bin/bash
clear
make
make clean
valgrind -s --leak-check=full --show-leak-kinds=all --track-fds=yes --track-origins=yes --suppressions=readline.supp --trace-children=yes ./minishell