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
		}
		i++;
	}
}

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
	sem_post(table.sem_exit);
	pthread_create(&finish_thread, NULL, (void *)wait_all_finished, &table);
	sem_post(table.sem_exit);
	if (monitor_philosophers(&table))
		terminate_philosophers(&table);
	pthread_join(finish_thread, NULL);
	printf(GREEN"\t_______________________\n"RESET);
	printf(GREEN"\t|  the dinner is over |\n"RESET);
	printf(GREEN"\t|_____________________|\n"RESET);
	cleanup_resources(&table);
	return (0);
}

void    close_sem(t_philo *philo)
{
	sem_close(philo->table->status);
	sem_close(philo->table->forks);
	sem_close(philo->table->message);
	sem_close(philo->table->sem_exit);
	sem_close(philo->table->finish_eat);
}

void	*life_monitor(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->table->status);
		if (philo->table->finish == 1)
		{
			sem_post(philo->table->status);
			write (1, "finish\n", 7);
			return (NULL);
		}
		if ((my_clock() - philo->last_eat) > philo->table->time_die)
		{
			sem_wait(philo->table->message);
			printf("%ld %d "RED"died\n"RESET, my_clock() - philo->table->time_start, philo->id);
			close_sem(philo);
			exit(1);
			return (NULL);
		}
		sem_post(philo->table->status);
	}
	return (NULL);
}

void *meal_monitor(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	while (1)
	{
		my_sleep(1);
		sem_wait(philo->table->status);
		if (philo->nb_eat == philo->table->eat_limit)
		{
			sem_post(philo->table->finish_eat);
			sem_post(philo->table->status);
			sem_wait(philo->table->sem_exit);
			sem_wait(philo->table->status);
			philo->table->finish = 1;
			sem_post(philo->table->status);
			close_sem(philo);
			exit(0);
			return (NULL);
		}
		sem_post(philo->table->status);		
	}
	return (NULL);
}

int	take_fork(t_philo *philo)
{
	if (philo->table->nb_philo == 1 && philo->forks_in_hand == 1)
	{
		my_sleep(philo->table->time_die + 1);
		return (1);
	}
	sem_wait(philo->table->forks);
	philo->forks_in_hand++;
	if (philo->forks_in_hand == 1)
		message(philo, BLUE_LIGHT"has taken a fork"RESET);
	else
		message(philo, BLUE"has taken a fork"RESET);
	return (0);
}

void	message(t_philo *philo, char *msg)
{
	sem_wait(philo->table->message);
	printf(""BG_WHITE GREY"%ld "BLUE"%d "RESET"%s\n",
		get_time_elapsed(philo->table->time_start), philo->id, msg);
	sem_post(philo->table->message);
}

int	eat_philo(t_philo *philo)
{
	message(philo, GREEN"is eating"RESET);
	sem_wait(philo->table->status);
	philo->last_eat = my_clock();
	sem_post(philo->table->status);
	my_sleep(philo->table->time_eat);
	sem_wait(philo->table->status);
	philo->nb_eat++;
	sem_post(philo->table->status);
	release_fork(philo);
	return (0);
}

void	sleep_philo(t_philo *philo)
{
	message(philo, YELLOW"is sleeping"RESET);
	my_sleep(philo->table->time_sleep);
}

void	*to_be_or_not_to_be(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		if (philo->id % 2 == 1)
			message(philo, ORANGE"is thinking"RESET);
		take_fork(philo);
		take_fork(philo);
		if (philo->forks_in_hand == 2)
		{
			eat_philo(philo);
			sleep_philo(philo);
			if (philo->id % 2 == 0)
				message(philo, ORANGE"is thinking"RESET);
		}
	}
	return (NULL);
}