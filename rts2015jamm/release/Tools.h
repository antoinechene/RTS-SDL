#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <vector>

char*	getword(char* str);
int	getnum(char* str);
int	char_match(char c, char* charset);
int	id_count_word_charset(char* str, char* charset);
char**	id_str_to_word_tab_char(char* str, char* charset);
char*	id_itoa(int n);
#endif
