/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 13:49:11 by kri-              #+#    #+#             */
/*   Updated: 2026/11/06 17:42:11 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
Add the new request as per its priority
*/

t_uint	prioq_push(t_prioQ *heap, t_slotreq *request)
{
	t_uint	index;

	if (heap->slots[1].coder_id != 0)
		return (0);
	if (heap->slots[0].coder_id == 0)
		index = 0;
	else if (request->priority < heap->slots[0].priority)
	{
		heap->slots[1] = heap->slots[0];
		index = 0;
	}
	else
		index = 1;
	heap->slots[index] = *request;
	return (heap->slots[index].coder_id);
}

/*
Get highest-priority job, but don't remove
*/
t_uint	prioq_peek(t_prioQ *heap)
{
	return (heap->slots[0].coder_id);
}

/*
 Get highest-priority job AND remove it
*/
t_uint	prioq_pop(t_prioQ *heap)
{
	t_uint	id;

	id = prioq_peek(heap);
	if (id == 0)
		return (0);
	heap->slots[0] = heap->slots[1];
	heap->slots[1].coder_id = 0;
	heap->slots[1].priority = 0;
	return (id);
}

/*
 Remove the specific request
*/
void	prioq_remove(t_prioQ *heap, t_slotreq *request)
{
	if (heap->slots[0].coder_id == request->coder_id)
	{
		heap->slots[0] = heap->slots[1];
	}
	else if (heap->slots[1].coder_id != request->coder_id)
		return ;
	heap->slots[1].coder_id = 0;
	heap->slots[1].priority = 0;
}
