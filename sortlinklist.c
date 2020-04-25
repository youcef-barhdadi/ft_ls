# include "ft_ls.h"

static void  split(t_list *head, t_list **first, t_list **second)
{
	t_list *fast;
	t_list *slow;

	slow = head;
	fast = head->next;

	while (fast !=  NULL)
	{
		fast = fast->next;
		if (fast != NULL)
		{
			fast = fast->next;
			slow = slow->next;
		}
	
	}

	*first = head;
	*second = slow->next;

	slow->next = NULL;

}

static t_list *sorted_marge(t_list *first, t_list *second, int (*cmp)(void *a, void *b))
{

	t_list *res;

	if (first == NULL)
		return second;
	else if (second == NULL)
		return first;
	if (cmp (first->content, second->content) < 0)
	{
		res = first;
		res->next = sorted_marge(first->next, second, cmp);
	}
	else {
	
		res = second;
		res->next = sorted_marge(first, second->next, cmp);
	
	}
	return res;

}

void sort_list(t_list **list, int (*cmp)(void *a, void *b))
{
	t_list *node;
	t_list *head = *list;
	t_list *a;
	t_list *b;


	if (list == NULL || head->next == NULL)
		return ;
	split(head, &a, &b);
	sort_list(&a, cmp);
	sort_list(&b, cmp);
	*list = sorted_marge(a, b, cmp);
}
