/*
 * SSTF IO Scheduler - Implemented by Carlos Moratelli
 *
 * For Kernel 4.12.4
 * 
 */
#include <linux/blkdev.h>
#include <linux/elevator.h>
#include <linux/bio.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/list.h>
#include <limits.h>

/* SSTF data structure. */
struct sstf_data {
	struct list_head list;
	long sector;
};

static long last_sector = 0;

/* Esta função despacha o próximo bloco a ser lido. */
static int sstf_dispatch(struct request_queue *q, int force){
	struct sstf_data *nd = q->elevator->elevator_data;
	char direction = 'R';
	struct request *rq;
	
	/* Aqui deve-se retirar uma requisição da fila e enviá-la para processamento.
	 * Use como exemplo o driver noop-iosched.c. Veja como a requisição é tratada.
	 * 
	 * Antes de retornar da função, imprima o sector que foi atendido.
	 */
	
	 rq = list_first_entry_or_null(&nd->list, struct request, queuelist);
	 if (rq) {
		 list_del_init(&rq->queuelist);
		 last_sector = blk_rq_pos(rq);
		 elv_dispatch_add_tail(q, rq);
		 return 1;
	 }
	 return 0;
 }
}



static void sstf_add_request(struct request_queue *q, struct request *rq){
	struct sstf_data *nd = q->elevator->elevator_data;
	char direction = 'R';
	/* Aqui deve-se adicionar uma requisição na fila do driver.
	 * Use como exemplo o driver noop-iosched.c
	 * 
	 * Antes de retornar da função, imprima o sector que foi adicionado na lista.
	 */
	nd->sector = blk_rq_pos(rq);
	closest = LONG_MAX;
	list_head* best;

	sstf_data* current;

	list_for_each_entry(current, &rq->queuelist, list){
	prev_neighbour_dist = next_neighbour_dist = LONG_MAX;
	if (current){
		prev_neighbour_dist = abs(current->sector - nd->sector);
	}
	if (current->next){
		next_neighbour_dist = abs(current->sector - nd->sector);
	}

	if ( prev_neighbour_dist + next_neighbour_dist < closest ){
		closest = abs(prev_neighbour_dist) +  abs(next_neighbour_dist)
		best = current;
	}

	list_add(&nd->list, &best->list);

		
	printk(KERN_EMERG "[SSTF] add %c %lu\n", direction, blk_rq_pos(rq));


}


static int sstf_init_queue(struct request_queue *q, struct elevator_type *e){
	struct sstf_data *nd;
	struct elevator_queue *eq;

	eq = elevator_alloc(q, e);
	if (!eq)
		return -ENOMEM;

	nd = kmalloc_node(sizeof(*nd), GFP_KERNEL, q->node);
	if (!nd) {
		kobject_put(&eq->kobj);
		return -ENOMEM;
	}
	eq->elevator_data = nd;

	INIT_LIST_HEAD(&nd->list);

	spin_lock_irq(q->queue_lock);
	q->elevator = eq;
	spin_unlock_irq(q->queue_lock);
	return 0;
}

static void sstf_exit_queue(struct elevator_queue *e)
{
	struct sstf_data *nd = e->elevator_data;

	/* Implementação da finalização da fila (queue).
	 * 
	 * Use como exemplo o driver noop-iosched.c
	 * 
	 */

	BUG_ON(!list_empty(&nd->queue));
	kfree(nd);

}

/* Infrastrutura dos drivers de IO Scheduling. */
static struct elevator_type elevator_sstf = {
	.ops.sq = {
		.elevator_dispatch_fn		= sstf_dispatch,
		.elevator_add_req_fn		= sstf_add_request,
		.elevator_init_fn		= sstf_init_queue,
		.elevator_exit_fn		= sstf_exit_queue,
	},
	.elevator_name = "sstf",
	.elevator_owner = THIS_MODULE,
};

/* Inicialização do driver. */
static int __init sstf_init(void)
{
	elv_register(&elevator_sstf);

	return 0;
}

/* Finalização do driver. */
static void __exit sstf_exit(void)
{
	elv_unregister(&elevator_sstf);
}

module_init(sstf_init);
module_exit(sstf_exit);


MODULE_AUTHOR("Carlos Moratelli");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("SSTF IO scheduler");
