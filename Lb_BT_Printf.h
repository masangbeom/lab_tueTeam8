


#ifndef  _LB_BT_PRINTF_H
#define  _LB_BT_PRINTF_H


typedef char *Lb_va_list;


#define Lb_va_start(ap, p)			(ap = (char *) (&(p)+1))
#define Lb_va_arg(ap, type)		((type *) (ap += sizeof(type)))[-1]
#define Lb_va_end(ap)

void Lb_BT_printf( char *fmt, ... );
void Lb_BT_sprintf( char *pStr, char *fmt, ... );
void Lb_BT_vsprintf( char *pStr, char *fmt, Lb_va_list args );

#endif



