/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:54:29 by jrinaudo          #+#    #+#             */
/*   Updated: 2025/02/07 14:01:47 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_alive(t_philo *philo)
{
	long	time;

	time = get_time_elapse(philo->last_eat);
		if (time > philo->table->time_die)
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
	sem_wait(philo->table->forks);
	if (is_alive(philo))
		exit (1);
	if (philo->table->nb_philo == 1 && philo->forks_in_hand == 1)
	{
		my_sleep(philo->table->time_die + 1);
		return (release_fork(philo), 1);
	}
	philo->forks_in_hand++;
	if (philo->forks_in_hand == 1)
		message(philo, BLUE_LIGHT"has taken a fork"RESET);
	else
		message(philo, BLUE_LIGHT"has taken a fork"RESET);
	return (0);
}

void	message(t_philo *philo, char *msg)
{
	sem_wait(philo->table->message);
	if (is_alive(philo))
	{
		sem_post(philo->table->message);
		exit (1);
	}
	printf(""BG_WHITE GREY"%ld "BLUE"%d "RESET"%s\n",
		get_time_elapsed(philo->table->time_start), philo->id, msg);
	sem_post(philo->table->message);
}
//modification de la fonction pour exit (0) quand le philo a fini de manger,
// le main va recuperer cette infos et kill tout ce beau mon
int	eat_philo(t_philo *philo)
{
	if (is_alive(philo))
		exit (1);
	message(philo, GREEN"is eating"RESET);
	philo->last_eat = get_time_elapsed(philo->table->time_start);
	my_sleep(philo->table->time_eat);
	philo->nb_eat++;
	if (philo->nb_eat == philo->table->eat_limit)
		exit (0);
	philo->last_eat = get_time_elapsed(philo->table->time_start);
	release_fork(philo);
	return (0);
}

void	*to_be_or_not_to_be(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		take_fork(philo);
		take_fork(philo);
			return (NULL);
		if (philo->forks_in_hand == 2)
		{
			eat_philo(philo);
			if (is_alive(philo))
				exit (1);
			message(philo, YELLOW"is sleeping"RESET);
			my_sleep(philo->table->time_sleep);
			if (is_alive(philo))
				exit (1);
			message(philo, ORANGE"is thinking"RESET);
		}
	}
	exit (0);
}
