/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcristi@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 16:53:37 by alcristi          #+#    #+#             */
/*   Updated: 2021/08/17 17:02:08 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new;
	t_list	*aux;

	if (!lst)
	{
		return (0);
	}
	new = 0;
	while (lst)
	{	
		aux = ft_lstnew(f(lst->content));
		if (!aux)
		{
			ft_lstclear(&new, del);
			return (0);
		}
		ft_lstadd_back(&new, aux);
		lst = lst->next;
	}
	return (new);
}
