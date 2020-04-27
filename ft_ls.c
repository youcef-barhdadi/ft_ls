# include "ft_ls.h"




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
	if (S_ISLNK(st.st_mode))
		file->permssion[0] = 'l'; 
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
	else if (S_ISREG(st.st_mode))
		file->permssion[0] = '-'; 
	else
		file->permssion[0] = ' '; 
	return file;
}


void print_long(struct stat st, char *filename, char *path)
{
	char buffer[1000];

	t_file *file = malloc(sizeof(t_file));
	file->size = st.st_size;
	file->time = st.st_ctime;
	file->links = st.st_nlink;
	if (S_ISLNK(st.st_mode))
	{
		ssize_t size = readlink(path, buffer, 1000);
		if (size == -1)
		{
			perror("ft_ls ");
			exit(1);
		}
		buffer[size] = '\0';
		file->referto = ft_strdup(buffer);
		file->islink  = 1;
	}
	get_permssion(st, file);
	get_file_type(st, file);
	get_usergroup_names(st, file);
	char *time = ft_substr(ctime(&file->time),4,12);
	printf("%s %lld %s %s %lld %s %s",file->permssion,file->links ,file->user_name, file->group_name, file->size,time, filename);
	if (S_ISLNK(st.st_mode))
	{
		printf(" -> %s", file->referto);
	}
	printf("\n");
	free(file);
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

t_list 	*file_to_list(char *filename)
{
	t_list *list = NULL;
	DIR *dir;
	struct dirent *info;


	dir = opendir(filename);
	while ((info = readdir(dir)))
	{

		ft_lstadd_back(&list, ft_lstnew(ft_strdup(info->d_name)));

	}

	closedir(dir);


	return list;

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


int cmp_str(void *s1, void *s2)
{

	return strcmp(s1, s2);
}

int cmp_str_rev(void *s1, void *s2)
{

	return  strcmp(s1, s2) * -1;
}


char *get_path(char *dir, char *filename)
{
	if (ft_strlen(dir) == 1 && dir[0] == '/')
	return ft_strjoin("/", filename);
	char *b = ft_strjoin(dir, "/");
	return ft_strjoin(b, filename);

}

void handle_display(char *dir,t_list *list, t_config *con, int file)
{

	struct stat sb;

	char *path;



	if (file)
	{

		if (con->llong)
		{
			if(lstat(dir, &sb) < 0)
			{

				perror("ft_ls :");
				exit(1);
			}

			print_long(sb, dir, dir);

		}
		else
		{
			printf("%s\n", dir);
		}
	}


	while (list)
	{


		if (con->llong)
		{
			path = get_path(dir, list->content);
			if (con->hidden || ((char *)list->content)[0] != '.')
			{	ft_memset(&sb, 0, sizeof(struct stat));
				if(lstat(path, &sb) < 0)
				{

					printf("%s\n", path);
					perror("ft_ls :");
					exit(1);
				}

				print_long(sb, list->content, path);
				//printf("%s %d\n", path, S_ISLNK(sb.st_mode));
			
			}
		}
		else
		{
			if (con->hidden ||  ((char *)list->content)[0] != '.')
				printf("%s\t", list->content);
		}
		list = list->next;
	}




}
int main(int argc, char **argv)
{
	char *path = ".";
	int file = 0;
	t_list *lst = NULL;
	t_config *con = malloc(sizeof(t_config));
	ft_memset(con, 0, sizeof(t_config));
	///	if (argc >  1)
	///		path = argv[1];
	parss_args(argv, con,&lst); 

	DIR *dir ;
	t_list *list;

	struct stat sb;
	long total = 0;
	while (lst)
	{
		if ( stat(lst->content, &sb) == -1)
		{
			perror("ft_ls ");
			printf("file name %s\n", lst->content);
			return 1;
		}
		if (S_ISDIR(sb.st_mode))
		{
			//dir =  opendir(lst->content);
			list  = 	file_to_list(lst->content);
			if (!con->rev)
				sort_list(&list, &cmp_str);
			else
				sort_list(&list, &cmp_str_rev);
		}
		else
			file = 1;
		handle_display(lst->content, list, con, file);
		file = 0;
		//closedir(dir);
		lst = lst->next;
	}
	return 0;
}
