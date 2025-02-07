/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:54:29 by jrinaudo          #+#    #+#             */
/*   Updated: 2025/02/07 22:18:28 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_alive(t_philo *philo)
{
	long	duration;
	duration = get_time_elapsed(philo->last_eat);
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
	int val = 0;
	while (philo->forks_in_hand)
	{
		sem_post(philo->table->forks);
		sem_getvalue(philo->table->forks, &val);
		printf(""RED"%d has released a fork now ther is %d forks"RESET"\n", philo->id, val);
		philo->forks_in_hand--;
	}
}

static int	take_fork(t_philo *philo)
{
	int val = 0;
	if (philo->table->nb_philo == 1 && philo->forks_in_hand == 1)
	{
		printf("%d is alone and has taken a fork\n", philo->id);
		my_sleep(philo->table->time_die + 1);
		return (release_fork(philo), 1);
	}
	sem_getvalue(philo->table->forks, &val);
	printf("forks = %d\n", val);
	printf("%d is trying to take a fork\n", philo->id);
	sem_wait(philo->table->forks);
	printf("%d has taken a fork\n", philo->id);
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
	int val = 0;

	sem_getvalue(philo->table->message, &val);
	printf(""YELLOW"message = %d and %d try to talk"RESET"\n", val, philo->id);	
	sem_wait(philo->table->message);
	if (!is_alive(philo))
	{
		sem_post(philo->table->message);
		exit (1);
	}
	printf(""YELLOW"message = %d and %d talk"RESET"\n", val, philo->id);	
	printf(""BG_WHITE GREY"%ld "BLUE"%d "RESET"%s\n",
		get_time_elapsed(philo->table->time_start), philo->id, msg);
	sem_post(philo->table->message);
}

int	eat_philo(t_philo *philo)
{
	if (!is_alive(philo))
		exit (1);
	message(philo, GREEN"is eating"RESET);
	philo->last_eat = get_time_elapsed(philo->last_eat);
	my_sleep(philo->table->time_eat);
	philo->nb_eat++;
	if (philo->nb_eat == philo->table->eat_limit)
		exit (0);
	philo->last_eat = get_time_elapsed(philo->last_eat);
	release_fork(philo);
	return (0);
}

void	*to_be_or_not_to_be(void *arg)
{
	t_philo	*philo;
/* 	int		val;
 */
	philo = (t_philo *)arg;
	/* sem_getvalue(philo->table->message, &val);
	printf("message = %d\n", val); */
	while (1)
	{
		if (philo->id % 2 == 1)
			my_sleep(philo->table->time_eat - 1);
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
			my_sleep(philo->table->time_sleep);
			message(philo, ORANGE"is thinking"RESET);
		}
	}
	exit (0);
}
