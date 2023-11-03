#include "spell_lib.h"

/**
 * Parses the dictionary file and populates the dictionary array.
 *
 * @param pathname The path to the dictionary file.
 * @param output A pointer to the array that will hold the dictionary words.
 *
 * @return int Returns the number of words in the dictionary.
 * Returns 0 if the file could not be opened or memory allocation failed.
 */
int parseDict(char *pathname, char ***output) {
  FILE *dictFile = fopen(pathname, "r");
  if (!dictFile)
    return 0;

  int wordCount = 0;
  char line[256];

  // First pass: count words
  while (fgets(line, sizeof(line), dictFile)) {
    char *token = strtok(line, " \t\n");
    if (token)
      wordCount++;
  }

  // Allocate memory for dictionary based on word count
  *output = (char **)malloc(sizeof(char *) * wordCount);
  if (!*output) {
    fclose(dictFile);
    return 0;
  }

  // Reset file pointer to beginning for second pass
  rewind(dictFile);

  // Second pass: store words
  int idx = 0;
  while (fgets(line, sizeof(line), dictFile) && idx < wordCount) {
    char *token = strtok(line, " \t\n\r");
    if (token) {
      (*output)[idx] = strdup(token);
      idx++;
    }
  }

  fclose(dictFile);
  return wordCount;
}

/**
 * Searches for a word in the dictionary.
 *
 * @param word The word to be searched.
 * @param dict The dictionary array.
 * @param dictSize The number of words in the dictionary.
 *
 * @return int Returns 1 if the word is found in the dictionary, 0 otherwise.
 */
int wordSearch(char *word, char **dict, int dictSize) {
  if (!word || !dict || dictSize <= 0)
    return 0;

  for (int i = 0; i < dictSize; i++) {
    if (strcasecmp(word, dict[i]) == 0) {
      return 1;
    }
  }
  return 0;
}

/**
 * Prints an error message indicating an unknown word in the input file.
 *
 * @param word The unknown word.
 * @param line The line number in the input file where the word was found.
 * @param out The file stream where the error message should be printed.
 *
 * @return int Returns 1 if the message is printed successfully, 0 otherwise.
 */
int bad_word(char *word, int line, FILE *out) {
  if (!out)
    return 0;

  fprintf(out, "Unknown word %s found in line %d of the input file\n", word,
          line);
  return 1;
}
