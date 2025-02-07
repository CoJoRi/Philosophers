/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 19:10:12 by jrinaudo          #+#    #+#             */
/*   Updated: 2025/02/07 11:00:58 by jrinaudo         ###   ########.fr       */
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
	long	time_now;

	time_now = clock();
	return (time_now - time_start);
}

void	my_sleep(int duration)
{
	long	time;

	time = clock();
	while ((clock() - time) < duration)
		usleep(50);
}
