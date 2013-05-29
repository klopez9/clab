
/* Kevin Lopez, klopez9 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define  WORD_LENGTH  30
#define  INITIAL_ARRAY_MAX 50

int loadArray(char *inFileName, char ***array, int *count, int *capacity);
void printArray(char **array, int count);
void doubleArraySize(char ***array, int *capacity);
void menu(char ***array, int *wordCount, int *capacity);
int searchWord(char ***array, int *count);
int insertWord(char ***dictionary, int *wordCount, char word[], int *capacity);
int removeWord(char ***array, int *count);
int printCount(int count);
int writeToFile(char *outFileName, char ***array, int *count);

int main(int argc, char* argv[])
{
    char **wordArray;  /* declare the dynamic array of strings */
                       /* the array is NOT allocated yet */
    int capacity = INITIAL_ARRAY_MAX;
    int wordCount = 0;
    int i;

    if (argc != 3) /* if execution command does not have the .exe,
                      an inputFile and an outputFile specified */
    {
        fprintf(stderr,"Usage: %s inputfile outputfile\n", argv[0]);
        return 1;
    }

    if (loadArray(argv[1], &wordArray, &wordCount, &capacity) != 0)
    
    /* i.e. if loadArray does not reach the end of the function ("return 0").
     * loadArray is called here; it takes in the 2nd argument from the terminal command
     (the text file of words) and references to wordArray, wordCount, and capacity so
     we can pass-by-reference. The function parameters, thus, are a string, a triple
     char pointer, and two int pointers.
     */
    {
        fprintf(stderr,"    loadArray failed! Program terminating...\n");
        return 1;  /* don't need to call exit - we're in main! */
    }

    printf("\nFinished loading %d words.", wordCount);
    printf("\nArray Capacity is %d\n", capacity);
    menu(&wordArray, &wordCount, &capacity);

    if (writeToFile(argv[2], &wordArray, &wordCount) != 0)
    {
      printf("Error writing to file\n");
    }
    else 
    {
      printf("Dictionary written to file '%s'\n", argv[2]);
    }
    
    for (i = 0; i < wordCount; i++) 
    {
      if (wordArray[i] != NULL) 
      {
        free(wordArray[i]);
      }
    }
    free(wordArray);
    return 0;
}


int loadArray(char *inFileName, char ***array, int *count, int *capacity)
{
    FILE *inFile; /* file stream */
    char word[WORD_LENGTH];  /* this is the ONLY auto array we'll need */

    if ((inFile = fopen(inFileName, "r")) == NULL)
    /* opens file "inFileName" for reading */
    {
	    fprintf(stderr,"Error opening input file, %s\n", inFileName);
	    return -1;
    }

    *array = (char **)malloc((*capacity) * sizeof(char*));

    /* we only dereference once in order to pass-by-reference.
    ***array is a reference to an array of strings. The extra *
    is to denote that we are passing-by-reference.
    */

    if (*array == NULL)
    {
	    fprintf(stderr, "Malloc of array in loadArray failed!\n");
	    return -1;
    }

    printf("\nReading file '%s' (each . is 1000 words read)\n", inFileName);

    *count = 0; /*dereference the reference to count*/

    while (fscanf(inFile, "%s", word) == 1)
    {
	    if ((*count+1) >= *capacity)
	    {
        doubleArraySize(array, capacity);
	    }

	    if (insertWord(array, count, word, capacity) != 0)
	    {
	       fprintf(stderr,"    Insert returned an error!\n");
	       fclose(inFile);
	       return 1;
	    }
	
	    if (*count % 1000 == 0)
	    {
	      printf(".");
	      fflush(stdout);  /* stdout is buffered, so have to force flush */
	    }
    }

    fclose(inFile);

    return 0;
}

void printArray(char **array, int count)
{
   int i;

   printf("\n");
   for (i = 0; i < count; i++)
       printf("array[%d]:\t%s\n", i, array[i]);
}

void doubleArraySize(char ***array, int *capacity)
{
/* original array initialization:
  *array = (char **)malloc(*capacity * sizeof(char*));
*/
  /* declare temporary array */
  int i;
  char **tempArray;

  /* allocate space for temporary array */
  tempArray = (char **)malloc((*capacity) * 2 * sizeof(char*));

  /* copy values into temp array */
  for (i = 0; i < (*capacity); i++)
  {
    tempArray[i] = (*array)[i];
  }

  /* free array, as it is no longer needed */
  free(*array);

  /* make array point to the temp array and double the capacity */
  *array = tempArray;
  *capacity *= 2;
}

void menu(char ***array, int *wordCount, int *capacity)
{
  int exit;
  char option;
  char word[WORD_LENGTH];
  
  exit = 0;
  do 
  {
    printf("\nMenu\n");
    printf("'S'earch, 'I'nsert, 'R'emove, 'C'ount, 'P'rint, 'Q'uit\n");
    scanf("%c", &option);
    /* takes in char + newline character
       see "newline handler" below
    */
    if (option == 's' || option == 'S')
    {
      /* call search() */
      printf("\nSearch\n");
      searchWord(array, wordCount);
    } 
    else if (option == 'i' || option == 'I') 
    {
      /* call insert() */
      printf("\nInsert\n");
      printf("\nEnter a word: ");
      scanf("%s", word);
      insertWord(array, wordCount, word, capacity);
      printf("\nWord inserted.\n");
    } 
    else if (option == 'r' || option == 'R') 
    {
      /* call remove() */
      printf("\nRemove\n");
      removeWord(array, wordCount);
    } 
    else if (option == 'c' || option == 'C') 
    {
      /* call count() */
      printf("\nCount\n");
      printCount(*wordCount);
    } 
    else if (option == 'p' || option == 'P') 
    {
      printf("\nPrint\n");
      printArray(*array, *wordCount);
    } 
    else if (option == 'q' || option == 'Q') 
    {
      exit = 1;
    } 
    else 
    {
      /* invalid entry */
      printf("\nInvalid Entry, returning to main menu\n");
    }
    /* newline handler */
    while (option != '\n') 
    {
      scanf("%c", &option);
    }
  } 
  while (exit == 0);
}

int searchWord(char ***array, int *count) {

/*FIXME*/
  int i;
  int flag;
  char word[WORD_LENGTH];

  printf("\nEnter a word: ");
  scanf("%s", word);

  flag = 0;
  if (word == NULL) 
  {
    printf("\nInvalid Entry, returning to main menu\n");
  } 
  else 
  {
    for (i = 0; i < *count; i++) 
    {
      /* printf("\nChecking %s at \t%d", array[i], i); */
      if ((*array)[i] != NULL && strcmp((*array)[i], word) == 0) 
      {
        flag = 1;
        printf("\n'%s' found in array", word);
      }
    }
    if (flag == 0)
      printf("\n'%s' not found in array", word);
  }
  return 0;
}

int insertWord(char ***array, int *count, char word[], int *capacity)
{
    char *wordPtr;
    int wordPosition;
    int i;
    int compare;
    
    wordPtr = (char *)malloc((strlen(word) + 1) * sizeof(char));
    if (wordPtr == NULL)
    {
	    fprintf(stderr,"    Malloc of array[%d] failed!\n", *count);
	    return -1;
    }
    /* Memory for this word has been allocated, so copy characters
       and insert into array */

    strcpy(wordPtr, word);

/* Code to sort the pointers "on arrival" */

    i = 0;
    wordPosition = 0;
    
    /* check if the array is empty */
    if (*count > 0) 
    {
      /* We want to stay inside the array. Our first condition
         checks the position "i" that we are accessing to make sure
         it isn't greater than the number of words in the array
         (count), because otherwise array[i] will be a null pointer.
         Then we compare the strings. We keep going until we reach
         a case where strcmp(array[i], wordPtr) >= 0
      */
      while (i < *count && (*array)[i] != NULL) 
      {
        compare = strcmp((*array)[i], wordPtr);
        if (compare == 0)
        {
          printf("Error: word already in dictionary\n");
          free(wordPtr);
          return 1;
        }
        else if (compare > 0)
        {
          break;
        }
        i++;
      }      
      
      wordPosition = i;
      
      /*printf("Inserting %s at\t%d\n", word, wordPosition);*/
      
      for (i = *count; i >= wordPosition; i--) 
      {
        /* begin by shifting the last word (that is, the word
           at array[*count]) down by the length of wordPtr;
           then, shift the previous word down by length of wordPtr;
           keep going until you get to the place where the word
           is supposed to go (array[wordPosition]) and insert the
           word there
        */
        (*array)[i+1] = (*array)[i];
      }
    }
    (*array)[wordPosition] = wordPtr;
    (*count)++;
    return 0;
}

int removeWord(char ***array, int *count) 
{
/*FIXME*/

  int i;
  int flag;
  char word[WORD_LENGTH];

  printf("\nEnter a word: ");
  scanf("%s", word);

  flag = 0;
  if (word == NULL) 
  {
    printf("\nInvalid Entry, returning to main menu\n");
  } 
  else 
  {
    for (i = 0; i < *count; i++) 
    {
      /* printf("\nChecking %s at \t%d", array[i], i); */
      if ((*array)[i] != NULL && strcmp((*array)[i], word) == 0) 
      {
        free((*array)[i]);
        flag = 1;
        printf("\nWord removed");
      }
      if (flag == 1) /* word has been found and removed */
        (*array)[i] = (*array)[i+1];
    }
    if (flag == 0) 
    {
      printf("\nWord not in array");
    } 
    else if (flag == 1) 
    {
      (*count)--;
    }
  }
  return 0;
}

int printCount(int count) 
{
  printf("Total words in dictionary: %d\n", count);
  return 0;
}

int writeToFile (char *outFileName, char ***array, int *count) 
{

  int i;
  FILE *outFile;

  if ((outFile = fopen(outFileName, "w")) == NULL)
  {
    fprintf(stderr,"Error opening input file, %s\n", outFileName);
	  return -1;
  }

  for (i = 0; i < *count; i++)
  {
    if ((*array)[i] != NULL && (fputs((*array)[i], outFile) == EOF || fputs("\n", outFile) == EOF))
    {
      return -1;
    }
  }

  if (fclose(outFile) == EOF) return -1;

  return 0;
}
