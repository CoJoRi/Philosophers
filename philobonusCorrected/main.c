/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:05:48 by jrinaudo          #+#    #+#             */
/*   Updated: 2025/02/08 17:03:54 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 waitpid(-1, &status, 0);	Attend qu'un enfant se termine et stocke son état
 dans status.
WIFEXITED(status)	Vérifie si l'enfant s'est terminé avec exit().
WEXITSTATUS(status)	Récupère la valeur passée à exit()
 */
void	create_philosophers(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philo)
	{
		table->philos[i].pid_philo = fork();
		if (table->philos[i].pid_philo == 0)
			to_be_or_not_to_be(&table->philos[i]);
		i++;
	}
}

int	monitor_philosophers(t_table *table)
{
	int	i;
	int	status;
	int	all_eat;

	i = 0;
	all_eat = 0;
	while (i < table->nb_philo)
	{
		waitpid(-1, &status, 0);
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) == 0)
				all_eat++;
			else if (WEXITSTATUS(status) == 1)
				return (printf("\n\t"BG_WHITE BLUE
						" Someone died....RIP him " RESET "\n"), 1);
		}
		if (all_eat == table->nb_philo)
			return (printf("\n\t"BG_WHITE BLUE
					" all have eat " RESET "\n"), 0);
		i++;
	}
	return (0);
}

void	cleanup_resources(t_table *table)
{
	sem_close(table->forks);
	sem_unlink("/baguette");
	sem_close(table->message);
	sem_unlink("/message_semaphore");
	sem_close(table->dead);
	sem_unlink("/dead");
}

int	main(int argc, char **argv)
{
	t_table	table;

	if (argc < 5)
		return (write(2, "Error args -> need 4 minimum\n", 29), 1);
	if (init_table(&table, argv + 1, argc - 1))
		return (1);
	printf(GREEN"\n\t_______________________\n"RESET);
	printf(GREEN"\t|                     |\n"RESET);
	printf(GREEN"\t|     Bon Appetit     |\n"RESET);
	printf(GREEN"\t|_____________________|\n"RESET);
	create_philosophers(&table);
	if (monitor_philosophers(&table))
		kill(0, SIGINT);
	printf(GREEN"\n\t_______________________\n"RESET);
	printf(GREEN"\t|                     |\n"RESET);
	printf(GREEN"\t|  the dinner is over |\n"RESET);
	printf(GREEN"\t|_____________________|\n"RESET);
	cleanup_resources(&table);
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