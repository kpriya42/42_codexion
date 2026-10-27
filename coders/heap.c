/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 13:49:11 by kri-              #+#    #+#             */
/*   Updated: 2026/10/27 15:24:06 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	prioq_push(t_prioQ *heap, t_slotreq *request)
{
	t_uint	index;

	if (heap->slots[0].coder_id == 0)
		index = 0;
	else if (heap->slots[1].coder_id == 0)
		index = 1;
	else
		return ;
	heap->slots[index].coder_id = request->coder_id;
	heap->slots[index].priority = request->priority;
	//heap->slots[index].fifo_priority = request->fifo_priority;
}

/*
Get highest-priority job, but don't remove
*/
t_uint	prioq_peek(t_prioQ *heap)
{
	if (heap->slots[0].coder_id == 0 && heap->slots[1].coder_id == 0)
		return (0);
	if (heap->slots[0].coder_id == 0)
		return (heap->slots[1].coder_id);
	if (heap->slots[1].coder_id == 0)
		return (heap->slots[0].coder_id);
	if (heap->slots[0].priority < heap->slots[1].priority)
		return (heap->slots[0].coder_id);
	else
		return (heap->slots[1].coder_id);
}

/*
 Get highest-priority job AND remove it
*/
t_uint	prioq_pop(t_prioQ *heap)
{
	t_uint	id;

	id = prioq_peek(heap);
	if (heap->slots[0].coder_id == id)
		heap->slots[0].coder_id = 0;
	else if (heap->slots[1].coder_id == id)
		heap->slots[1].coder_id = 0;
	return (id);
}

void	prioq_remove(t_prioQ *heap, t_slotreq *request)
{
	int	index;

	index = 0;
	while (index < 2)
	{
		if (heap->slots[index].coder_id == request->coder_id)
		{
			heap->slots[index].coder_id = 0;
			heap->slots[index].priority = 0;
			//heap->slots[index].fifo_priority = 0;
		}
		index++;
	}
}
