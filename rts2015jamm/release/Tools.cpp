#include "Tools.h"
#include <string.h>
#include <stdlib.h>

char*	getword(char* str)
{
	size_t	i;
	size_t	j;
	size_t	k;
	bool	find;
	size_t	size;
	char*	ptr;

	size = strlen(str);
	ptr = new char[size + 1];
	if (ptr == NULL)
		return NULL;
	find = false;
	j = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == ' ' ||  str[i] == '\n' || str[i] == '\0' || str[i] == '\t')
		{
			ptr[i] = '\0';
			i++;
			find = true;
			break ;
		}
		ptr[i] = str[i];
		i++;
	}
	if (find)
	{
		k = i;
		while (i <= size)
		{
			str[j] = str[i];
			i++;
			j++;
		}
		return (ptr);
	}
	return NULL;
}

int	getnum(char* str)
{
	size_t	i;
	size_t	j;
	size_t	k;
	bool	find;
	size_t	size;
	char*	ptr;

	find = false;
	size = strlen(str);
	ptr = new char[size + 1];
	if (ptr == NULL)
		return -1;
	j = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == ' ' ||  str[i] == '\n' || str[i] == '\0' || str[i] == '\t')
		{
			ptr[i] = '\0';
			i++;
			find = true;
			break;
		}
		ptr[i] = str[i];
		i++;
	}
	if (find)
	{
		k = i;
		while (i <= size)
		{
			str[j] = str[i];
			i++;
			j++;
		}
		return (atoi(ptr));
	}
	else
		return (0);
}

int	char_match(char c, char* charset)
{
	int	i;

	i = 0;
	while (charset[i] != 0)
	{
		if (c == charset[i])
			return 1;
		i = i + 1;
	}
	return 0;
}

int	id_count_word_charset(char* str, char* charset)
{
	int     i;
	int     count_word;

	i = 0;
	count_word = 0;
	while (str[i] != 0)
	{
	        if (char_match(str[i], charset) == 1)
	                count_word = count_word + 1;
	        i = i + 1;
	}
	return count_word;
}

char**	id_str_to_word_tab_char(char* str, char* charset)
{
	int     i;
	int     count_word;
	int     l_str;
	char**  tab;

	l_str = id_count_word_charset(str, charset);
//	tab = (char**)malloc(sizeof(*tab) * (l_str + 2));
	tab = new char*[(sizeof(*tab) * (l_str + 2))];
	if (tab == NULL)
		return NULL;
	i = 0;
	count_word = 0;
	while (str[i] != 0)
	{
		if (char_match(str[i], charset) == 1)
	        {
	        	str[i] = 0;
		}
		else if (char_match(str[i], charset) == 0 && (str[i - 1] == 0 || i == 0))
	        {
	                tab[count_word] = &str[i];
	                count_word = count_word + 1;
	        }
	        i = i + 1;
	}
	tab[count_word] = 0;
	return tab;
}

char*	id_itoa(int n)
{
	char*	str;
	int	expo;
	int	i;

	str = new char[24];
	expo = 1;
	i = 0;
	if (n < 0)
	{
		str[0] = '-';
		i = i + 1;
	}
	while (n / expo > 9 || n / expo < -9)
		expo = expo * 10;
	while (expo > 0)
	{
		if (n < 0)
			str[i] = ((n / expo % 10) * -1 + 48);
		else
			str[i] = (n / expo % 10) + 48;
		i = i + 1;
		expo = expo / 10;
	}
	str[i] = '\0';
	return (str);
}


