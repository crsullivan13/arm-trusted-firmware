#include <stdint.h>

#define L3_CONFIG_SMC 0x82000000

void set_actlr_el3() {
    uint64_t actlr = 0;
    asm volatile("mrs %0, ACTLR_EL3" : "=r" (actlr));
    actlr |= (1 << 10) | (1 << 11);
    asm volatile("msr ACTLR_EL3, %0" :: "r" (actlr));
}

//delegate control of CLUSTERTHREADSID_EL1 register to EL1
void set_actlr_el2() {
    uint64_t actlr = 0;
    asm volatile("mrs %0, ACTLR_EL2" : "=r" (actlr));
    actlr |= (1 << 10);
    asm volatile("msr ACTLR_EL2, %0" :: "r" (actlr));
}

//configure partitions
void set_cluster_part_el1(uint64_t value) {
    asm volatile("msr S3_0_C15_C4_3, %0" :: "r" (value));
}

//set which partition scheme ID ACP requests use
void set_cluster_acp_sid_el1(uint64_t value) {
    asm volatile("msr S3_0_C15_C4_1, %0" :: "r" (value));
}

//set which partition scheme ID stash requests use
void set_cluster_stash_id_el1(uint64_t value) {
    asm volatile("msr S3_0_C15_C4_2, %0" :: "r" (value));
}


//set some control bits
void set_cluster_thread_sidovr_el1(uint64_t value) {
    asm volatile("msr S3_0_C15_C4_7, %0" :: "r" (value));
}

//select between the L3 cache partitions allocated
void set_cluster_thread_sid_el1(uint64_t value) {
    asm volatile("msr S3_0_C15_C4_0, %0" :: "r" (value));
}