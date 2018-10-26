#ifndef __RCOM_UTIL_H
#define __RCOM_UTIL_H

#define MIN(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

int findChar(char *str, char c);

int findLastChar(char *str, char c);

int findChar_ignore_2(char *str, char c);

int findCharCount(char *str, char c);

int fileExists(char *fname);

#endif /* __RCOM_UTIL_H */
