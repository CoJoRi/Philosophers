/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 19:10:12 by jrinaudo          #+#    #+#             */
/*   Updated: 2025/02/06 19:05:32 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Check if the input is a digitF
 * 
 * @param argv The input
 * @return int 1 if not a digit, 0 if it is
 */
int	check_input(char *argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		if (argv[i] < '0' || argv[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

/**
 * @brief Initializes and returns the current time in milliseconds.
 *
 * This function retrieves the current time using the gettimeofday function,
 * and converts it to milliseconds. The time is returned as a long integer.
 *
 * @return The current time in milliseconds.
 */
long	init_time_start(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long)tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

/**
 * @brief Initializes the start times for the table and each philosopher.
 *
 * This function sets the start time for the table and then iterates through
 * each philosopher, setting their individual start times.
 *
 * @param table A pointer to the table structure containing philosophers.
 */
void	init_times(t_table *table)
{
	int	i;

	i = 0;
	table->time_start = init_time_start();
	while (i < table->nb_philo)
	{
		table->philos[i].time_start = init_time_start();
		i++;
	}
}

/* 
tv.tv_sec
tv.tv_usec
display table->time_start
philo->time_start
 */