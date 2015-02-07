/* Copyright (c) 2009-2011, Code Aurora Forum. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of Code Aurora Forum, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef _ARCH_ARM_MACH_MSM_SOCINFO_H_
#define _ARCH_ARM_MACH_MSM_SOCINFO_H_

#include <linux/init.h>

/*
 * SOC version type with major number in the upper 16 bits and minor
 * number in the lower 16 bits.  For example:
 *   1.0 -> 0x00010000
 *   2.3 -> 0x00020003
 */
#define SOCINFO_VERSION_MAJOR(ver) ((ver & 0xffff0000) >> 16)
#define SOCINFO_VERSION_MINOR(ver) (ver & 0x0000ffff)

enum msm_cpu {
	MSM_CPU_UNKNOWN = 0,
	MSM_CPU_7X01,
	MSM_CPU_7X25,
	MSM_CPU_7X27,
	MSM_CPU_8X50,
	MSM_CPU_8X50A,
	MSM_CPU_7X30,
	MSM_CPU_8X55,
	MSM_CPU_8X60,
};

#ifdef CONFIG_M7LAB_VENUS	//	Teddy 2011-02-23
#define HW_VERSION(major, minor)  (((major) << 16) | (minor))
enum hw_id_rev {
	HW_ID_REV_NONE,
	HW_ID_REV_V1A = HW_VERSION(1,0),
	HW_ID_REV_V2A = HW_VERSION(2,0),
	HW_ID_REV_V3A = HW_VERSION(3,0),
	HW_ID_REV_V4A = HW_VERSION(4,0),
	HW_ID_REV_V5A = HW_VERSION(5,0),
	HW_ID_REV_V6A = HW_VERSION(6,0),
	HW_ID_REV_V7A = HW_VERSION(7,0),
	HW_ID_REV_V8A = HW_VERSION(8,0),
	HW_ID_REV_V9A = HW_VERSION(9,0),
	HW_ID_REV_MAX = 0xFFFFFFFF
};

enum hw_id_carrier {
	HW_ID_CRICKET	= 0x02,
	HW_ID_VENUS		= 0x03,
	HW_ID_NONE		= 0xFF,
	HW_ID_MAX = HW_ID_NONE
};
#endif	//	CONFIG_M7LAB_VENUS

enum msm_cpu socinfo_get_msm_cpu(void);
uint32_t socinfo_get_id(void);
uint32_t socinfo_get_version(void);
char *socinfo_get_build_id(void);
uint32_t socinfo_get_platform_type(void);
uint32_t socinfo_get_platform_version(void);
int __init socinfo_init(void) __must_check;
#ifdef CONFIG_M7LAB_VENUS	//	Teddy 2011-02-23
#ifdef CONFIG_M7LAB_JUKE_USB	//	Teddy 2011-07-08
uint32_t socinfo_get_npi_mode(void);
#endif	//	CONFIG_M7LAB_JUKE_USB
uint32_t socinfo_get_carrier_id(void);
uint32_t socinfo_get_hw_id(void);
#endif	//	CONFIG_M7LAB_VENUS

static inline int cpu_is_msm7x01(void)
{
	enum msm_cpu cpu = socinfo_get_msm_cpu();

	BUG_ON(cpu == MSM_CPU_UNKNOWN);
	return cpu == MSM_CPU_7X01;
}

static inline int cpu_is_msm7x25(void)
{
	enum msm_cpu cpu = socinfo_get_msm_cpu();

	BUG_ON(cpu == MSM_CPU_UNKNOWN);
	return cpu == MSM_CPU_7X25;
}

static inline int cpu_is_msm7x27(void)
{
	enum msm_cpu cpu = socinfo_get_msm_cpu();

	BUG_ON(cpu == MSM_CPU_UNKNOWN);
	return cpu == MSM_CPU_7X27;
}

static inline int cpu_is_msm7x30(void)
{
	enum msm_cpu cpu = socinfo_get_msm_cpu();

	BUG_ON(cpu == MSM_CPU_UNKNOWN);
	return cpu == MSM_CPU_7X30;
}

static inline int cpu_is_qsd8x50(void)
{
	enum msm_cpu cpu = socinfo_get_msm_cpu();

	BUG_ON(cpu == MSM_CPU_UNKNOWN);
	return cpu == MSM_CPU_8X50;
}

static inline int cpu_is_qsd8x50a(void)
{
	enum msm_cpu cpu = socinfo_get_msm_cpu();

	BUG_ON(cpu == MSM_CPU_UNKNOWN);
	return cpu == MSM_CPU_8X50A;
}

static inline int cpu_is_msm8x55(void)
{
	enum msm_cpu cpu = socinfo_get_msm_cpu();

	BUG_ON(cpu == MSM_CPU_UNKNOWN);
	return cpu == MSM_CPU_8X55;
}

static inline int cpu_is_msm8x60(void)
{
	enum msm_cpu cpu = socinfo_get_msm_cpu();

	BUG_ON(cpu == MSM_CPU_UNKNOWN);
	return cpu == MSM_CPU_8X60;
}

#endif
