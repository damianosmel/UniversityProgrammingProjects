#include "../system.h"

int do_kerdouble(m_ptr)
	message *m_ptr;
{
	/*getting the number by a int member of struct *message */
	int result;
	result=m_ptr->m1_i1;
	result=2*result;
	/*the result into the same message*/
	m_ptr->m1_i1=result;
	/*kprintf("\nhello from kernel ");*/
	return 0;

}
