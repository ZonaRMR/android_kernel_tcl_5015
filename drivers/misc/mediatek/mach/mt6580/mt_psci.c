#include <linux/init.h>
#include <linux/smp.h>

#include <asm/cpu_ops.h>
#include <asm/psci.h>
#include <mach/mt_spm_mtcmos.h>

extern const struct cpu_operations cpu_psci_ops;

#ifdef CONFIG_SMP

static int __init mt_psci_cpu_init(struct device_node *dn, unsigned int cpu)
{
	return 0;
}

static int __init mt_psci_cpu_prepare(unsigned int cpu)
{
	if (cpu == 1)
		spm_mtcmos_cpu_init();
	return cpu_psci_ops.cpu_prepare(cpu);
}

static int mt_psci_cpu_boot(unsigned int cpu)
{
	int ret;
	ret = cpu_psci_ops.cpu_boot(cpu);
	if (ret < 0)
		return ret;

	return spm_mtcmos_ctrl_cpu(cpu, STA_POWER_ON, 1);
}

#ifdef CONFIG_HOTPLUG_CPU
static int cpu_psci_cpu_disable(unsigned int cpu)
{
	return cpu_psci_ops.cpu_disable(cpu);
}

static void cpu_psci_cpu_die(unsigned int cpu)
{
	cpu_psci_ops.cpu_die(cpu);
}
#endif

const struct cpu_operations mt_cpu_psci_ops = {
	.name		= "mt-boot",
	.cpu_init	= mt_psci_cpu_init,
	.cpu_prepare	= mt_psci_cpu_prepare,
	.cpu_boot	= mt_psci_cpu_boot,
#ifdef CONFIG_HOTPLUG_CPU
	.cpu_disable	= mt_psci_cpu_disable,
	.cpu_die	= mt_psci_cpu_die,
#endif
};

#endif
