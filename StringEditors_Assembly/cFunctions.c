#include <stdio.h>
#include <stdlib.h>

void display(char *arr[])
{
    //iterates through array in prints each string
    for (int i = 0; i < 10; i++)
    {
        printf("Message[%d]: %s\n", i, arr[i]);
    }
}

void read(char *arr[], int *mem[])
{
    //validates that the location of the string is in the range
    int index = -1;
    while (index < 0 || index > 9)
    {
        printf("What string do you want to replace? ");
        scanf("%d", &index);
        if (index < 0 || index > 9)
            printf("Invalid String Location. \n");
    }
    char newLine = fgetc(stdin);
    int size = 128;
    int pos = 0;
    //initalizes a new string 
    char *newString = malloc(sizeof(char) * size);
    char character;
    int flag = 1;
    printf("Enter new message: ");
    //adds each character one by one to the new string
    while (flag == 1)
    {
        character = fgetc(stdin);
        if (character == '\n')
        {
            newString[pos] = '\0';
            flag = 0;
        }
        else
        {
            newString[pos] = character;
        }
        pos++;
        //increaes memory if it gets too big
        if (pos >= size)
        {
            size += size;
            newString = realloc(newString, size);
        }
    }
    pos -= 2;
    int letter = (int)newString[0];
    flag = 1;
    //checks taht the first character is captial letter by using ascii value
    if (letter < 65 || letter > 90)
        flag = 0;
    //checks last character to be punctuation
    if (!(newString[pos] == '.' || newString[pos] == '?' || newString[pos] == '!'))
        flag = 0;
    //indexes through memory checker array to delete the current string if there was memory alllocated for it
    if (flag != 0){
        if(*mem[index] != 0){
            char* temp = arr[index];
            arr[index] = newString;
            free(temp);
            }
        else{
            arr[index] = newString;
        }
        //sets the number at the same index in memory array to 1, to indicate memory was allocated
        *mem[index] = 1;
    } 
    else
    //if the string is invalid free the new string
    {
        printf("Invalid message, keeping current.\n");
        free(newString);
    }
}

void printStats(char *arr[])
{
    //validates the string location is correct
    int index = -1;
    while (index < 0 || index > 9)
    {
        printf("What string do you want to see? ");
        scanf("%d", &index);
        if (index < 0 || index > 9)
            printf("Invalid String Location. \n");
    }

    char newLine = fgetc(stdin);
    int i = 0;
    //creates list of punctuation marks
    char listPunc[] = {'!', '?', '.', ',', '"', ':', ';', '-', '_', ')', '(', '{', '}', '[', ']'};
    int letters = 0;
    int digits = 0;
    int specials = 0;
    int punct = 0;
    int flag = 0;
    char *string = arr[index];
    while (string[i] != '\0')
    {
        flag = 0;
        //checks to see if the character is in the list of punctuations
        int asciiVal = (int)string[i];
        for (int j = 0; j < 15; j++)
        {
            if (string[i] == listPunc[j] || asciiVal == 39)
                flag = 1;
        }
        //uses ascii to see if its letter or number
        if ((asciiVal >= 65 && asciiVal <= 90) || (asciiVal >= 97 && asciiVal <= 122))
        {
            letters += 1;
        }
        else if (asciiVal >= 48 && asciiVal <= 57)
        {
            digits += 1;
        }
        else if (flag == 1)
        {
            punct += 1;
        }
        //anything else is special character
        else
        {
            specials += 1;
        }
        i += 1;
    }

    printf("There are a total of %d characters in the text.\n", i);
    printf("There are %d letters.\n", letters);
    printf("There are %d digits.\n", digits);
    printf("There are %d special characters.\n", specials);
    printf("There are %d punctuation characters.\n", punct);
}

void weave(char *arr[], int index){
    //gets the length of the string
    char* string = arr[index];
    printf("Current Message: %s \n", string);
    int i = 0;
    int length = 0;
    while (string[i] != '\0'){
        length++;
        i++;
    }
    i = 0;
    char character;
    //iterates through string until we reach or go past the end
    while(i < length){
        //if the second and third character is less than or equal to length, not including null, then swap them
        if((i + 1) <= (length - 1) && (i + 2) <= (length - 1)){
            character = string[i + 2];
            string[i + 2] = string[i + 1];
            string[i + 1] = character;
        }
        //goes to next 4 characters
        i += 4;
    }
    printf("Weaved: %s \n", string);
}

void printNum(char* string){
    //testing function
    printf("number entered: '%s' \n", string);
}

void freeList(char* arr[], int* mem[]){
    //frees the string if the same index in the memory list contains a 1, which means memory is allocated
    for(int i = 0; i < 10; i++){
        if(*mem[i] != 0)
            free(arr[i]);
    }
}

