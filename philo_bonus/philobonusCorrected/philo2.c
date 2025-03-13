/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:12:49 by jrinaudo          #+#    #+#             */
/*   Updated: 2025/03/06 13:28:15 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_alive(t_philo *philo)
{
	long	duration;

	duration = clock() - philo->last_eat;
	if (duration > philo->table->time_die)
	{
		sem_wait(philo->table->dead);
		sem_wait(philo->table->message);
		printf(""BG_WHITE GREY"%ld "BLUE"%d "RED" died\n"RESET,
		get_time_elapsed(philo->table->time_start), philo->id);
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
