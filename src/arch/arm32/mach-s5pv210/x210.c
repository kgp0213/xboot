/*
 * x210.c
 *
 * Copyright(c) 2007-2018 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <xboot.h>
#include <mmu.h>

static const struct mmap_t mach_map[] = {
	{"ram",  0x32000000, 0x32000000, SZ_1M * 32, MAP_TYPE_CB},
	{"dma",  0x34000000, 0x34000000, SZ_1M * 32, MAP_TYPE_NCNB},
	{"heap", 0x36000000, 0x36000000, SZ_1M * 160, MAP_TYPE_CB},
	{ 0 },
};

static int mach_detect(struct machine_t * mach)
{
	virtual_addr_t virt;

	if((read32(phys_to_virt(0xe0000000)) >> 12) != 0x43110)
		return 0;
	virt = phys_to_virt(0xe010e81c);
	write32(virt, (read32(virt) & ~0x301) | 0x301);
	return 1;
}

static void mach_memmap(struct machine_t * mach)
{
	mmu_setup(mach->map);
}

static void mach_shutdown(struct machine_t * mach)
{
	virtual_addr_t virt = phys_to_virt(0xe010e81c);
	write32(virt, (read32(virt) & ~0x301) | 0x201);
}

static void mach_reboot(struct machine_t * mach)
{
}

static void mach_sleep(struct machine_t * mach)
{
}

static void mach_cleanup(struct machine_t * mach)
{
}

static void mach_logger(struct machine_t * mach, const char * buf, int count)
{
	virtual_addr_t virt = phys_to_virt(0xe2900800);
	int i;

	if((read32(virt + 0x8) & (1 << 0)))
	{
		for(i = 0; i < count; i++)
		{
			while((read32(virt + 0x18) & (1 << 24)));
			write8(virt + 0x20, buf[i]);
		}
	}
	else
	{
		for(i = 0; i < count; i++)
		{
			while(!(read32(virt + 0x10) & (1 << 1)));
			write8(virt + 0x20, buf[i]);
		}
	}
}

static const char * mach_uniqueid(struct machine_t * mach)
{
	return NULL;
}

static int mach_keygen(struct machine_t * mach, const char * msg, void * key)
{
	return 0;
}

static struct machine_t x210 = {
	.name 		= "x210",
	.desc 		= "X210 Based On Samsung S5PV210",
	.map		= mach_map,
	.detect 	= mach_detect,
	.memmap		= mach_memmap,
	.shutdown	= mach_shutdown,
	.reboot		= mach_reboot,
	.sleep		= mach_sleep,
	.cleanup	= mach_cleanup,
	.logger		= mach_logger,
	.uniqueid	= mach_uniqueid,
	.keygen		= mach_keygen,
};

static __init void x210_machine_init(void)
{
	register_machine(&x210);
}

static __exit void x210_machine_exit(void)
{
	unregister_machine(&x210);
}

machine_initcall(x210_machine_init);
machine_exitcall(x210_machine_exit);
