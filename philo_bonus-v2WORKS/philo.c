/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:54:29 by jrinaudo          #+#    #+#             */
/*   Updated: 2025/02/18 08:57:03 by jrinaudo         ###   ########.fr       */
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
	sem_wait(philo->table->status);
	if (philo->finish == 1)
	{
		sem_post(philo->table->status);
		sem_post(philo->table->message);
		return ;
	}
	printf(""BG_WHITE GREY"%ld "BLUE"%d "RESET"%s\n",
		get_time_elapsed(philo->table->time_start), philo->id, msg);
	sem_post(philo->table->status);
	sem_post(philo->table->message);
}

int	eat_philo(t_philo *philo)
{
	message(philo, GREEN"is eating"RESET);
	philo->last_eat = my_clock();
	sem_wait(philo->table->status);
	philo->next_eat = philo->last_eat + philo->table->time_die;
	sem_post(philo->table->status);
	my_sleep(philo->table->time_eat);
	philo->nb_eat++;
	if (philo->nb_eat == philo->table->eat_limit)
	{
		sem_post(philo->table->finish_eat);
		printf("finish eat[%d]\n", philo->id);
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
		sem_wait(philo->table->status);
		if (philo->finish == 1)
		{
			//printf("finish detect in routine[%d]\n", philo->id);
			return (sem_post(philo->table->status), NULL);
		}
		sem_post(philo->table->status);
		if (philo->id % 2 == 1)
			message(philo, ORANGE"is thinking"RESET);
		take_fork(philo);
		take_fork(philo);
		if (philo->forks_in_hand == 2)
		{
			eat_philo(philo);
			message(philo, YELLOW"is sleeping"RESET);
			my_sleep(philo->table->time_sleep);
			if (philo->id % 2 == 0)
				message(philo, ORANGE"is thinking"RESET);
		}
	}
	return (NULL);
}
