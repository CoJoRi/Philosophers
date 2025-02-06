/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:05:48 by jrinaudo          #+#    #+#             */
/*   Updated: 2025/02/05 09:26:31 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_table	table;
	int		i;

	if (argc < 5)
		return (write(2, "Error args -> need 4 minimum\n", 21), 1);
	i = 0;
	if (init_table(&table, argv, argc))
		return (1);
	pthread_join(table.medic, NULL);
	while (i < table.nb_philo)
		pthread_join(table.threads[i++], NULL);
	free_mutex(&table);
	return (0);
}
/* 
table->time_start
table.tv.tv_sec * 1000 + table.tv.tv_usec / 1000
 */