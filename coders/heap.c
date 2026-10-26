/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 13:49:11 by kri-              #+#    #+#             */
/*   Updated: 2026/10/26 19:30:12 by kri-             ###   ########.fr       */
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
	heap->slots[index].edf_priority = request->edf_priority;
	heap->slots[index].fifo_priority = request->fifo_priority;
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
	return (heap->slots[0].coder_id);
}
