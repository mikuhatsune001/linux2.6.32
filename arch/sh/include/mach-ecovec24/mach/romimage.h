/* EcoVec board specific boot code:
 * converts the "partner-jet-script.txt" script into assembly
 * the assembly code is the first code to be executed in the romImage
 */

#include <asm/romimage-macros.h>
#include "partner-jet-setup.txt"

	/* execute icbi after enabling cache */
	mov.l	1f, r0
	icbi	@r0

	/* jump to cached area */
	mova	2f, r0
	jmp	@r0
	nop

	.align 2
1 :	.long 0xa8000000
2 :
