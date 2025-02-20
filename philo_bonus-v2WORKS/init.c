/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 14:10:06 by zeph              #+#    #+#             */
/*   Updated: 2025/02/18 08:47:15 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/**
 * Checks if input string contains only digits
 * 
 * @param argv String to check
 *
 * @return 0 if string contains only digits, 1 otherwise
 */
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
		table->philos[i].id = i + 1;
		table->philos[i].last_eat = my_clock();
		table->philos[i].next_eat = table->philos[i].last_eat + table->time_die;
		table->philos[i].nb_eat = 0;
		table->philos[i].finish = 0;
		table->philos[i].forks_in_hand = 0;
		table->philos[i].eat_enough = 0;
		table->philos[i].table = table;
	}
	return (0);
}

/**
 * Validates command line arguments for the philosophers program
 * 
 * @param argc Number of arguments to validate
 * @param argv Array of string arguments to validate
 * @param argv_ctrled Array to store the converted integer values
 * 
 * @return 0 if validation succeeds, 1 if any validation fails with error
 *  message
 *         Fails if:
 *         - Any argument is not a valid number
 *         - Any argument is negative
 *         - Number of philosophers is 0
 *         - Number of philosophers exceeds 200
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
			return (write(2, "Error Negative value\n", 21), 1);
		i++;
	}
	if (argv_ctrled[0] == 0)
		return (write(2, "Error Not enough philosophers (min: 1)\n", 40), 1);
	if (argv_ctrled[0] > 200)
		return (write(2, "Error Too many philosophers (max: 200)\n", 39), 1);
	return (0);
}

/**
 * Initialize semaphores for the philosopher program
 * Creates and opens three named semaphores:
 * - message_semaphore: Controls access to message printing (initialized to 1)
 * - baguette: Represents available forks/chopsticks (initialized to number of
 *  philosophers)
 * - exit: Controls access to sem_finish status checking (initialized to 1)
 * 
 * First unlinks any existing semaphores with same names to ensure clean state
 * Then opens semaphores with specified initial values
 *
 * @param table Pointer to the table structure containing program data
 * @return 0 on success, 1 if any semaphore creation fails
 */
static int	init_semaphors(t_table *table)
{
	sem_unlink("/message_semaphore");
	sem_unlink("/baguette");
	sem_unlink("/exit");
	sem_unlink("/finish eating");
	sem_unlink("/status");
	table->message = sem_open("/message_semaphore", O_CREAT, 0644, 1);
	if (table->message == SEM_FAILED)
		return (write(2, "Error: sem_open failed", 22), 1);
	table->forks = sem_open("/baguette", O_CREAT, 0644, table->nb_philo);
	if (table->forks == SEM_FAILED)
		return (write(2, "Error: sem_open failed", 22), 1);
	table->sem_exit = sem_open("/exit", O_CREAT, 0644, 0);
	if (table->sem_exit == SEM_FAILED)
		return (write(2, "Error: sem_open failed", 22), 1);
	table->finish_eat = sem_open("/finish eating", O_CREAT, 0644, 0);
	if (table->finish_eat == SEM_FAILED)
		return (write(2, "Error: sem_open failed", 22), 1);
	table->status = sem_open("/status", O_CREAT, 0644, 1);
	if (table->status == SEM_FAILED)
		return (write(2, "Error: sem_open failed", 22), 1);
	return (0);
}

/**
 * @brief Initializes the dining philosophers table structure
 *
 * This function sets up the table structure with the parsed command line
 *  arguments, initializes semaphores and philosophers data structures.
 * It performs the following:
 * - Validates command line arguments
 * - Sets up table parameters (number of philosophers, time limits)
 * - Initializes semaphores
 * - Creates philosopher processes
 * - Records start time
 *
 * @param table Pointer to the table structure to initialize
 * @param argv Array of command line argument strings
 * @param argc Number of command line arguments
 *
 * @return 0 on success, 1 on error
 */
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
	table->time_start = my_clock();
	return (0);
}
