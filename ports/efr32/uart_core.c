#include <unistd.h>
#include "py/mpconfig.h"
#include "em_leuart.h"
#include "em_usart.h"

/*
 * Core UART functions to implement for a port
 */

// Receive single character
int mp_hal_stdin_rx_chr(void) {
#if USE_LEUART
    return LEUART_Rx(LEUART0);
#else
	return USART_Rx(USART0);
#endif
}

// Send string of given length
void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {
    while (len--) {
#if USE_LEUART
		LEUART_Tx(LEUART0, *str++);
#else
		USART_Tx(USART0, *str++);
#endif
    }
}
