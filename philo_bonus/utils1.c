/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 14:12:08 by zeph              #+#    #+#             */
/*   Updated: 2025/02/07 09:38:59 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Converts a string to an integer.
 *
 * This function takes a string as input and converts it to an integer.
 * It handles optional leading whitespace, an optional sign, and numeric
 *  characters.
 * If the resulting integer exceeds the range of an int, it returns 0.
 *
 * @param str The string to be converted to an integer.
 * @return The converted integer value, or 0 if the conversion fails or the
 *  value is out of range.
 */
int	ft_atoi(const char *str)
{
	int		i;
	long	nb;
	int		sign;

	i = 0;
	nb = 0;
	sign = 1;
	if (*str == '\0')
		return (0);
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = (nb * 10) + (str[i] - '0');
		i++;
	}
	if ((nb * sign) > INT_MAX || nb * sign < INT_MIN)
		return (0);
	return (nb * sign);
}

