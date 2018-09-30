#ifndef FASTCALL_H
#define FASTCALL_H

#ifdef __x86_64
// The 64 bits calling convention is actually similar to 32's fastcall
# define FASTCALL
#else
# define FASTCALL __attribute__((fastcall))
#endif /* __x86_64 */

#endif /* !FASTCALL_H */
