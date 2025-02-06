/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 14:10:06 by zeph              #+#    #+#             */
/*   Updated: 2025/02/06 19:04:42 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Initializes the philosophers with their respective attributes.
 *
 * This function initializes the philosophers with their respective attributes
 * such as their id, last time they ate, number of times they ate, time to die,
 * time to eat, time to sleep, maximum number of times they can eat, and the
 *  forks
 * they are holding.
 *
 * @param table A pointer to the table structure containing the philosophers
 *  and forks.
 * @param argv An array of integers containing the input arguments.
 * @param argc The number of input arguments.
 * @return Returns 0 on success, otherwise returns 1.
 */
static int	init_philos(t_table *table, int *argv, int argc)
{
	int		i;

	i = -1;
	while (++i < table->nb_philo)
	{
		table->philos[i].id = i;
		table->philos[i].last_eat = 0;
		table->philos[i].nb_eat = 0;
		table->philos[i].time_die = argv[1];
		table->philos[i].time_eat = argv[2];
		table->philos[i].time_sleep = argv[3];
		table->philos[i].eat_max = -1;
		if (argc == 5)
			table->philos[i].eat_max = argv[4];
		table->philos[i].dead = 0;
		table->philos[i].took_l_fork = 0;
		table->philos[i].took_r_fork = 0;
		table->philos[i].table = table;
		table->philos[i].l_fork = &table->forks[i];
		if (table->nb_philo == 1)
			table->philos[i].r_fork = NULL;
		else
			table->philos[i].r_fork = &table->forks[(i + 1) % table->nb_philo];
	}
	return (0);
}

/**
 * @brief Initializes the mutexes for the forks, status, message, and time.
 *
 * This function initializes the mutexes for the forks, status, message, and 
 * time.
 * If an error occurs during the initialization of a mutex, all previously
 * initialized
 * mutexes are destroyed.
 *
 * @param table A pointer to the table structure containing the philosophers
 * and forks.
 * @return Returns 0 on success, otherwise returns 1.
 */
static int	init_mutexes(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philo)
	{
		if (pthread_mutex_init(&table->forks[i], NULL) != 0
			|| pthread_mutex_init(&table->philos[i].mut_last_meal, NULL) != 0)
			return (1);
		i++;
	}
	if (pthread_mutex_init(&table->status, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&table->message, NULL) != 0)
		return (1);
	return (0);
}

/**
 * @brief Initializes the threads for each philosopher.
 * 
 * This function creates a thread for each philosopher and assigns the turn
 *  function
 * as the routine for each thread. If an error occurs during the creation of
 *  a thread,
 * all previously created threads are cancelled.
 *
 * @param table A pointer to the table structure containing the philosophers
 * and forks.
 * @return Returns 0 on success, otherwise returns 1.
 */
static int	init_threads(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philo)
	{
		if (pthread_create(&table->threads[i], NULL, turn, &table->philos[i]))
		{
			while (--i >= 0)
				pthread_cancel(table->threads[i]);
			return (1);
		}
		i++;
	}
	if (pthread_create(&table->medic, NULL, watch_table, table))
	{
		while (--i >= 0)
			pthread_cancel(table->threads[i]);
		return (1);
	}
	return (0);
}

/**
 * @brief Checks the validity of command-line arguments.
 *
 * This function validates the command-line arguments passed to the program.
 * It ensures that each argument is a valid integer and converts it to an
 *  integer.
 * Additionally, it checks for specific constraints such as non-negative values
 * and a maximum limit on the number of philosophers.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line argument strings.
 * @param argv_ctrled The array to store the converted integer values of the
 *  arguments.
 * @return Returns 0 if all arguments are valid, otherwise returns 1 and prints
 *  an error message.
 */
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
			return (write(2, "Error Negative or null value\n", 29), 1);
		i++;
	}
	if (argv_ctrled[0] > 200)
		return (write(2, "Error Too many philosophers (max: 200)\n", 39), 1);
	return (0);
}

/**
 * @brief Initializes the table structure for the philosophers simulation.
 *
 * This function sets up the table structure by initializing the number of
 *  philosophers,
 * mutexes, philosopher structures, and threads. It also validates the input
 *  arguments.
 *
 * @param table Pointer to the table structure to be initialized.
 * @param argv Array of input arguments.
 * @param argc Number of input arguments.
 * @return int Returns 0 on success, 1 on failure.
 */
int	init_table(t_table *table, char**argv, int argc)
{
	int	argvctrled[5];

	if (check_args(argc - 1, argv + 1, argvctrled))
		return (1);
	table->nb_philo = argvctrled[0];
	table->finish = 0;
	table->eat_max_ok = 0;
	if (init_mutexes(table))
		return (free_mutex(table), 1);
	if (init_philos(table, argvctrled, argc - 1))
		return (free_mutex(table), 1);
	init_times(table);
	if (init_threads(table))
		return (free_mutex(table), 1);
	return (0);
}
