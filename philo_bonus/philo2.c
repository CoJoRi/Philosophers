/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:12:49 by jrinaudo          #+#    #+#             */
/*   Updated: 2025/02/09 21:36:57 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Checks if a philosopher is still alive based on time since last meal
 * 
 * @param philo Pointer to philosopher struct containing state information
 * 
 * @return 1 if philosopher is alive, 0 if philosopher has died
 * 
 * Calculates time elapsed since philosopher's last meal.
 * If elapsed time exceeds time_die limit:
 * - Acquires dead semaphore
 * - Prints death message
 * - Releases forks
 * - Returns 0 (dead)
 * Otherwise returns 1 (alive)
 */
int	is_alive(t_philo *philo)
{
	long	duration;

	duration = clock() - philo->last_eat;
	if (duration > philo->table->time_die)
	{
		sem_wait(philo->table->dead);
		message(philo, RED"died"RESET);
		release_fork(philo);
		return (0);
	}
	return (1);
}

/**
 * Release all forks currently held by a philosopher
 * The function posts to the semaphore for each fork the philosopher has,
 * effectively returning them to the table
 * 
 * @param philo Pointer to philosopher structure containing fork count and
 *  table info
 */
void	release_fork(t_philo *philo)
{
	while (philo->forks_in_hand)
	{
		sem_post(philo->table->forks);
		philo->forks_in_hand--;
	}
}
