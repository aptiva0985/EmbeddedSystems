#ifndef _LOCK_H_
#define _LOCK_H_

int mutex_create(void);
int mutex_lock(int mutex);
int mutex_unlock(int mutex);

#endif
