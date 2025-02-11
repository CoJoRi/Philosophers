/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:54:29 by jrinaudo          #+#    #+#             */
/*   Updated: 2025/02/11 09:54:16 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Pauses the execution of the program for a specified duration.
 *
 * This function uses a busy-wait loop to pause the execution of the program
 * for the specified duration in milliseconds. It repeatedly checks the elapsed
 * time and calls `usleep` to yield the processor for short intervals.
 *
 * @param duration The duration to sleep in milliseconds.
 */
void	my_sleep(int duration)
{
	long	time;

	time = init_time_start();
	while (init_time_start() - time < duration)
		usleep(50);
}

/**
 * @brief Function for a philosopher to take the left fork.
 *
 * This function locks the mutex associated with the left fork of the
 * philosopher,
 * sets the philosopher's took_l_fork flag to 1, and prints a message indicating
 * that the philosopher has taken a fork.
 *
 * @param philo Pointer to the philosopher structure.
 * @return Always returns 0.
 */
static int	take_left_fork(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	philo->took_l_fork = 1;
	message(philo, BLUE"has taken a fork"RESET);
	return (0);
}

/**
 * @brief Attempts to take the right fork for the philosopher.
 *
 * This function checks if the right fork is available. If it is not available,
 * the philosopher will sleep for a duration longer than the time it takes to
 *  die,
 * indicating that the philosopher cannot proceed without the fork. If the right
 * fork is available, it locks the fork using a mutex, marks that the
 *  philosopher
 * has taken the right fork, and logs a message indicating that the philosopher
 * has taken a fork.
 *
 * @param philo A pointer to the philosopher structure.
 * @return Returns 1 if the right fork is not available, otherwise returns 0.
 */
static int	take_right_fork(t_philo *philo)
{
	if (!philo->r_fork)
	{
		my_sleep(philo->time_die + 1);
		return (1);
	}
	pthread_mutex_lock(philo->r_fork);
	philo->took_r_fork = 1;
	message(philo, BLUE_LIGHT"has taken a fork"RESET);
	return (0);
}

/**
 * @brief Prints a message for a philosopher in a thread-safe manner.
 *
 * This function locks the necessary mutexes to ensure that the message is
 *  printed
 * without interference from other threads. If the simulation has finished, it
 *  will unlock the mutexes and return immediately. Otherwise, it prints the
 *  message with the current time and philosopher's ID, then unlocks the
 *  mutexes.
 * @param philo Pointer to the philosopher structure.
 * @param msg The message to be printed.
 */
void	message(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->table->message);
	pthread_mutex_lock(&philo->table->status);
	if (philo->table->finish)
	{
		pthread_mutex_unlock(&philo->table->status);
		pthread_mutex_unlock(&philo->table->message);
		return ;
	}
	pthread_mutex_unlock(&philo->table->status);
	printf(""BG_WHITE GREY"%ld "BLUE"%d "RESET"%s\n",
		get_time(philo), philo->id, msg);
	pthread_mutex_unlock(&philo->table->message);
}

/**
 * @brief Function executed by each philosopher thread.
 *
 * This function represents the life cycle of a philosopher. Each philosopher
 * will attempt to take forks, eat, sleep, and think in a loop until the
 * simulation ends.
 *
 * @param arg Pointer to the philosopher structure (t_philo).
 * @return Always returns NULL.
 */
void	*turn(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (check_finish(philo) == 0)
	{
		if (philo->id % 2)
		{
			think_philo(philo);
			take_right_fork(philo);
			take_left_fork(philo);
		}
		else
		{
			take_left_fork(philo);
			take_right_fork(philo);
		}
		if (philo->took_l_fork && philo->took_r_fork)
		{
			eat_philo(philo);
			sleep_philo(philo);
			think_philo(philo);
		}
	}
	return (NULL);
}

/* GDB DEBUG
philo->table->threads[0]
philo->table->threads[1]
philo->table->threads[2]
philo->table->threads[3]
philo->dead
philo->table->finish
philo->table->forks
philo->table->nb_philo
philo->table->time_die
philo->table->time_eat
philo->table->time_sleep
philo->table->eat_max
philo->table->time_start
philo->table->tv
philo->table->philos
philo->table->message
philo->table->threads
philo->dead;
philo->last_eat;
philo->nb_eat
 */