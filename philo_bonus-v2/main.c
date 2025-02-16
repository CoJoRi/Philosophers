/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:05:48 by jrinaudo          #+#    #+#             */
/*   Updated: 2025/02/16 11:13:45 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/**
 * Creates child processes for each philosopher at the table
 * using fork(). Each philosopher runs in their own process
 * executing the to_be_or_not_to_be function.
 *
 * @param table Pointer to the table structure containing
 *              philosopher information and settings
 * 
 * @note The parent process continues creating philosophers while
 *       child processes execute philosopher logic
 */
void	create_philosophers(t_table *table)
{
	int		i;

	i = 0;
	while (i < table->nb_philo)
	{
		table->philos[i].pid = fork();
		if (table->philos[i].pid == 0)
		{
			if (pthread_create(&table->philos[i].medic, NULL, life_monitor, &table->philos[i]))
				pthread_cancel(table->philos[i].medic);
			if (pthread_create(&table->philos[i].eat_mon, NULL, meal_monitor, &table->philos[i]))
				pthread_cancel(table->philos[i].eat_mon);
			to_be_or_not_to_be(&table->philos[i]);
			pthread_join(table->philos[i].medic, NULL);
			pthread_join(table->philos[i].eat_mon, NULL);	
			exit(0);		
		}
		i++;
	}
}

/**
 * @brief Monitors the status of philosopher processes
 *
 * This function waits for all philosopher processes to finish and checks their
 *  exit status.
 * It tracks two conditions:
 * 1. If all philosophers have finished eating their required meals
 * 2. If any philosopher has died
 *
 * @param table Pointer to the table structure containing simulation parameters
 *
 * @return 0 if all philosophers finished eating successfully
 *         1 if a philosopher died during the simulation
 *
 * waitpid(-1, &status, 0);	Attend qu'un enfant se termine et stocke son état
 * dans status.
 * WIFEXITED(status)	Vérifie si l'enfant s'est terminé avec exit().
 * WEXITSTATUS(status)	Récupère la valeur passée à exit()
 */
static int	monitor_philosophers(t_table *table)
{
	int	status;
	int	i;

	i = 0;
	waitpid(-1, &status, 0);
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 1)
		{
			printf("\n\t"BG_WHITE BLUE"Someone died. RIP him"RESET"\n");
			table->finish = 1;
			while (i < table->nb_philo)
			{
				sem_post(table->finish_eat);
				i++;
			}
			return (1);
		}
		else if (WEXITSTATUS(status) == 0)
			return (0);
	}
	return (0);
}

/**
 * Performs cleanup of semaphore resources used by the dining philosophers
 *  program
 * Closes and unlinks all semaphores used for:
 * - Forks/baguettes
 * - Message printing
 * - Death status tracking
 * 
 * @param table Pointer to the table structure containing semaphore handles
 * @return void
 */
void	cleanup_resources(t_table *table)
{
	sem_close(table->forks);
	sem_close(table->message);
	sem_close(table->finish_eat);
	sem_close(table->sem_exit);
	sem_close(table->status);
	sem_unlink("/baguette");
	sem_unlink("/message_semaphore");
	sem_unlink("/finish eating");
	sem_unlink("/exit");
	sem_unlink("/status");
}

static void	terminate_philosophers(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philo)
	{
		kill(table->philos[i].pid, SIGKILL);
		i++;
	}
}

void *wait_all_finished(void *arg)
{
	t_table *table = (t_table *)arg;
	int i = 0;

	while (i < table->nb_philo)
	{
		sem_wait(table->finish_eat);
		i++;
	}
	if (table->finish == 1)
		return NULL;
	sem_wait(table->message);
	printf("\n\t"BG_WHITE BLUE " all have eaten " RESET "\n");
	sem_post(table->message);
	i = 0;
	while (i < table->nb_philo)
	{
		sem_post(table->sem_exit);
		i++;
	}
	return NULL;
}

/**
 * @brief Main function for the Dining Philosophers problem simulation
 * 
 * Initializes and manages the dining philosophers simulation.
 * The program requires at least 4 command line arguments to run properly.
 * It sets up the table, creates philosophers processes, monitors their
 *  activities, and ensures proper cleanup of resources.
 * 
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 * 
 * @return int Returns 0 on successful execution, 1 on error
 *             Error cases:
 *             - Insufficient command line arguments
 *             - Table initialization failure
 */
int	main(int argc, char **argv)
{
	t_table	table;
	pthread_t finish_thread;

	if (argc < 5)
		return (write(2, "Error args -> need 4 minimum\n", 29), 1);
	if (init_table(&table, argv + 1, argc - 1))
		return (1);
	printf(GREEN"\t_______________________\n"RESET);
	printf(GREEN"\t|     Bon Appetit     |\n"RESET);
	printf(GREEN"\t|_____________________|\n"RESET);
	create_philosophers(&table);
	pthread_create(&finish_thread, NULL, (void *)wait_all_finished, &table);
	if (monitor_philosophers(&table))
		terminate_philosophers(&table);
	pthread_join(finish_thread, NULL);
	printf(GREEN"\t_______________________\n"RESET);
	printf(GREEN"\t|  the dinner is over |\n"RESET);
	printf(GREEN"\t|_____________________|\n"RESET);
	cleanup_resources(&table);
	return (0);
}
