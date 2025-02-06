/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 14:10:06 by zeph              #+#    #+#             */
/*   Updated: 2025/02/06 11:55:29 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_semaphore()
{

}


static int	init_philos(t_table *table, int *argv, int argc)
{
	int		i;

	i = -1;
	while (++i < table->nb_philo)
	{
		table->philos[i].id = i;
		table->philos[i].last_eat = 0;
		table->philos[i].nb_eat = 0;
		if (argc == 5)
			table->philos[i].eat_max = argv[4];
		else
		table->philos[i].eat_max = -1;
		table->philos[i].dead = 0;
		table->philos[i].took_l_fork = 0;
		table->philos[i].took_r_fork = 0;
	}
	return (0);
}

static int	check_args(int argc, char **argv, int *argv_ctrled)
{
	int	i;

	i = 0;
	while (i < argc)
	{
		if (check_input(argv[i]))
		{
			printf("Error invalid arg:%d \n", i + 1);
			return (1);
		}
		argv_ctrled[i] = ft_atoi(argv[i]);
		if (argv_ctrled[i] <= 0)
			return (printf("Error Negative values\n"), 1);
		i++;
	}
	if (argv_ctrled[0] > 200)
		return (printf("Error Too many philosophers (max: 200)\n"), 1);
	return (0);
}

int	init_table(t_table *table, char**argv, int argc)
{
	int	argvctrled[5];
	sem_t *semaphore;

	sem_unlink("/semaphore_name");
	semaphore = sem_open("/semaphore_name", O_CREAT, 0644, 1);

	if (check_args(argc - 1, argv + 1, argvctrled))
		return (1);
	table->time_die = argv[1];
	table->time_eat = argv[2];
	table->time_sleep = argv[3];
	table->nb_philo = argvctrled[0];
	table->finish = 0;
	table->eat_max_ok = 0;
	if (init_philos(table, argvctrled, argc - 1))
		return (free_mutex(table), 1);
	init_times(table);
	return (0);
}
