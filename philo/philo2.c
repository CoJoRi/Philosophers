/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:41:43 by jrinaudo          #+#    #+#             */
/*   Updated: 2025/02/12 13:38:34 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Releases forks held by a philosopher
** @param philo: Pointer to philosopher structure
** 
** This function safely releases both forks (if held) by the philosopher:
** 1. Locks status mutex to ensure thread safety
** 2. If left fork was taken, unlocks left fork mutex
** 3. If right fork was taken, unlocks right fork mutex
** 4. Resets fork status flags to 0
** 5. Unlocks status mutex
*/
void	release_fork(t_philo *philo)
{
	if (philo->took_l_fork)
		pthread_mutex_unlock(philo->l_fork);
	if (philo->took_r_fork)
		pthread_mutex_unlock(philo->r_fork);
	philo->took_l_fork = 0;
	philo->took_r_fork = 0;
}

/**
 * Checks if the simulation should finish for a philosopher
 * 
 * @param philo Pointer to philosopher structure containing thread data
 * @return 1 if simulation should finish (philosopher died or table finished),
 *  0 otherwise
 * 
 * Locks status mutex to safely check finish conditions.
 * Checks both table finish flag and philosopher dead status.
 */
int	check_finish(t_philo *philo)
{
	int	finish;

	finish = 0;
	pthread_mutex_lock(&philo->table->status);
	if (philo->table->finish || philo->dead)
		finish = 1;
	pthread_mutex_unlock(&philo->table->status);
	return (finish);
}

/**
 * eat_philo - Function for a philosopher to eat.
 * @philo: Pointer to the philosopher structure.
 *
 * This function handles the eating process of a philosopher. It locks the
 * necessary mutexes to ensure thread safety, checks if the philosopher is
 * dead, and if so, releases the forks and returns. If the philosopher is
 * not dead, it updates the philosopher's eating status, increments the
 * number of times the philosopher has eaten, and checks if the philosopher
 * has reached the maximum number of meals. If all philosophers have reached
 * the maximum number of meals, it sets the finish flag. The function also
 * updates the last meal time and makes the philosopher sleep for the
 * specified eating time before releasing the forks.
 *
 * Return: 1 if the philosopher is dead, 0 otherwise.
 */
int	eat_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->status);
	if (philo->dead)
		return (release_fork(philo)
			, pthread_mutex_unlock(&philo->table->status), 1);
	pthread_mutex_unlock(&philo->table->status);
	pthread_mutex_lock(&philo->mut_last_meal);
	philo->last_eat = get_time(philo);
	pthread_mutex_unlock(&philo->mut_last_meal);
	message(philo, GREEN"is eating"RESET);
	my_sleep(philo->time_eat);
	pthread_mutex_lock(&philo->mut_last_meal);
	philo->nb_eat++;
	if (philo->nb_eat == philo->eat_max)
	{
		pthread_mutex_lock(&philo->table->status);
		philo->table->eat_max_ok++;
		pthread_mutex_unlock(&philo->table->status);
	}
	pthread_mutex_unlock(&philo->mut_last_meal);
	release_fork(philo);
	return (0);
}

/**
 * @brief Makes the philosopher sleep for a specified amount of time.
 *
 * This function locks the status mutex to check if the simulation has finished.
 * If the simulation has finished, it unlocks the mutex and returns immediately.
 * Otherwise, it unlocks the mutex, prints a message indicating that the
 *  philosopher
 * is sleeping, and then makes the philosopher sleep for the duration specified
 * in `philo->time_sleep`.
 *
 * @param philo A pointer to the philosopher structure.
 */
void	sleep_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->status);
	if (philo->table->finish)
	{
		pthread_mutex_unlock(&philo->table->status);
		return ;
	}
	pthread_mutex_unlock(&philo->table->status);
	message(philo, YELLOW"is sleeping"RESET);
	my_sleep(philo->time_sleep);
}

void	think_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->status);
	if (philo->table->finish)
	{
		pthread_mutex_unlock(&philo->table->status);
		return ;
	}
	pthread_mutex_unlock(&philo->table->status);
	message(philo, ORANGE"is thinking"RESET);
}
