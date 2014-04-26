/*-
 * Copyright (c) 2014, Jonathan Price
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

#include "NumberGuesser.h"

static int play_attempts(void);
static int play_time(void);
static int gamemode(void);
static int difficulty(void);
static void print_help(void);
static void usage(void)  __attribute__((noreturn));

static int answer;
static int numberwang;

/*
 * Main function, initialises random, determines gamemode
 * and difficulty, and executes functions appropriately
 * if an error occurs, return EXIT_FAILURE, else EXIT_SUCCESS
 */
int
main(int argc, char *argv[])
{
	int mode, diff, result, ch;
	
	while ((ch = getopt(argc, argv, "hH:")) != -1) {
		switch (ch) {
		case 'h':
			/* FALLTHROUGH */
		case 'H':
			usage();
		}
	}

	/* Initialise random number generator */
	srand((unsigned int)(time(NULL)));

	/* fetch the gamemode from the user */
	mode = gamemode();
	if (mode == EXIT_FAILURE) {
		return EXIT_FAILURE;
	} else if (mode == MODE_HELP) {
		print_help();
		return EXIT_SUCCESS;
	}
	
	/* fetch the difficulty from the user */
	diff = difficulty();
	if (diff == EXIT_FAILURE)
		return EXIT_FAILURE;
	
        /* run gamemode based on argument */
	switch (mode) {
		case MODE_ATTEMPTS:
			result = play_attempts();
			break;
		case MODE_TIME:
			result = play_time();
			break;
		default:
			fprintf(stderr, "An unknown error occurred\n");
			exit(EXIT_FAILURE);
	}

	if (result == 0) {
		return EXIT_SUCCESS;
	} else {
		fprintf(stderr, "An unknown error occurred\n");
		exit(EXIT_FAILURE);
	}
}

/*
 * attempts mode. Reads input from user and outputs response
 * if an error occurs, return EXIT_FAILURE, else EXIT_SUCCESS
 */
static int
play_attempts()
{
	int guess, num_attempts = 1;
	double time_spent;
	time_t begin, end;

	//Start the clock
	time(&begin);

	printf("Guess what the secret number is: ");
	//Read a number in from the keyboard
	int test = scanf("%d", &guess);
	if (test != 1) return EXIT_FAILURE;

	while (guess != answer) {
		if (num_attempts >= 10) {
			printf("Sorry, you ran out of guesses!\n");
			printf("The number was: %d\n", answer);
			time(&end);
			time_spent = difftime(end,begin);
			printf("It took you %d seconds\n", (int)time_spent);
			return EXIT_SUCCESS;
		}

		if (guess == numberwang) {
			printf("THAT'S NUMBERWANG!\n");
			return EXIT_SUCCESS;
		} if (guess < answer) {
			printf("Too low, try a higher number: ");
		} else if (guess > answer) {
			printf("Too high, try a lower number: ");
		}

		num_attempts++;
		test = scanf("%d", &guess);
		if (test != 1)
			return EXIT_FAILURE;
	}

	printf("Correct! The number was: %d\n", answer);
	printf("It took you %d attempts\n", num_attempts);
	time(&end);
	time_spent = difftime(end, begin);
	printf("It took you %d seconds\n", (int)time_spent);
	return EXIT_SUCCESS;
}

/*
 * time gamemode. Reads input from user and outputs response
 * if an error occurs, return EXIT_FAILURE, else EXIT_SUCCESS
 */
static int
play_time()
{
	int guess, time_left, num_attempts = 1;
	double time_spent;
	time_t begin, end;

	/* Start the clock */
	time(&begin);

	printf("Guess what the secret number is: ");

	/* Read a number in from the keyboard */
	int test = scanf("%d", &guess);
	if (test != 1)
		return EXIT_FAILURE;
	while (guess != answer) {
		time(&end);
		time_spent = difftime(end,begin);
		time_left = TIMELIMIT - (int)time_spent;

		if (time_left <= 0) {
			printf("Sorry, you ran out of time!\n");
			printf("The number was: %d\n", answer);
			time(&end);
			time_spent = difftime(end, begin);
			printf("It took you %d seconds\n", (int)time_spent);
			return EXIT_SUCCESS;
		}

		if (guess == numberwang) {
			printf("THAT'S NUMBERWANG!\n");
			return EXIT_SUCCESS;
		}

		if (guess < answer) {
			printf("Time Left : %2d | Too low, try a higher number: ", time_left);
		} else if (guess > answer) {
			printf("Time Left : %2d | Too high, try a lower number: ", time_left);
		}

		num_attempts++;
		test = scanf("%d", &guess);
		if (test != 1)
			return EXIT_FAILURE;
	}

	printf("Correct! The number was: %d\n", answer);
	printf("It took you %d attempts\n", num_attempts);
	time(&end);
	time_spent = difftime(end, begin);
	printf("It took you %d seconds\n", (int)time_spent);
	return EXIT_SUCCESS;
}

/*
 * request a gamemode from the user, and run the relevant gamemode function
 * if an error occurs, return EXIT_FAILURE, else EXIT_SUCCESS
 */
static int
gamemode()
{
	printf("Choose a gamemode, (a)ttempts or (t)ime, or view (h)elp\n");
	char selection;
	int test = scanf(" %c", &selection);
	if (test != 1) return EXIT_FAILURE;
	
	switch (selection) {
		case 'a':
		case 'A':
			return MODE_ATTEMPTS;
		case 't':
		case 'T':
			return MODE_TIME;
		case 'h':
		case 'H':
			return MODE_HELP;
	}
	
	fprintf(stderr, "%c is not a valid gamemode, exiting.\n", selection);
	exit(EXIT_FAILURE);
}

/*
 * request a difficulty from the user
 * if an error occurs, return EXIT_FAILURE, else EXIT_SUCCESS
 */
static int
difficulty()
{
	printf("Choose a difficulty, (e)asy, (m)edium or (h)ard\n");
	char selection;
	int test = scanf(" %c", &selection);
	if (test != 1)
		return EXIT_FAILURE;
	
	/* Determine difficulty from input, and initialise entropy accordingly */
	switch (selection) {
		case 'e':
		case 'E':
			answer = rand() % (EASY_MAX + 1);
			numberwang = rand() % (EASY_MAX + 1);
			printf("Easy Mode: 0-%d\n", EASY_MAX);
			return DIFF_EASY;
		case 'm':
		case 'M':
			answer = rand() % (MEDIUM_MAX + 1);
			numberwang = rand() % (MEDIUM_MAX + 1);
			printf("Medium Mode: 0-%d\n", MEDIUM_MAX);
			return DIFF_MEDIUM;
		case 'h':
		case 'H':
			answer = rand() % (HARD_MAX + 1);
			numberwang = rand() % (HARD_MAX + 1);
			printf("Hard Mode: 0-%d\n", HARD_MAX);
			return DIFF_HARD;
	}
	
	fprintf(stderr, "%c is not a valid difficulty, exiting.\n", selection);
	exit(EXIT_FAILURE);
}

/*
 * ran when the user inputs the help argument
 */
static void
print_help()
{
	printf("Number Guesser - V1.0\n");
	printf("A simple number guessing game.\n\n");
	printf("There are two gamemodes, attempts and time.\n\n");

	printf("In attempts mode, you are given a fixed number of attempts at guessing the ");
	printf("correct number.\n");
	printf("Each time you take a guess, you are told whether the actual number is higher\n");
	printf("or lower. You have unlimited time.\n");
	printf("If you run out of guesses, the game is over.\n");
	printf("Easy: %d attempts\n", EASY_ATTEMPTS);
	printf("Medium: %d attempts\n", MEDIUM_ATTEMPTS);
	printf("Hard: %d attempts\n\n", HARD_ATTEMPTS);

	printf("In time mode, you are given %d seconds to guess the correct number.\n", TIMELIMIT);
	printf("Each time you take a guess, your remaining time is printed\n");
	printf("You have unlimited guesses.\n");
	printf("If you run out of time, the game is over.\n\n");

	printf("There are three difficulties: easy, medium and hard\n");
	printf("In easy mode, the number could be anything from 0-%d\n", EASY_MAX);
	printf("In medium mode, the number could be anything from 0-%d\n", MEDIUM_MAX);
	printf("In hard mode, the number could be anything from 0-%d\n", HARD_MAX);
}

/*
 * print usage to stderr
 */
static void
usage()
{
	(void)fprintf(stderr, "Usage: NumberGuesser [-h | -H]\n");
	exit(EXIT_FAILURE);
}
