#include "mgos.h"
#include "spi_flash.h"
#include "esp_rboot.h"
#define BUTTON_PIN 14

enum mgos_app_init_result mgos_app_init(void) {
	// if skip button is pressed, do not do anything.
	// this way the firmware image can be read from the deivce
	mgos_gpio_setup_input(BUTTON_PIN, MGOS_GPIO_PULL_UP);
	if ( !mgos_gpio_read(BUTTON_PIN) ) {
		LOG(LL_INFO, ("Skip button pressed. Will not alter bootloader."));
		return MGOS_APP_INIT_SUCCESS;
	}

	// detect devices flash size and adapt image to it
	unsigned char *buffer = NULL;
	buffer = (unsigned char*) malloc(SECTOR_SIZE);
	if ( spi_flash_read(0, (uint32 *) buffer, SECTOR_SIZE) != 0 ) {
		LOG(LL_ERROR, ("flash read error! abort."));
		return MGOS_APP_INIT_SUCCESS;
	}

	unsigned char *loaded_flash_size = buffer+3;
	uint8 true_flash_size = 0;
	switch( (1 << ((spi_flash_get_id() >> 16) & 0xFF)) ) {
	case 2097152:
		true_flash_size = 0x3f; // 0x0f = 512KB; 0x2f = 1MB; 0x3f = 2MB; 0x4f = 4MB
		break;
	case 4194304:
		true_flash_size = 0x4f;
		break;
	default:
		LOG(LL_ERROR, ("flash size %i not supported! abort.", (1 << ((spi_flash_get_id() >> 16) & 0xFF)) ));
		return MGOS_APP_INIT_SUCCESS;
	}
	if (*loaded_flash_size == true_flash_size) {
		LOG(LL_INFO, ("All done. Waiting for updates."));
		return MGOS_APP_INIT_SUCCESS;
	}

	LOG(LL_INFO, ("Change flash size from %02x to %02x and reboot", *loaded_flash_size, true_flash_size ));
	*loaded_flash_size = true_flash_size;
	spi_flash_erase_sector(0);
	spi_flash_write(0, (uint32 *) buffer, SECTOR_SIZE);
	free(buffer);

	// tell rboot to move esp system config at next boot
	rboot_config *rboot_cfg;
	rboot_cfg = get_rboot_config();
	(*rboot_cfg).sdk_config_source = 524288;
	rboot_set_config(rboot_cfg);

	mgos_system_restart_after(200);
	return MGOS_APP_INIT_SUCCESS;
}
