/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 14:01:34 by zeph              #+#    #+#             */
/*   Updated: 2025/02/11 19:59:23 by jrinaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <semaphore.h>
# include <pthread.h>
# include <signal.h>



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
# define CYAN		"\033[1;36m"

typedef struct s_philo	t_philo;
typedef struct s_table	t_table;

struct s_philo
{
	int				id;
	pid_t			pid;
	long			last_eat;
	int				nb_eat;
	int				forks_in_hand;
	int				eat_enough;
	int				finish;
	pthread_t		medic;
	pthread_t		eat_mon;
	t_table			*table;
};

struct s_table
{
	int				nb_philo;
	long			time_start;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				eat_limit;
	int				eat_max_ok;
	int				finish;
	sem_t			*finish_eat;
	sem_t			*status;
	sem_t			*forks;
	sem_t			*message;
	sem_t			*sem_exit;
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

/*----------------------------------init.c-----------------------------------*/
int		init_table(t_table *table, char **argv, int argc);

/*----------------------------------time.c----------------------------------*/
long	get_time_elapsed(long time_start);
long	my_clock(void);
void	my_sleep(int duration);

/*----------------------------------utils1.c---------------------------------*/
int		ft_atoi(const char *str);

/*----------------------------------philo.c----------------------------------*/
int		take_fork(t_philo *philo);
void	message(t_philo *philo, char *msg);
int		eat_philo(t_philo *philo);
void	sleep_philo(t_philo *philo);
void	*to_be_or_not_to_be(void	*args);

/*----------------------------------philo2.c---------------------------------*/
void	release_fork(t_philo *philo);

/*----------------------------------monitor.c---------------------------------*/
void	*life_monitor(void *arg);
void	*meal_monitor(void *arg);
void    close_sem(t_philo *philo);
int		is_alive(t_philo *philo);

#endif