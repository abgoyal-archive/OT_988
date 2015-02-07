# bcm4329
DHDCFLAGS = -DLINUX -DBCMDRIVER -DBCMDONGLEHOST -DDHDTHREAD -DBCMWPA2         \
	-DUNRELEASEDCHIP -Dlinux -DDHD_SDALIGN=64 -DMAX_HDR_READ=64           \
	-DDHD_FIRSTREAD=64 -DDHD_GPL -DDHD_SCHED -DBDC -DTOE -DDHD_BCMEVENTS  \
	-DSHOW_EVENTS -DBCMSDIO -DDHD_GPL -DBCMLXSDMMC -DBCMPLATFORM_BUS      \
	-Wall -Wstrict-prototypes \
	-DCUSTOM_OOB_GPIO_NUM=21 								\
	-DSET_RANDOM_MAC_SOFTAP   -DSOFTAP    -DOOB_INTR_ONLY      \
	-DOEM_ANDROID -DBCMWAPI_WPI -DBCMWAPI_WAI					 \
	-DEMBEDDED_PLATFORM -DARP_OFFLOAD_SUPPORT -DPKT_FILTER_SUPPORT 		\
	-DDHD_DEBUG_TRAP -DCSCAN 	\
	-DGET_CUSTOM_MAC_ENABLE 		\
	-I$(src)/src/include/ \
	-I$(src)/src/dhd/sys/ \
	-I$(src)/src/dongle/ \
	-I$(src)/src/bcmsdio/sys/ \
	-I$(src)/src/wl/sys/ \
	-I$(src)/src/shared/

DHDOFILES = src/dhd/sys/dhd_linux.o \
	    src/shared/linux_osl.o \
	    src/shared/bcmutils.o \
	    src/shared/bcmevent.o \
	    src/dhd/sys/dhd_common.o \
	    src/dhd/sys/dhd_custom_gpio.o \
	    src/wl/sys/wl_iw.o \
	    src/shared/siutils.o \
	    src/shared/sbutils.o \
	    src/shared/aiutils.o \
	    src/shared/hndpmu.o \
	    src/dhd/sys/dhd_bta.o \
	    src/shared/bcmwifi.o \
	    src/dhd/sys/dhd_sdio.o \
	    src/dhd/sys/dhd_linux_sched.o \
	    src/dhd/sys/dhd_cdc.o \
	    src/bcmsdio/sys/bcmsdh_sdmmc.o \
	    src/bcmsdio/sys/bcmsdh.o \
	    src/bcmsdio/sys/bcmsdh_linux.o \
	    src/bcmsdio/sys/bcmsdh_sdmmc_linux.o


obj-$(CONFIG_BCM4330) += bcm4330.o
bcm4330-objs += $(DHDOFILES)
EXTRA_CFLAGS = $(DHDCFLAGS)
EXTRA_LDFLAGS += --strip-debug
