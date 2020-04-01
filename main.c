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

void recurseve(char *start)
{
	char *path_mode;
	DIR *dir = opendir(start);
	struct dirent *info;
	struct stat sb;
	t_list *list = NULL;
	int first = 0;
	//list = ft_lstnew(ft_strdup("."));
	printf("%s :\n", start);
	start = ft_strjoin(start, "/");
	while ((info = readdir(dir)))
	{
		if (stat(ft_strjoin(start, info->d_name), &sb) == -1)
		{
			printf("[%s]\n", info->d_name);
			perror("ft_ls");
			exit(2);
		}
		printf("%s\n", info->d_name);
		if (S_ISDIR(sb.st_mode))
		{
		//	printf("(%s)\n",info->d_name);
			if ( !ft_strchr(info->d_name, '.'))
			ft_lstadd_back(&list, ft_lstnew(ft_strjoin(start, ft_strdup(info->d_name))));
		}
	}
	while (list)
	{

//		printf("%s\n", list->content);
		recurseve(list->content);
		list = list->next;
	}
}



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

t_file *get_permssion(const struct stat st, t_file *file)
{
	mode_t perm = st.st_mode;
	file->permssion[1]= (perm & S_IRUSR) ? 'r' : '-';
	file->permssion[2]= (perm & S_IWUSR) ? 'w' : '-';
	file->permssion[3]= (perm & S_IXUSR) ? 'x' : '-';
	file->permssion[4]= (perm & S_IRGRP) ? 'r' : '-';
	file->permssion[5]= (perm & S_IWGRP) ? 'w' : '-';
	file->permssion[6]= (perm & S_IXGRP) ? 'x' : '-';
	file->permssion[7]= (perm & S_IROTH) ? 'r' : '-';
	file->permssion[8]= (perm & S_IWOTH) ? 'w' : '-';
	file->permssion[9]= (perm & S_IXOTH) ? 'x' : '-';
	file->permssion[10] = 0;
	return file;
}


t_file *get_usergroup_names(const struct stat st, t_file *file)
{
	struct passwd *pwd;
	struct group *grp;

	pwd = getpwuid(st.st_uid);
	grp = getgrgid(st.st_gid);
	if (pwd == NULL)
	{
		if (errno == 0)
		{
			/*  NOT found */
		}
		else
		{
			/* Error */
		}
	}

	file->user_name = ft_strdup(pwd->pw_name);
	file->group_name = ft_strdup(grp->gr_name);
	
}



t_file *get_file_type(const struct stat st,t_file *file)
{
	if (S_ISREG(st.st_mode))
		file->permssion[0] = '-'; 
	else if (S_ISDIR(st.st_mode))
		file->permssion[0] = 'd'; 
	else if (S_ISCHR(st.st_mode))
		file->permssion[0] = 'c'; 
	else if (S_ISBLK(st.st_mode))
		file->permssion[0] = 'b'; 
	else if (S_ISSOCK(st.st_mode))
		file->permssion[0] = 's'; 
	else if (S_ISFIFO(st.st_mode))
		file->permssion[0] = 'f'; 
	else if (S_ISLNK(st.st_mode))
		file->permssion[0] = 'l'; 
	else
		file->permssion[0] = ' '; 
	return file;
}


void print_long(struct stat st, char *filename)
{
	char buffer[1000];

	t_file *file = malloc(sizeof(t_file));
	file->size = st.st_size;
	file->time = st.st_ctime;
	file->links = st.st_nlink;
	if (S_ISLNK(st.st_mode))
	{
		ssize_t size = readlink(filename, buffer, 1000);
		buffer[size] = 0;
		file->referto = ft_strdup(buffer);
		file->islink  = 1;
	}
	get_permssion(st, file);
	get_file_type(st, file);
	get_usergroup_names(st, file);
	char *time = ft_substr(ctime(&file->time),4,12);
	printf("%s %lld %s %s %lld %s %s\n",file->permssion,file->links ,file->user_name, file->group_name, file->size,time, filename);
}


typedef struct  s_config
{
	int hidden;
	int rev;
	int rec;
	int llong;
}	t_config;

void readoptions(char *option, t_config *con)
{
	int i = 1;

	while (option[i])
	{
		if (option[i] == 'R')
			con->rec = 1;
		else if (option[i] ==  'a')
			con->hidden  = 1;
		else if (option[i] == 'r')
			con->rev = 1;
		else if (option[i] == 'l')
			con->llong = 1;
		i++;
	}
	
}

void	readfiles(char *file, t_list **head)
{
	// chek if file exist
	
	int fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror("ft_ls");
		exit(2);
	}
	ft_lstadd_back(head, ft_lstnew(file));

}

void parss_args(char **argv, t_config *con , t_list **lst)
{
	int i = 1;
	//t_config *con = malloc(sizeof(t_config));
	//t_list *lst;
	while (argv[i])
	{
		if (argv[i][0] == '-')
		{
			readoptions(argv[i], con);
		}
		else
		{
			readfiles(argv[i], lst);
		}
		i++;
	}

}

int main(int argc, char **argv)
	{
	char *path = ".";
	t_list *lst = NULL;
	t_config *con = malloc(sizeof(t_config));
	ft_memset(con, 0, sizeof(t_config));
///	if (argc >  1)
///		path = argv[1];
	parss_args(argv, con,&lst); 

	DIR *dir ;
	
	struct stat sb;
	long total = 0;
	while (lst)
	{
		dir =  opendir(lst->content);
		struct dirent *info;
		while ((info = readdir(dir)))
		{
			path = ft_strjoin(lst->content, "/");
			path = ft_strjoin(path , info->d_name);
			if (stat(path, &sb) < 0)
			{
				perror("ft_ls");
				exit(2);
			}
			if (con->llong)
				print_long(sb, info->d_name);
			else
				printf("%s\t", info->d_name);
			ft_memset(&sb, 0, sizeof(struct stat));
		}
		closedir(dir);
		lst = lst->next;
	}
	return 0;
}
