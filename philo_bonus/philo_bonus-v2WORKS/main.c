/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:05:48 by jrinaudo          #+#    #+#             */
/*   Updated: 2025/02/18 08:56:09 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
			if (pthread_create(&table->philos[i].stop_philo, NULL, stop_philo, &table->philos[i]))
				pthread_cancel(table->philos[i].stop_philo);
			pthread_detach(table->philos[i].stop_philo);
			pthread_detach(table->philos[i].medic);
			to_be_or_not_to_be(&table->philos[i]);
		//	printf("sortie routine [%d]\n", table->philos[i].id);
			my_sleep(25);
			close_sem(&table->philos[i]);
		//	printf("sortie routine [%d]\n", table->philos[i].id);
			exit(0);
		}
		i++;
	}
}

static int	monitor_philosophers(t_table *table)
{
	int	status;
	int	i;

	i = 0;
	while(1)
	{
		waitpid(-1, &status, 0);
		if (WIFEXITED(status))
		{
			sem_wait(table->status);	
			table->finish = 1;
			sem_post(table->status);
			while (i < table->nb_philo)
			{
				sem_post(table->finish_eat);
				sem_post(table->sem_exit);
				i++;
			}
			my_sleep(5);
			return (1);
		}
	}
	return (0);
}

void	cleanup_resources(t_table *table)
{
	sem_close(table->forks);
	sem_close(table->message);
	sem_close(table->finish_eat);
	sem_close(table->sem_exit);
	sem_unlink("/baguette");
	sem_unlink("/message_semaphore");
	sem_unlink("/finish eating");
	sem_unlink("/exit");
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
	int j = 0;

	while (i < table->eat_limit)
	{
		printf("[%d]",sem_getvalue(table->finish_eat, &j));
		sem_wait(table->finish_eat);
		printf("{%d}",sem_getvalue(table->finish_eat, &j));
		i++;
	}
	sem_wait(table->status);
	if (table->finish == 1)
		return (sem_post(table->status), NULL);
	sem_post(table->status);
	//sem_wait(table->message);
	printf("\n\t"BG_WHITE BLUE " all have eaten " RESET "\n");
	i = 0;
	while (i < table->nb_philo)
	{
		sem_post(table->sem_exit);
		//printf("\n\t"BG_WHITE BLUE " philosopher %d is leaving the table " RESET "\n", i);
		i++;
	}
	return NULL;
}

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
	if (table.eat_limit != -1)
	{
		pthread_create(&finish_thread, NULL, (void *)wait_all_finished, &table);
		if (monitor_philosophers(&table))
		terminate_philosophers(&table);
	}
	create_philosophers(&table);
	pthread_join(finish_thread, NULL);
	printf(GREEN"\t_______________________\n"RESET);
	printf(GREEN"\t|  the dinner is over |\n"RESET);
	printf(GREEN"\t|_____________________|\n"RESET);
	cleanup_resources(&table);
	return (0);
}
