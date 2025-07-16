#include <getopt.h>
#include <locale.h>
#include <malloc.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>

struct flags_s {
  int pattern;             //-e / -f
  int ignore_case;         //-i
  int invert_match;        //-v
  int count;               //-c
  int files_with_matches;  //-l
  int line_number;         //-n
  int without_filename;    //-h
  int without_errors;      //-s
  int only_match;          //-o
  int not_one_file;
};
typedef struct flags_s flags_t;

struct patterns_s {
  char **array;
  int capacity;
};
typedef struct patterns_s patterns_t;

int is_error(int status){
  return status == -1 ? 1 : 0;
}

int init_patterns(patterns_t *patterns) {
  patterns->array = NULL;
  patterns->capacity = 0;
  return 0;
}

int add_pattern(patterns_t *patterns, char *pattern) {
  if(patterns == NULL || pattern == NULL)
    return -1;
  int status = 0;
  patterns->array = (char**)realloc(patterns->array, ++(patterns->capacity) * sizeof(char**));
  if(patterns->array == NULL)
    status = -1;
  char *new_pattern = NULL;
  if(!is_error(status))
    new_pattern = (char *)malloc(sizeof(char) * (strlen(pattern) + 1));
  if(new_pattern == NULL)
    status = -1;
  if(!is_error(status)) { 
    strcpy(new_pattern, pattern);
    patterns->array[patterns->capacity - 1] = new_pattern;
  }
  return status;
}

void free_patterns(patterns_t *patterns) {
  for (int i = 0; i < patterns->capacity && patterns->array; i++) {
    free(patterns->array[i]);
  }
  if (patterns->array) free(patterns->array);
  patterns->array = NULL;
  patterns->capacity = 0;
}

char *get_string(FILE *file);

int is_file_empty(FILE *file) {
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);
  return (size == 0);
}

int add_patterns_from_file(patterns_t *patterns, const char *path) {
  int flag = 1;
  FILE *file = NULL;
  if ((file = fopen(path, "r")) == NULL) {
    printf("Unknow file: %s", path);
    flag = 0;
  }
  char *line;
  int empty = is_file_empty(file);
  while ((line = get_string(file)) != NULL && !empty) {
    if (line) {
      add_pattern(patterns, line);
      free(line);
    }
  }
  if (line) free(line);
  if (file) fclose(file);
  return flag;
}

int parse_args(int argc, char *argv[], flags_t *flags, patterns_t *patterns) {
  int flag = 1;
  opterr = 0;
  int opt;
  while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1 && flag) {
    switch (opt) {
      case 'e':
        flags->pattern = 1;
        add_pattern(patterns, optarg);
        break;
      case 'i':
        flags->ignore_case = 1;
        break;
      case 'v':
        flags->invert_match = 1;
        break;
      case 'c':
        flags->count = 1;
        break;
      case 'l':
        flags->files_with_matches = 1;
        break;
      case 'n':
        flags->line_number = 1;
        break;
      case 'h':
        flags->without_filename = 1;
        break;
      case 's':
        flags->without_errors = 1;
        break;
      case 'f':
        flag = add_patterns_from_file(patterns, optarg);
        flags->pattern = 1;
        break;
      case 'o':
        flags->only_match = 1;
        break;
      default:
        printf("Unknown option\n");
        flag = 0;
        break;
    }
  }
  if (flags->invert_match && flags->only_match) flag = 0;
  return flag;
}

int regex_compile(patterns_t patterns, flags_t flags, regex_t **regexes) {
  int flag = 1;
  *regexes = (regex_t *)malloc(patterns.capacity * sizeof(regex_t));
  int regex_flags = REG_EXTENDED;
  if (flags.ignore_case) {
    regex_flags |= REG_ICASE;
  }
  for (int i = 0; i < patterns.capacity && flag; i++) {
    int ret = regcomp(&(*regexes)[i], patterns.array[i], regex_flags);
    if (ret) {
      char error_msg[100];
      regerror(ret, &(*regexes)[i], error_msg, sizeof(error_msg));
      fprintf(stderr, "Error '%s': %s\n", patterns.array[i], error_msg);
      for (int j = 0; j < i; j++) {
        regfree(&(*regexes)[j]);
      }
      free(*regexes);
      *regexes = NULL;
      flag = 0;
    }
  }
  return flag;
}

void free_regex(regex_t **regexes, int count) {
  for (int i = 0; i < count; i++) regfree(&(*regexes)[i]);
  free(*regexes);
}

char *get_string(FILE *file) {
  if (file == NULL) return NULL;
  int success = 1;
  int capacity = 1024;
  int length = 0;
  int ch = 0;
  char *buffer = (char *)malloc(capacity * sizeof(char));
  if (buffer != NULL) {
    ch = fgetc(file);
    while (ch != EOF && ch != '\n' && success) {
      if (length + 1 >= capacity) {
        char *temp = realloc(buffer, capacity * 2 * sizeof(char));
        if (temp != NULL) {
          buffer = temp;
          capacity *= 2;
        } else {
          success = 0;
        }
      }
      if (success) {
        buffer[length] = (char)ch;
        length++;
        ch = fgetc(file);
      }
    }
    if (!success || (length == 0 && ch != '\n')) {
      free(buffer);
      buffer = NULL;
    } else {
      buffer[length] = '\0';
    }
  }
  return buffer;
}

int find_match(const char *line, flags_t flags, regex_t *regexes,
               int count_of_regex) {
  for (int i = 0; i < count_of_regex; i++) {
    int ret = regexec(&regexes[i], line, 0, NULL, 0);
    int match = (ret == 0);
    if (flags.invert_match) match = !match;
    if (match) return 1;
  }
  return 0;
}

void print_only_matches(const char *line, regex_t *regexes, int regex_count,
                        const char *path, int line_number, flags_t flags) {
  for (int i = 0; i < regex_count; i++) {
    const char *cursor = line;
    regmatch_t pmatch[1];
    while (regexec(&regexes[i], cursor, 1, pmatch, 0) == 0) {
      int start = pmatch[0].rm_so;
      int end = pmatch[0].rm_eo;
      if (start == end || start == -1) break;
      if (flags.not_one_file && !flags.without_filename) {
        printf("%s:", path);
      }
      if (flags.line_number) {
        printf("%d:", line_number);
      }
      for (int j = start; j < end; j++) {
        putchar(cursor[j]);
      }
      putchar('\n');
      cursor += end;
    }
  }
}

void process_file(const char *path, flags_t flags, regex_t *regexes,
                  int count_of_regex) {
  int flag = 1;
  FILE *file = NULL;
  if ((file = fopen(path, "r")) == NULL) {
    if (!flags.without_errors) printf("Unknow file: %s\n", path);
    flag = 0;
  };
  char *line;
  int line_number = 0;
  int file_has_matches = 0;
  while ((line = get_string(file)) != NULL && flag) {
    line_number++;
    int match = find_match(line, flags, regexes, count_of_regex);
    if (match) {
      file_has_matches++;
      if (flags.files_with_matches) break;
      if (!flags.count) {
        if (flags.only_match) {
          print_only_matches(line, regexes, count_of_regex, path, line_number,
                             flags);
        } else {
          if (flags.not_one_file && !flags.without_filename)
            printf("%s:", path);
          if (flags.line_number) printf("%d:", line_number);
          printf("%s\n", line);
        }
      }
    }
    if (line) free(line);
  }
  if (flag) {
    if (flags.files_with_matches) {
      if (file_has_matches) printf("%s\n", path);
    } else if (flags.count) {
      if (flags.not_one_file && !flags.without_filename) printf("%s:", path);
      printf("%d\n", file_has_matches);
    }
    fclose(file);
  }
}

int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "ru_RU.UTF-8");
  flags_t flags = {0};
  patterns_t patterns;
  init_patterns(&patterns);
  int flag = parse_args(argc, argv, &flags, &patterns);
  if (!flags.pattern && optind < argc) {
    add_pattern(&patterns, argv[optind++]);
    flags.pattern = 1;
  }
  if (!flags.pattern) flag = 0;
  regex_t *regexes = NULL;
  if (flag) flag = regex_compile(patterns, flags, &regexes);
  if (argc - optind > 1) {
    flags.not_one_file = 1;
  }
  for (int i = optind; i < argc && flag; i++) {
    process_file(argv[i], flags, regexes, patterns.capacity);
  }
  if (regexes) free_regex(&regexes, patterns.capacity);
  if (patterns.array) free_patterns(&patterns);
}
