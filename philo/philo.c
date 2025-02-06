/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:54:29 by jrinaudo          #+#    #+#             */
/*   Updated: 2025/02/05 22:51:36 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	my_sleep(int duration)
{
	long	time;

	time = init_time_start();
	while (init_time_start() - time < duration)
		usleep(50);
}


static int	take_left_fork(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	philo->took_l_fork = 1;
	message(philo, BLUE"has taken a fork"RESET);
	return (0);
}

static int	take_right_fork(t_philo *philo)
{
	if (!philo->r_fork)
	{
		my_sleep(philo->time_die + 1);
		return (1);
	}
	pthread_mutex_lock(philo->r_fork);
	philo->took_r_fork = 1;
	message(philo, BLUE_LIGHT"has taken a fork"RESET);
	return (0);
}

void	message(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->table->message);
	pthread_mutex_lock(&philo->table->status);
	if (philo->table->finish)
	{
		pthread_mutex_unlock(&philo->table->status);
		pthread_mutex_unlock(&philo->table->message);
		return ;
	}
	pthread_mutex_unlock(&philo->table->status);
	printf(""BG_WHITE GREY"%ld "BLUE"%d "RESET"%s\n",
		get_time(philo), philo->id, msg);
	pthread_mutex_unlock(&philo->table->message);
}

void	*turn(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (check_finish(philo) == 0)
	{
		if (philo->id % 2 == 0)
		{
			take_right_fork(philo);
			take_left_fork(philo);
		}
		else
		{
			take_left_fork(philo);
			take_right_fork(philo);
		}
		if (philo->took_l_fork && philo->took_r_fork)
		{
			eat_philo(philo);
			sleep_philo(philo);
			message(philo, ORANGE"is thinking"RESET);
		}
	}
	return (NULL);
}

/* GDB DEBUG
philo->table->threads[0]
philo->table->threads[1]
philo->table->threads[2]
philo->table->threads[3]
philo->dead
philo->table->finish
philo->table->forks
philo->table->nb_philo
philo->table->time_die
philo->table->time_eat
philo->table->time_sleep
philo->table->eat_max
philo->table->time_start
philo->table->tv
philo->table->philos
philo->table->message
philo->table->threads
philo->dead;
philo->last_eat;
philo->nb_eat
 */