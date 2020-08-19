#include "mgos.h"
#include "spi_flash.h"

enum mgos_app_init_result mgos_app_init(void) {
	unsigned char *data = NULL;
	data = (unsigned char*) malloc(4096);
	if ( spi_flash_read (0, (uint32 *) data, 4096) != 0 ) {
		LOG(LL_ERROR, ("flash read error! abort."));
	} else {
		unsigned char *flip = data+3;
		if (*flip == TARGET_FLASH_SIZE) {
			LOG(LL_INFO, ("All done. Waiting for updates."));
			return MGOS_APP_INIT_SUCCESS;
		}

		LOG(LL_INFO, ("Change flash size from %02x to %02x and reboot", *flip, TARGET_FLASH_SIZE ));
		*flip = TARGET_FLASH_SIZE;
		spi_flash_erase_sector(0);
		spi_flash_write(0, (uint32 *) data, 4096);
		mgos_system_restart_after(200);
	}
	return MGOS_APP_INIT_SUCCESS;
}
