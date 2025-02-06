/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 11:48:32 by jrinaudo          #+#    #+#             */
/*   Updated: 2025/02/05 21:16:36 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Frees all mutexes associated with the philosophers' table.
 *
 * This function prints a message indicating the end of the dinner and 
 * iterates through each philosopher, printing the number of times they 
 * have eaten and whether they have died. It then destroys all mutexes 
 * associated with the forks, message, status, and time.
 *
 * @param table A pointer to the t_table structure containing the 
 * philosophers and mutexes to be freed.
 */
void	free_mutex(t_table *table)
{
	int	i;

	i = 0;
	printf(GREEN"\n\t_______________________\n"RESET);
	printf(GREEN"\t|                     |\n"RESET);
	printf(GREEN"\t|  the dinner is over |\n"RESET);
	printf(GREEN"\t|_____________________|\n"RESET);
	while (i < table->nb_philo)
	{
		printf("\t  %d has eaten %d times", table->philos[i].id,
			table->philos[i].nb_eat);
		if (table->philos[i].dead)
			printf(" but died... RIP Him\n");
		else
			printf("\n");
		pthread_mutex_destroy(&table->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&table->message);
	pthread_mutex_destroy(&table->status);
}
