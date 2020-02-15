// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    FBullCowCount Count;

    Isograms = GetValidWords(Words);

    SetupGame();

}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if(bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(PlayerInput);
    }
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    HintNumber = FMath::RandRange(0, HiddenWord.Len() - 1);
    Hint = HiddenWord[HintNumber];
    Lives = HiddenWord.Len() * 3;
    bGameOver = false;

    PrintLine(TEXT("Welcome to BullCow!"));
    PrintLine(TEXT("Guess the %i letter word"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives"), Lives);
    PrintLine(TEXT("The character at position %i is %c"), HintNumber + 1, Hint);
    PrintLine(TEXT("Press Tab to type your guess,\n then press enter to guess..."));
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("The hidden word was %s"), *HiddenWord);
    PrintLine(TEXT("Press enter to play again"));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if(Guess == HiddenWord)
    {
        PrintLine(TEXT("You win!"));
        EndGame();
    }
    else
    {
        if(Guess.Len() != HiddenWord.Len())
        {
            PrintLine(TEXT("The word is %i letters long"), HiddenWord.Len());
        }
        else if(!IsIsogram(Guess))
        {
            PrintLine(TEXT("The word is not an Isogram"));
        }
        else
        {
            --Lives;
            if(Lives > 0)
            {
                FBullCowCount Score = GetBullCows(Guess);

                PrintLine(TEXT("You have %i Bulls & %i Cows"), Score.Bulls, Score.Cows);

                PrintLine(TEXT("Guess again, you have %i lives left"), Lives);
            }
            else
            {
                PrintLine(TEXT("You lose :("));
                EndGame();
            }
        }
    }
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for(int32 i = 0; i < Word.Len(); i++)
    {
        for(int32 j = i+1; j < Word.Len(); j++)
        {
            if(Word[i] == Word[j])
            {
                return false;
            }
        }
    }

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for(FString Word : WordList)
    {
        if(Word.Len() >= 4 && Word.Len() <= 8)
        {
            if(IsIsogram(Word))
            {
                ValidWords.Emplace(Word);
            }
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for(int32 i = 0; i < Guess.Len(); i++)
    {
        if(Guess[i] == HiddenWord[i])
        {
            Count.Bulls++;
            continue;
        }

        for(int32 j = 0; j < HiddenWord.Len(); j++)
        {
            if(Guess[i] == HiddenWord[j])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}