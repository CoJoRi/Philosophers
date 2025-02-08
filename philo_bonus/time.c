/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 19:10:12 by jrinaudo          #+#    #+#             */
/*   Updated: 2025/02/08 22:30:01 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	clock(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long)tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

long	get_time_elapsed(long time_start)
{
	return (clock() - time_start);
}

void	my_sleep(int duration, t_philo *philo)
{
	long	time;

	time = clock();
	while ((clock() - time) < duration)
	{
		if (!is_alive(philo))
		{
			release_fork(philo);
			exit(1);
		}
		usleep(50);
	}
}
/* {
	long	time;

	time = clock();
	while ((clock() - time) < duration)
		usleep(10);
} */
