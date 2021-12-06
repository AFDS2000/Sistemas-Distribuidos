/* Grupo 31
 * André Cabaço 53457
 * André Seixas 53870
 * Miguel Agostinho 53475
 */

#ifndef _CTRL_MUTEX_PRIVATE_H
#define _CTRL_MUTEX_PRIVATE_H


void lock_read(int ticket);

void unlock_read();

void lock_write(int ticket);

void unlock_write();

void lock_stats();

void unlock_stats();


#endif