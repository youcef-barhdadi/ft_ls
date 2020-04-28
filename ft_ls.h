# ifndef _FT_LS_
#define _FT_LS_
# include <stdio.h>
# include <fcntl.h>
# include <time.h>
# include <sys/stat.h>
# include <dirent.h>
# include "libft/libft.h"
# include <sys/types.h>
# include <pwd.h>
# include <grp.h>
# include <errno.h>

typedef struct s_file
{
	char *user_name;
	char *group_name;
	char permssion[11];
	off_t size;
	time_t time;
	nlink_t links;
	int 	islink;
	char *referto;

}t_file;


typedef struct  s_config
{
	int hidden;
	int rev;
	int rec;
	int llong;
}	t_config;


void recurseve(char *start, t_config *con);
void handle_display(char *dir,t_list *list, t_config *con, int file);
void sort_list(t_list **list, int (*cmp)(void *a, void *b));



#endif 
