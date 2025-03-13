/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 08:26:16 by jrinaudo          #+#    #+#             */
/*   Updated: 2025/02/15 11:34:28 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void    close_sem(t_philo *philo)
{
	sem_close(philo->table->forks);
	sem_close(philo->table->message);
	sem_close(philo->table->sem_exit);
	sem_close(philo->table->finish_eat);
	sem_close(philo->table->status);
	//printf("close_sem [%d]\n", philo->id);
}

void	*life_monitor(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		my_sleep(1);
		sem_wait(philo->table->status);
		if (my_clock() > philo->next_eat)
		{
			sem_post(philo->table->status);
			sem_wait(philo->table->message);
			//message(philo, RED"died"RESET);
			philo->finish = 1;
			printf("%ld %d "RED"died\n"RESET, my_clock() - philo->table->time_start, philo->id);
			sem_post(philo->table->sem_exit);
			my_sleep(5);
			return (NULL);
		}
		sem_post(philo->table->status);
	}
	return (NULL);
}

void *stop_philo(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	sem_wait(philo->table->sem_exit);
	sem_wait(philo->table->status);
	philo->finish = 1;
	//sem_post(philo->table->message);
	sem_post(philo->table->status);
	//printf("stop_philo [%d]\n", philo->id);
	//close_sem(philo);
	//kill(philo->pid, SIGKILL);
	return (NULL);
}
