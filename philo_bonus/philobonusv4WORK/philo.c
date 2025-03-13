/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:54:29 by jrinaudo          #+#    #+#             */
/*   Updated: 2025/02/14 21:12:50 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Attempt to take a fork for a philosopher
 *
 * This function manages the fork-taking process for a philosopher.
 * For a single philosopher case, it simulates waiting until death.
 * Otherwise, it waits on the semaphore for available forks and updates
 * the philosopher's fork count.
 *
 * @param philo Pointer to philosopher structure containing thread data
 *
 * @return 1 if single philosopher case (leading to death),
 *         0 on successful fork acquisition,
 *         exits with 1 if philosopher dies while waiting
 * 
 * @note Different color messages are displayed depending on whether it's
 *       the first or second fork being taken
 */
int	take_fork(t_philo *philo)
{
	if (philo->table->nb_philo == 1 && philo->forks_in_hand == 1)
	{
		my_sleep(philo->table->time_die + 1, philo);
		return (1);
	}
	sem_wait(philo->table->forks);
	if (!is_alive(philo))
	{
		exit (1);
	}
	philo->forks_in_hand++;
	if (philo->forks_in_hand == 1)
		message(philo, BLUE_LIGHT"has taken a fork"RESET);
	else
		message(philo, BLUE"has taken a fork"RESET);
	return (0);
}

/**
 * @brief Prints a formatted message with timestamp and philosopher ID
 * 
 * This function synchronizes message printing using a semaphore to avoid
 * output interleaving between philosophers. The message is printed with
 * the following format: "[timestamp] [philosopher_id] [message]"
 *
 * @param philo Pointer to philosopher struct containing ID and table info
 * @param msg String message to be printed
 * 
 * @note The function uses color formatting macros (BG_WHITE, GREY, BLUE, RESET)
 *       to enhance output readability
 */
void	message(t_philo *philo, char *msg)
{
	sem_wait(philo->table->message);
	printf(""BG_WHITE GREY"%ld "BLUE"%d "RESET"%s\n",
		get_time_elapsed(philo->table->time_start), philo->id, msg);
	sem_post(philo->table->message);
}

/**
 * Makes a philosopher eat
 * The function handles the eating process of a philosopher
 * - Checks if philosopher is alive
 * - Displays eating message
 * - Updates last eating time
 * - Sleeps for eating duration
 * - Increments eating counter
 * - Releases forks after eating
 * 
 * @param philo Pointer to philosopher structure
 * @return 0 on success, exits with 0 if eat limit reached, exits with 1 if dead
 */
int	eat_philo(t_philo *philo)
{
	int		wait_eat;

	wait_eat = 100000;
	if (!is_alive(philo))
		exit (1);
	message(philo, GREEN"is eating"RESET);
	philo->last_eat = clock();
	my_sleep(philo->table->time_eat, philo);
	philo->nb_eat++;
	if (philo->nb_eat == philo->table->eat_limit)
	{
		release_fork(philo);
		sem_wait(philo->table->message);
		printf("philo %d a mange assez", philo->id);
		sem_post(philo->table->message);
		while (wait_eat)
			wait_eat--;
		exit (0);
	}
	release_fork(philo);
	return (0);
}

/**
 * @brief Handles philosopher's sleeping state
 * 
 * Displays a message that the philosopher is sleeping and makes the
 * philosopher sleep for the duration specified in the table configuration.
 * 
 * @param philo Pointer to the philosopher structure containing the
 *  philosopher's data
 * 
 * @return void
 */
void	sleep_philo(t_philo *philo)
{
	message(philo, YELLOW"is sleeping"RESET);
	my_sleep(philo->table->time_sleep, philo);
}

/*
** Thread routine function simulating a philosopher's life cycle
** This function represents the main loop of a philosopher's actions:
** thinking, taking forks, eating, and sleeping
**
** @param arg: void pointer to philosopher's data structure (t_philo)
** @return: void pointer (NULL), but function exits process on completion
**
** The function implements the following logic:
** 1. Odd-numbered philosophers start by thinking to avoid deadlock
** 2. Attempts to take forks (one at a time)
** 3. If philosopher has both forks, proceeds to eat, sleep, and think
** 4. Exits with status 1 if philosopher dies, 0 on normal termination
*/
void	*to_be_or_not_to_be(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		if (!is_alive(philo))
			exit (1);
		if (philo->id % 2 == 1)
			message(philo, ORANGE"is thinking"RESET);
		take_fork(philo);
		if (!is_alive(philo))
			exit (1);
		take_fork(philo);
		if (philo->forks_in_hand == 2)
		{
			eat_philo(philo);
			sleep_philo(philo);
			message(philo, ORANGE"is thinking"RESET);
		}
	}
	exit (0);
}
