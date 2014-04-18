/*
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

#define TIMELIMIT 25
#define ATTEMPTS 10
#define TIME 20

int attempts(void);
int againstTheClock(void);
void success(void);
void printHelp(void);

static int superSecretNumber;
static int numberWang;
static int result;
static int numAttempts = 1;
static time_t begin, end;
static double time_spent;

/*
 * Main function, ran on start
 */
int
main(int argc, char* argv[])
{

        if((argc > 1) && (strcmp("help", argv[1]) == 0)) {
                printHelp();
                return EXIT_SUCCESS;
        }


        //Make sure the user specified a difficulty
        if(argc != 3) {
                printf("Usage: %s <attempts|time|help> [easy|medium|hard]\n", argv[0]);
                return EXIT_FAILURE;
        }

        //Initialise random number generator
        srand( (unsigned int)(time(NULL)) );
        superSecretNumber = 0;
        numberWang = 0;

        /*
         * Game mode:
         * attempts = 10 attempts
         * time = 25 seconds
         */
        if(strcmp("attempts", argv[1]) == 0) {
                result = ATTEMPTS;
        }
        else if(strcmp("time", argv[1]) == 0) {
                result = TIME;
        }
        else {
                printf("%s is not a valid game mode. Please try again.\n", argv[1]);
                return EXIT_FAILURE;
        }


        /*
         * Difficulty:
         * Easy = 0-100
         * Medium = 0-1000
         * Hard = 0-5000
         */
        if(strcmp("easy", argv[2]) == 0) {
                superSecretNumber = rand() % 101;
                numberWang = rand() % 101;
                printf("Easy Mode: 0-100\n");
        }
        else if(strcmp("medium", argv[2]) == 0) {
                superSecretNumber = rand() % 1001;
                numberWang = rand() % 1001;
                printf("Medium Mode: 0-1000\n");
        }
        else if(strcmp("hard", argv[2]) == 0) {
                superSecretNumber = rand() % 5001;
                numberWang = rand() % 5001;
                printf("Hard Mode: 0-5000\n");
        }
        else {
                printf("%s is not a valid difficulty. Please try again.\n", argv[2]);
                return EXIT_FAILURE;
        }

        //run gamemode based on argument
        if(result == ATTEMPTS) {
                result = attempts();
        }
        else if(result == TIME) {
                result = againstTheClock();
        }
        else {
                return EXIT_FAILURE;
        }

        if(result == 0) {
                exit(EXIT_SUCCESS);
        } else {
                exit(EXIT_FAILURE);
        }
}

/*
 * attempts function
 * Ran when attempts gamemode is selected
 */
int
attempts()
{
        int guess;

        //Start the clock
        time(&begin);

        printf("Guess what the secret number is: ");
        //Read a number in from the keyboard
        int test = scanf("%d", &guess);
        if(test != 1) return EXIT_FAILURE;

        while(guess != superSecretNumber) {
                if(numAttempts >= 10) {
                        printf("Sorry, you ran out of guesses!\n");
                        printf("The number was: %d\n", superSecretNumber);
                        time(&end);
                        time_spent = difftime(end,begin);
                        printf("It took you %d seconds\n", (int)time_spent);
                        return EXIT_SUCCESS;
                }

                if(guess == numberWang) {
                        printf("THAT'S NUMBERWANG!\n");
                        return EXIT_SUCCESS;
                }
                if(guess < superSecretNumber) {
                        printf("Too low, try a higher number: ");
                }
                else if(guess > superSecretNumber) {
                        printf("Too high, try a lower number: ");
                }

                numAttempts++;
                test = scanf("%d", &guess);
                if(test != 1) return EXIT_FAILURE;
        }

        success();
        return EXIT_SUCCESS;
}

/*
 * againstTheClock function
 * Ran when time game mode is selected
 */
int
againstTheClock()
{
        int guess;
        int time_left;

        //Start the clock
        time(&begin);

        printf("Guess what the secret number is: ");
        //Read a number in from the keyboard
        int test = scanf("%d", &guess);
        if(test != 1) return EXIT_FAILURE;
        while(guess != superSecretNumber) {
                time(&end);
                time_spent = difftime(end,begin);
                time_left = TIMELIMIT - (int)time_spent;

                if(time_left <= 0) {
                        printf("Sorry, you ran out of time!\n");
                        printf("The number was: %d\n", superSecretNumber);
                        time(&end);
                        time_spent = difftime(end, begin);
                        printf("It took you %d seconds\n", (int)time_spent);
                        return EXIT_SUCCESS;
                }

                if(guess == numberWang) {
                        printf("THAT'S NUMBERWANG!\n");
                        return EXIT_SUCCESS;
                }

                if(guess < superSecretNumber) {
                        printf("Time Left : %2d | Too low, try a higher number: ", time_left);
                }
                else if(guess > superSecretNumber) {
                        printf("Time Left : %2d | Too high, try a lower number: ", time_left);
                }

                numAttempts++;
                test = scanf("%d", &guess);
                if(test != 1) return EXIT_FAILURE;
        }

        success();
        return EXIT_SUCCESS;
}

/*
 * Success function
 * ran when the user has won
 */
void
success()
{
        printf("Correct! The number was: %d\n", superSecretNumber);
        printf("It took you %d attempts\n", numAttempts);
        time(&end);
        time_spent = difftime(end,begin);
        printf("It took you %d seconds\n", (int)time_spent);
}

/*
 * printHelp function
 * ran when the user inputs the help argument
 */
void
printHelp()
{
        printf("Number Guesser - V1.0\n");
        printf("A simple number guessing game.\n\n");
        printf("There are two gamemodes, attempts and time.\n\n");

        printf("In attempts mode, you are given 10 attempts at guessing the correct number.\n");
        printf("Each time you take a guess, you are told whether the actual number is higher\n");
        printf("or lower. You have unlimited time.\n");
        printf("If you run out of guesses, the game is over.\n\n");

        printf("In time mode, you are given 25 seconds to guess the correct number.\n");
        printf("Each time you take a guess, your remaining time is printed\n");
        printf("You have unlimited guesses.\n");
        printf("If you run out of time, the game is over.\n\n");

        printf("There are three difficulties: easy, medium and hard\n");
        printf("In easy mode, the number could be anything from 0-100\n");
        printf("In medium mode, the number could be anything from 0-1000\n");
        printf("In hard mode, the number could be anything from 0-5000\n");
}

