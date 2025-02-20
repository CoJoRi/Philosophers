/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 19:10:12 by jrinaudo          #+#    #+#             */
/*   Updated: 2025/02/15 18:06:55 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/**
 * Returns current time in milliseconds since Unix epoch
 * 
 * Uses gettimeofday() to get current time and converts it to milliseconds.
 * The time is calculated by converting seconds to milliseconds and adding
 * microseconds converted to milliseconds.
 *
 * @return Current time in milliseconds
 */
long	my_clock(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long)tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

/**
 * Calculates the elapsed time since a given start time
 * 
 * @param time_start The starting time in my_clock ticks
 * @return The number of my_clock ticks elapsed since time_start
 * 
 * Note: Uses my_clock() function to measure CPU time in my_clock ticks
 */
long	get_time_elapsed(long time_start)
{
	return (my_clock() - time_start);
}

/*
** Pauses execution for a specified duration while monitoring philosopher's state
**
** @param duration   Time in microseconds to pause execution
** @param philo     Pointer to philosopher structure
**
** @details         Uses my_clock() to measure elapsed time and sleeps in small 
**                 intervals (50 microseconds). If philosopher dies during sleep,
**                 releases fork and exits program.
**
** @exit            Exits with status 1 if philosopher dies during sleep
*/
void	my_sleep(int duration)
{
	long	time;

	time = my_clock();
	while ((my_clock() - time) < duration)
	{
		usleep(100);
	}
}
