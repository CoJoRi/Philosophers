/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:54:29 by jrinaudo          #+#    #+#             */
/*   Updated: 2025/02/08 14:03:43 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_alive(t_philo *philo)
{
	long	duration;
	duration = clock() - philo->last_eat;
	if (duration > philo->table->time_die)
	{
		message(philo, RED"died"RESET);
		release_fork(philo);
		return (0);
	}
	return (1);
}

void	release_fork(t_philo *philo)
{
	while (philo->forks_in_hand)
	{
		sem_post(philo->table->forks);
		philo->forks_in_hand--;
	}
}

static int	take_fork(t_philo *philo)
{
	if (philo->table->nb_philo == 1 && philo->forks_in_hand == 1)
	{
		my_sleep(philo->table->time_die + 1, philo);
		return (release_fork(philo), 1);
	}
	sem_wait(philo->table->forks);
	if (!is_alive(philo))
	{
		release_fork(philo);
		exit (1);
	}
	philo->forks_in_hand++;
	if (philo->forks_in_hand == 1)
		message(philo, BLUE_LIGHT"has taken a fork"RESET);
	else
		message(philo, BLUE"has taken a fork"RESET);
	return (0);
}

void	message(t_philo *philo, char *msg)
{
	sem_wait(philo->table->message);
	printf(""BG_WHITE GREY"%ld "BLUE"%d "RESET"%s\n",
		get_time_elapsed(philo->table->time_start), philo->id, msg);
	sem_post(philo->table->message);
}

int	eat_philo(t_philo *philo)
{
	if (!is_alive(philo))
		exit (1);
	message(philo, GREEN"is eating"RESET);
	philo->last_eat = clock();
	my_sleep(philo->table->time_eat, philo);
	philo->nb_eat++;
	if (philo->nb_eat == philo->table->eat_limit)
	{
		release_fork(philo);
		exit (0);
	}
	release_fork(philo);
	return (0);
}

void	*to_be_or_not_to_be(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		if (philo->id % 2 == 1)
		{
			message(philo, ORANGE"is thinking"RESET);
			my_sleep(1, philo);
		}
		take_fork(philo);
		if (!is_alive(philo))
		{
			release_fork(philo);
			exit (1);
		}
		take_fork(philo);
		if (philo->forks_in_hand == 2)
		{
			eat_philo(philo);
			message(philo, YELLOW"is sleeping"RESET);
			my_sleep(philo->table->time_sleep, philo);
			message(philo, ORANGE"is thinking"RESET);
		}
	}
	exit (0);
}
