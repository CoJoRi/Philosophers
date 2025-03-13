/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:54:29 by jrinaudo          #+#    #+#             */
/*   Updated: 2025/02/16 12:12:59 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	take_fork(t_philo *philo)
{
	if (philo->table->nb_philo == 1 && philo->forks_in_hand == 1)
	{
		my_sleep(philo->table->time_die + 1);
		return (1);
	}
	sem_wait(philo->table->forks);
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
	message(philo, GREEN"is eating"RESET);
	sem_wait(philo->table->status);
	philo->last_eat = my_clock();
	sem_post(philo->table->status);
	my_sleep(philo->table->time_eat);
	philo->nb_eat++;
	if (philo->table->eat_limit != -1 && philo->nb_eat == philo->table->eat_limit)
	{
		philo->eat_enough = 1;
		message(philo, PURPLE"has eaten enough"RESET);
		sem_post(philo->table->finish_eat);
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
			message(philo, ORANGE"is thinking"RESET);
		take_fork(philo);
		take_fork(philo);
		eat_philo(philo);
		message(philo, YELLOW"is sleeping"RESET);
		my_sleep(philo->table->time_sleep);
		if (philo->id % 2 == 0)
			message(philo, ORANGE"is thinking"RESET);
	}
	return (NULL);
}
