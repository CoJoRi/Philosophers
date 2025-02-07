/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   medic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 12:44:51 by jrinaudo          #+#    #+#             */
/*   Updated: 2025/02/07 14:01:50 by jrinaudo         ###   ########.fr       */
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
			if (table->eat_max_ok == table->nb_philo)
				table->finish = 1;
			if (is_alive(&table->philos[i]) == 0)
				return (NULL);
			i++;
		}
		i = 0;
	}
	return (NULL);
}
