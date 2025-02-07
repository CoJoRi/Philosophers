/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:05:48 by jrinaudo          #+#    #+#             */
/*   Updated: 2025/02/07 14:00:22 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	print_finish(t_table *table)
{
	int	i;

	i = 0;
	printf(GREEN"\n\t_______________________\n"RESET);
	printf(GREEN"\t|                     |\n"RESET);
	printf(GREEN"\t|  the dinner is over |\n"RESET);
	printf(GREEN"\t|_____________________|\n"RESET);
	while (i < table->nb_philo)
	{
		printf("\t  %d has eaten %d times", i,
			table->philos[i].nb_eat);
		if (table->philos[i].dead)
			printf(" but died... RIP Him\n");
		else
			printf("\n");
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_table	table;
	//pthread_t	medic;
	int		i;

	if (argc < 5)
		return (write(2, "Error args -> need 4 minimum\n", 21), 1);
	i = 0;
	if (init_table(&table, argv + 1, argc - 1))
		return (1);
	//pthread_create(&medic, NULL, watch_table, &table);
	while (i < table.nb_philo)
	{
		table.philos[i].pid_philo = fork();
		if (table.philos[i].pid_philo == 0)
			to_be_or_not_to_be(&table.philos[i]);
		i++;
	}
	i = 0;
	while (i < table.nb_philo)
		waitpid(table.philos[i++].pid_philo, NULL, 0);
	print_finish(&table);
	sem_close(table.forks);
	sem_unlink("/baguette");
	sem_close(table.message);
	sem_unlink("/message_semaphore");
	return (0);
}
/* controle des valeurs de la structure table
	printf("nb_philo = %d\n", table.nb_philo);
	printf("time_start = %ld\n", table.time_start);
	printf("time_die = %d\n", table.time_die);
	printf("time_eat = %d\n", table.time_eat);
	printf("time_sleep = %d\n", table.time_sleep);
	printf("eat_limit = %d\n", table.eat_limit);
	printf("eat_max_ok = %d\n", table.eat_max_ok);
	printf("finish = %d\n", table.finish);
	printf("forks = %p\n", table.forks);
	printf("message = %p\n", table.message);
	printf("philos = %p\n", table.philos);
	printf("_______________________________________\n");
	while (i < table.nb_philo)
	{
		printf("philos[%d].pid_philo = %d\n", i, table.philos[i].pid_philo);
		printf("philos[%d].last_eat = %ld\n", i, table.philos[i].last_eat);
		printf("philos[%d].nb_eat = %d\n", i, table.philos[i].nb_eat);
		printf("philos[%d].dead = %d\n", i, table.philos[i].dead);
		printf("philos[%d].eat_enough = %d\n", i, table.philos[i].eat_enough);
		i++;
	}
 */