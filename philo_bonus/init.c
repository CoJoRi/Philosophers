/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 14:10:06 by zeph              #+#    #+#             */
/*   Updated: 2025/02/08 17:02:45 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_input(char *argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		if (argv[i] < '0' || argv[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

static int	init_philos(t_table *table)
{
	int		i;

	i = -1;
	while (++i < table->nb_philo)
	{
		table->philos[i].id = i;
		table->philos[i].pid_philo = 0;
		table->philos[i].last_eat = clock();
		table->philos[i].nb_eat = 0;
		table->philos[i].dead = 0;
		table->philos[i].forks_in_hand = 0;
		table->philos[i].eat_enough = 0;
		table->philos[i].table = table;
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
			write(2, "Error invalid arg\n", 18);
			return (1);
		}
		argv_ctrled[i] = ft_atoi(argv[i]);
		if (argv_ctrled[i] < 0)
			return (write(2, "Error Negative value\n", 21), 1);
		i++;
	}
	if (argv_ctrled[0] == 0)
		return (write(2, "Error Not enough philosophers (min: 1)\n", 40), 1);
	if (argv_ctrled[0] > 200)
		return (write(2, "Error Too many philosophers (max: 200)\n", 39), 1);
	return (0);
}

static int	init_semaphors(t_table *table)
{
	sem_unlink("/message_semaphore");
	sem_unlink("/baguette");
	sem_unlink("/dead_status");
	table->message = sem_open("/message_semaphore", O_CREAT, 0644, 1);
	if (table->message == SEM_FAILED)
	{
		write(2, "Error: sem_open failed", 22);
		return (1);
	}
	table->forks = sem_open("/baguette", O_CREAT, 0644, table->nb_philo);
	if (table->forks == SEM_FAILED)
	{
		write(2, "Error: sem_open failed", 22);
		return (1);
	}
	table->dead = sem_open("/dead_status", O_CREAT, 0644, 1);
	if (table->dead == SEM_FAILED)
	{
		write(2, "Error: sem_open failed", 22);
		return (1);
	}
	return (0);
}

int	init_table(t_table *table, char**argv, int argc)
{
	int	argvctrled[5];

	if (check_args(argc, argv, argvctrled))
		return (1);
	table->nb_philo = argvctrled[0];
	table->time_die = argvctrled[1];
	table->time_eat = argvctrled[2];
	table->time_sleep = argvctrled[3];
	if (argc == 5)
		table->eat_limit = argvctrled[4];
	else
		table->eat_limit = -1;
	table->finish = 0;
	table->eat_max_ok = 0;
	if (init_semaphors(table))
		return (1);
	if (init_philos(table))
		return (1);
	table->time_start = clock();
	return (0);
}
