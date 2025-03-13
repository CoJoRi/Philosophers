/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 08:26:16 by jrinaudo          #+#    #+#             */
/*   Updated: 2025/03/13 09:49:34 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/**
 * a termoner pour fonctionner ajouter un nmonitoring dans les forkj qui attend
 *  le semwait de finished, quand tous les  onde a mange + gerer la fermeture du
 *  thread monitoring avant de quitter
 * 
 * 
 */

void    close_sem(t_philo *philo)
{
	sem_close(philo->table->status);
	sem_close(philo->table->forks);
	sem_close(philo->table->message);
	sem_close(philo->table->sem_exit);
	sem_close(philo->table->finish_eat);
}

void	*life_monitor(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->table->status);
		if (philo->dead == 1)
		{
			sem_post(philo->table->status);
			return (NULL);
		}
		if ((my_clock() - philo->last_eat) > philo->table->time_die)
		{
			sem_wait(philo->table->message);
			printf("%ld %d "RED"died\n"RESET, my_clock() - philo->table->time_start, philo->id);
			close_sem(philo);
			exit(1);
			return (NULL);
		}
		sem_post(philo->table->status);
	}
	return (NULL);
}

