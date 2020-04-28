# include "ft_ls.h"


static int cmp(void *a, void *b)
{
	return strcmp(a, b);
}

static  void handler(char *dir_name, t_list *lst, t_config *con)
{
	
	printf("%s : \n", dir_name);
	sort_list(&lst, cmp);
	handle_display(dir_name,lst, con, 0);
	printf("\n");
}
void recurseve(char *start, t_config *con)
{
	char *path_mode;
	DIR *dir = opendir(start);
	if (dir  == NULL)
	{
		perror("(ft_ls) :");
		printf("%s\n", start);
		return ;
	}
	struct dirent *info;
	struct stat sb;
	t_list *dir_list = NULL;
	int first = 0;
	//list = ft_lstnew(ft_strdup("."));
	start = ft_strjoin(start, "/");
	t_list *file_list =  NULL;
	while ((info = readdir(dir)))
	{
		if (stat(ft_strjoin(start, info->d_name), &sb) == -1)
		{
			printf("[%s]\n", info->d_name);
			perror("ft_ls");
		//	exit(2);
			closedir(dir);
			return ;
		}
		ft_lstadd_back(&file_list, ft_lstnew(ft_strdup(info->d_name)));
	//	printf("%s\n", info->d_name);
		if (S_ISDIR(sb.st_mode))
		{
			//	printf("(%s)\n",info->d_name);
			if ( !ft_strchr(info->d_name, '.'))
				ft_lstadd_back(&dir_list, ft_lstnew(ft_strjoin(start, ft_strdup(info->d_name))));
		}
	}
	handler(start, file_list, con);
	while (dir_list)
	{

		//		printf("%s\n", list->content);
		recurseve(dir_list->content, con);
		dir_list = dir_list->next;
	}
	closedir(dir);
}


