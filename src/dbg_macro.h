/*
 * dbg_macro.h
 *
 *  Created on: 03 gen 2016
 *      Author: francesco
 */

#ifndef DBG_MACRO_H_
#define DBG_MACRO_H_

/* DBG: Function-like macro for debug, when macro DEBUG is defined DBG_ALT is
 * expanded into its first argument, otherwise its expanded into its second
 * argument. Arguments can be void, but is preferable to use a empty statement
 * (a single semicolon) instead.
 */
#ifdef DEBUG
#define DBG(x, y) x
#else
#define DBG(x, y) y
#endif

#endif /* DBG_MACRO_H_ */
