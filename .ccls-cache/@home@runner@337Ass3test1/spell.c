#include "spell.h"

/**
 * The main function of the spell checker program.
 * It prompts the user for the input file and dictionary file paths,
 * checks each word in the input file against the dictionary, and
 * reports any words not found in the dictionary.
 *
 * @return int Returns 0 upon successful completion.
 */
int main() {
  char inputFilePath[256];
  char dictFilePath[256];
  char **dictionary;
  int dictSize;

  // Prompt user for the dictionary file
  while (!0) {
    printf("Enter the name of the dictionary file: ");
    fgets(dictFilePath, sizeof(dictFilePath), stdin);
    dictFilePath[strcspn(dictFilePath, "\n")] = '\0';

    dictSize = parseDict(dictFilePath, &dictionary);

    if (dictSize > 0) {
      break;
    } else {
      printf("Invalid dictionary file. Please try again.\n");
    }
  }

  // Prompt user for the input file
  FILE *inputFile;
  while (!inputFile) {
    printf("Enter the name of the input file: ");
    fgets(inputFilePath, sizeof(inputFilePath), stdin);
    inputFilePath[strcspn(inputFilePath, "\n")] = '\0';

    inputFile = fopen(inputFilePath, "r");
    if (!inputFile) {
      printf("Invalid input file. Please try again.\n");
    }
  }

  int lineNum = 0;
  int totalMisspelled = 0;
  char line[1024];
  while (fgets(line, sizeof(line), inputFile)) {
    lineNum++;

    char *token = strtok(line, " \t\n\r");
    while (token) {
      // Remove trailing non-alphabetic characters
      while (strlen(token) > 0 && !isalpha(token[strlen(token) - 1])) {
        token[strlen(token) - 1] = '\0';
      }

      if (strlen(token) > 0 && !wordSearch(token, dictionary, dictSize)) {
        bad_word(token, lineNum, stdout);
        totalMisspelled++;
      }

      token = strtok(NULL, " \t\n\r");
    }
  }

  printf("Total number of misspelled words: %d\n", totalMisspelled);

  // Cleanup
  fclose(inputFile);
  for (int i = 0; i < dictSize; i++) {
    free(dictionary[i]);
  }
  free(dictionary);
  return 0;
}
