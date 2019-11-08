/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmills <kmills@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 23:54:47 by kmills            #+#    #+#             */
/*   Updated: 2019/04/13 14:34:48 by ergottli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		is_valid2(char c, int str_base)
{
	char digits[16];

	digits[0] = '0';
	digits[1] = '1';
	digits[2] = '2';
	digits[3] = '3';
	digits[4] = '4';
	digits[5] = '5';
	digits[6] = '6';
	digits[7] = '7';
	digits[8] = '8';
	digits[9] = '9';
	digits[10] = 'A';
	digits[11] = 'B';
	digits[12] = 'C';
	digits[13] = 'D';
	digits[14] = 'E';
	digits[15] = 'F';
	while (str_base--)
		if (digits[str_base] == c)
			return (1);
	return (0);
}

int		is_valid1(char c, int str_base)
{
	char digits[16];

	digits[0] = '0';
	digits[1] = '1';
	digits[2] = '2';
	digits[3] = '3';
	digits[4] = '4';
	digits[5] = '5';
	digits[6] = '6';
	digits[7] = '7';
	digits[8] = '8';
	digits[9] = '9';
	digits[10] = 'a';
	digits[11] = 'b';
	digits[12] = 'c';
	digits[13] = 'd';
	digits[14] = 'e';
	digits[15] = 'f';
	while (str_base--)
		if (digits[str_base] == c)
			return (1);
	return (0);
}

int		value_of(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	else if (c >= 'a' && c <= 'z')
		return (c - 'a' + 10);
	else if (c >= 'A' && c <= 'Z')
		return (c - 'A' + 10);
	return (0);
}

int		ft_atoi_base(const char *str, int str_base)
{
	int res;
	int sign;

	res = 0;
	sign = 1;
	while (*str == 32 || *str == '\n' || *str == '\t' || *str == '\v')
		++str;
	if (*str == '-')
	{
		sign = -1;
		++str;
	}
	else if (*str == '+')
		++str;
	while (is_valid1(*str, str_base) || is_valid2(*str, str_base))
	{
		res = res * str_base + value_of(*str);
		++str;
	}
	return (res * sign);
}
