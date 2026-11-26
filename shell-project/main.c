/*
 * main.c - gshell, the Gaucho Shell
 *
 * This is the only file you edit and submit. main() and the prompt loop
 * are done for you. Every other function below has a comment describing
 * what it does. Work through the checkpoints in the writeup in order:
 *
 *   Checkpoint 1: tokenize, parse_pipeline, and the exit built-in
 *   Checkpoint 2: run_command and run_pipeline for a single command
 *   Checkpoint 3: input and output redirection
 *   Checkpoint 4: pipelines
 */

#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/** The longest input line is 100 characters, plus 1 for the '\0' */
#define MAX_LINE 101

/** A 100-character line has at most 50 whitespace-separated tokens */
#define MAX_TOKENS 50

/** A pipeline can never have more commands than the line has tokens */
#define MAX_COMMANDS MAX_TOKENS

/**
 * One command of a pipeline. For example, the line
 *
 *     /bin/sort -r < in.txt | /usr/bin/head -n 3
 *
 * is a pipeline of two commands. The first has argv {"/bin/sort", "-r",
 * NULL}, input_file "in.txt", and output_file NULL.
 */
typedef struct {
    /** The executable path and its arguments, followed by NULL (as execve expects) */
    char *argv[MAX_TOKENS + 1];
    /** The file named after <, or NULL if the command has no input redirection */
    char *input_file;
    /** The file named after >, or NULL if the command has no output redirection */
    char *output_file;
} command_t;

/**
 * Splits `line` into its whitespace-separated tokens and stores a pointer
 * to each one in `tokens`. Changes the contents of `line`.
 *
 * Returns the number of tokens.
 *
 * __attribute__((unused)) stops the compiler from warning that a function
 * is never called. You can remove it once you call the function.
 */
__attribute__((unused))
static int tokenize(char *line, char *tokens[]) {
    // TODO (Checkpoint 1)
    int count = 0;
    char *saveptr = NULL;
    const char *delims = " \t\n\r\f\v";
    char *token = strtok_r(line, delims, &saveptr);

    while (token != NULL && count < MAX_TOKENS) {
        tokens[count++] = token;
        token = strtok_r(NULL, delims, &saveptr);
    }
    return count;
}

/**
 * Groups `tokens` into the commands of a pipeline and fills in one
 * command_t per command in `commands`.
 *
 * Returns the number of commands, or -1 if the tokens are not a valid
 * pipeline.
 */
__attribute__((unused))
static int parse_pipeline(char *tokens[], int num_tokens, command_t commands[]) {
    // TODO (Checkpoint 1)
    if (num_tokens == 0) {
        return -1;
    }
    int command_count = 0;
    int arg_index = 0;
    commands[command_count].input_file = NULL;
    commands[command_count].output_file = NULL;
    for (int i = 0; i < num_tokens; i++) {
        if (strcmp(tokens[i], "|") == 0) {
            if (arg_index == 0) {
                return -1; 
            }
            commands[command_count].argv[arg_index] = NULL; 
            command_count++;
            if (command_count >= MAX_COMMANDS) {
                return -1; 
            }
            arg_index = 0;
            commands[command_count].input_file = NULL;
            commands[command_count].output_file = NULL;

        } else if (strcmp(tokens[i], "<") == 0) {
            if (i + 1 >= num_tokens || commands[command_count].input_file != NULL) {
                return -1; 
            }

            commands[command_count].input_file = tokens[++i];
        } else if (strcmp(tokens[i], ">") == 0) {
            if (i + 1 >= num_tokens || commands[command_count].output_file != NULL) {
                return -1; 
            }

            commands[command_count].output_file = tokens[++i];
        } else {

            commands[command_count].argv[arg_index++] = tokens[i];
        }
    }
    if (arg_index == 0) {
        return -1;
    }
    commands[command_count].argv[arg_index] = NULL;
    return command_count + 1;

}

/**
 * Starts `cmd` in a child process. The child reads its standard input
 * from `in_fd` and writes its standard output to `out_fd`, unless the
 * command redirects them to a file. Does not wait for the child.
 *
 * Returns the child's process ID, or -1 if no child was started.
 */
__attribute__((unused))
static pid_t run_command(command_t *cmd, int in_fd, int out_fd) {
    // TODO (Checkpoints 2 and 3)
    return -1;
}

/**
 * Runs the `num_commands` commands of a pipeline, with each command's
 * standard output connected to the next command's standard input. Then
 * waits for all of them and prints each one's exit status, in order.
 */
__attribute__((unused))
static void run_pipeline(command_t commands[], int num_commands) {
    // TODO (Checkpoints 2 and 4)
}

/**
 * Runs one line of input: either the exit built-in, or a pipeline of
 * commands. Prints "Invalid command" to stderr if the line is not a valid
 * pipeline.
 */
static void parse_and_run_command(char *line) {
    // TODO (Checkpoint 1): this is not the correct way to check for the
    // exit command. For example, the line "  exit  " should also exit.
    if (strcmp(line, "exit") == 0) {
        exit(0);
    }
    fprintf(stderr, "Not implemented.\n");
}

int main(void) {
    char line[MAX_LINE];

    while (1) {
        printf("> ");
        fflush(stdout);
        if (fgets(line, sizeof(line), stdin) == NULL)
            break;
        // fgets keeps the '\n' at the end of the line. strcspn(line, "\n")
        // returns the index of the first '\n' in line (or of the final '\0'
        // if there is none), so this replaces the newline with '\0'.
        line[strcspn(line, "\n")] = '\0';
        parse_and_run_command(line);
    }

    return 0;
}
