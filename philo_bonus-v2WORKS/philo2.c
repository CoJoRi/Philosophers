/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:12:49 by jrinaudo          #+#    #+#             */
/*   Updated: 2025/02/16 08:50:23 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/**
 * Release all forks currently held by a philosopher
 * The function posts to the semaphore for each fork the philosopher has,
 * effectively returning them to the table
 * 
 * @param philo Pointer to philosopher structure containing fork count and
 *  table info
 */
void	release_fork(t_philo *philo)
{
	while (philo->forks_in_hand)
	{
		sem_post(philo->table->forks);
		philo->forks_in_hand--;
	}
}
