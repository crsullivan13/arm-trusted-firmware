/*
 * Copyright (c) 2024, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>

#include <common/debug.h>
#include <common/runtime_svc.h>
#include <lib/debugfs.h>
#include <lib/pmf/pmf.h>
#include <services/l3_config_svc.h>
#include <tools_share/uuid.h>


static int l3_config_svc_setup(void)
{
#if USE_DEBUGFS
	if (debugfs_smc_setup() != 0) {
		return 1;
	}
#endif /* USE_DEBUGFS */

#if ENABLE_PMF
	if (pmf_setup() != 0) {
		return 1;
	}
#endif /* ENABLE_PMF */

	return 0;
}

/*
 * This function handles Arm defined vendor-specific EL3 Service Calls.
 */
static uintptr_t l3_config_svc_handler(unsigned int smc_fid,
			u_register_t x1,
			u_register_t x2,
			u_register_t x3,
			u_register_t x4,
			void *cookie,
			void *handle,
			u_register_t flags)
{
	switch (smc_fid)
	{
	case L3_CONFIG_SMC:
		set_actlr_el3();
		set_cluster_part_el1(x1);
		set_cluster_acp_sid_el1(x2);
		set_cluster_stash_id_el1(x3);
		set_cluster_thread_sidovr_el1(x4);
		set_actlr_el2();


		SMC_RET1(handle, SMC_OK);
		break;
	default:
		WARN("Unimplemented EL3 Service call: 0x%x\n", smc_fid);
		SMC_RET1(handle, SMC_UNK);
		break;
	}
}

/* Define a runtime service descriptor for fast SMC calls */
DECLARE_RT_SVC(
	l3_config_svc,
	OEN_TOS_START,
	OEN_TOS_END,
	SMC_TYPE_FAST,
	l3_config_svc_setup,
	l3_config_svc_handler
);
