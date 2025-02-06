/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 14:01:34 by zeph              #+#    #+#             */
/*   Updated: 2025/02/06 11:44:56 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <fcntl.h>
# include <semaphore.h>

# define INT_MAX	2147483647
# define INT_MIN	-2147483648
# define BG_WHITE	"\033[47m"
# define BLUE		"\033[34m"
# define BLUE_LIGHT "\033[1;34m"
# define GREEN		"\033[1;32m"
# define RESET		"\033[0m"
# define RED		"\033[1;31m"
# define YELLOW 	"\033[1;33m"
# define PURPLE		"\033[1;35m"
# define GREY		"\033[1;30m"
# define GREY2		"\033[0;30m"
# define ORANGE		"\033[38;5;214m"

typedef struct s_philo	t_philo;
typedef struct s_table	t_table;

struct s_philo
{
	int				id;
	long			time_start;
	long			last_eat;
	int				nb_eat;
	int				eat_max;
	int				dead;
	int				took_l_fork;
	int				took_r_fork;
};
struct s_table
{
	long			time_start;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				nb_philo;
	int				eat_max_ok;
	int				finish;
	pthread_t		medic;
	t_philo			philos[200];
};

/*what is insite timeval:
struct timeval {
    time_t      tv_sec;   // Seconds passed since the first janvier 1970
	(epoch time)
    suseconds_t tv_usec;  // Microsecondes (1 millionth of a second) more
};
tv_sec * 1000 + tv_usec / 1000 → Gives time in milliseconds.
*/
/*----------------------------------main.c-----------------------------------*/
int		main(int argc, char **argv);
long	init_time_start(void);

/*----------------------------------init.c-----------------------------------*/
int		init_table(t_table *table, char **argv, int argc);

/*----------------------------------init2.c----------------------------------*/
void	init_times(t_table *table);
int		check_input(char *argv);

/*----------------------------------utils1.c---------------------------------*/
int		ft_atoi(const char *str);
long	get_time(t_philo *philo);

/*----------------------------------philo.c----------------------------------*/
void	*turn(void	*args);
void	message(t_philo *philo, char *msg);
void	my_sleep(int duration);

/*----------------------------------philo2.c---------------------------------*/
void	release_fork(t_philo *philo);
int		check_finish(t_philo *philo);
int		is_alive(t_philo *philo);
int		eat_philo(t_philo *philo);
void	sleep_philo(t_philo *philo);

/*----------------------------------medic.c---------------------------------*/
void	*watch_table(void *arg);

/*----------------------------------free.c-----------------------------------*/
void	free_mutex(t_table *table);

#endif