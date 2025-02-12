/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   medic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 12:44:51 by jrinaudo          #+#    #+#             */
/*   Updated: 2025/02/12 13:37:07 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Checks if the philosopher is still alive.
 *
 * This function determines if a philosopher is still alive by checking time
 * since their last meal and comparing it to their allowed time to die. If the
 * philosopher has exceeded their allowed time to die, the function marks the
 * philosopher as dead and sets the table's finish status.
 *
 * @param philo A pointer to the philosopher structure.
 * @return 1 if the philosopher is alive, 0 if the philosopher is dead.
 */
int	is_alive(t_philo *philo)
{
	long	time;
	long	time_die;

	pthread_mutex_lock(&philo->table->status);
	if (philo->table->finish || philo->dead)
	{
		pthread_mutex_unlock(&philo->table->status);
		return (0);
	}
	pthread_mutex_unlock(&philo->table->status);
	pthread_mutex_lock(&philo->mut_last_meal);
	time = get_time(philo) - philo->last_eat;
	time_die = philo->time_die;
	pthread_mutex_unlock(&philo->mut_last_meal);
	if (time > time_die)
	{
		message(philo, RED"died"RESET);
		pthread_mutex_lock(&philo->table->status);
		philo->table->finish = 1;
		philo->dead = 1;
		pthread_mutex_unlock(&philo->table->status);
		return (0);
	}
	return (1);
}

/**
 * @brief Monitors the state of the philosophers at the table.
 *
 * This function continuously checks the status of the philosophers to determine
 * if any philosopher has died or if the simulation should finish. It uses mutex
 * to safely access the shared table status and philosopher data.
 *
 * @param arg A pointer to the table structure containing the philosophers and
 *  their status.
 * @return NULL when the simulation is finished or a philosopher dies.
 */
void	*watch_table(void *arg)
{
	t_table	*table;
	int		i;

	table = (t_table *)arg;
	i = 0;
	while (1)
	{
		while (i < table->nb_philo)
		{
			my_sleep(1);
			pthread_mutex_lock(&table->status);
			if (table->eat_max_ok == table->nb_philo)
			{
				printf("max meal at: %ld\n", get_time(&table->philos[i]));
				table->finish = 1;
			}
			pthread_mutex_unlock(&table->status);
			if (is_alive(&table->philos[i]) == 0)
				return (NULL);
			i++;
		}
		i = 0;
	}
	return (NULL);
}
