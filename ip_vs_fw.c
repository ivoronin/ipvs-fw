/*
 * IPVS:        Fixed-Weight Scheduling module
 *
 * Authors:     Ilya Voronin <ivoronin@jet.msk.su>
 *
 *              This program is free software; you can redistribute it and/or
 *              modify it under the terms of the GNU General Public License
 *              as published by the Free Software Foundation; either version
 *              2 of the License, or (at your option) any later version.
 *
 */

#define KMSG_COMPONENT "IPVS"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt

#include <linux/module.h>
#include <linux/kernel.h>

#include <net/ip_vs.h>

/*
 *	Fixed Weight scheduling
 */
static struct ip_vs_dest *
ip_vs_fw_schedule(struct ip_vs_service *svc, const struct sk_buff *skb)
{
	struct ip_vs_dest *dest, *heaviest = NULL;
	unsigned int maxweight = 0, weight;

	IP_VS_DBG(6, "%s(): Scheduling...\n", __func__);

	list_for_each_entry_rcu(dest, &svc->destinations, n_list) {
		weight = atomic_read(&dest->weight);
		if ((dest->flags & IP_VS_DEST_F_OVERLOAD) ||
		    weight == 0)
			continue;
		if (weight > maxweight) {
			heaviest = dest;
			maxweight = weight;
		}
	}

	if (!heaviest)
		ip_vs_scheduler_err(svc, "no destination available");
	else
		IP_VS_DBG_BUF(6, "FW: server %s:%u activeconns %d "
			      "inactconns %d weight %d\n",
			      IP_VS_DBG_ADDR(svc->af, &heaviest->addr),
			      ntohs(heaviest->port),
			      atomic_read(&heaviest->activeconns),
			      atomic_read(&heaviest->inactconns),
			      atomic_read(&heaviest->weight));

	return heaviest;
}


static struct ip_vs_scheduler ip_vs_fw_scheduler = {
	.name =			"fw",
	.refcnt =		ATOMIC_INIT(0),
	.module =		THIS_MODULE,
	.n_list =		LIST_HEAD_INIT(ip_vs_fw_scheduler.n_list),
	.schedule =		ip_vs_fw_schedule,
};


static int __init ip_vs_fw_init(void)
{
	return register_ip_vs_scheduler(&ip_vs_fw_scheduler) ;
}

static void __exit ip_vs_fw_cleanup(void)
{
	unregister_ip_vs_scheduler(&ip_vs_fw_scheduler);
	synchronize_rcu();
}

module_init(ip_vs_fw_init);
module_exit(ip_vs_fw_cleanup);
MODULE_LICENSE("GPL");
