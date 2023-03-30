/**
 * @file config.h
 * @brief Configuration constants for the lock
 * @version 0.1
 * @date 2023-03-21
 *
 * For each constant, it is also supported to define them from command line.
 */
#ifndef CONFIG_H
#define CONFIG_H

/**
 * @name Length lock
 *
 * PID constants and anti-windup for the length lock
 *
 * @{
 */
#ifndef LENGTH_KP
#define LENGTH_KP (0.0001)
#endif

#ifndef LENGTH_KI
#define LENGTH_KI (0.00005)
#endif

#ifndef LENGTH_KD
#define LENGTH_KD (0.0)
#endif

/**
 * \def LENGTH_LOCK_LOWER_BOUND
 * Lower bound for the length lock.
 *
 * The closer this value becomes to 0, the greater lock range,
 * but also suffers more from the non-linearity of the home-made
 * HV amplifer.
 *
 */
#ifndef LENGTH_LOCK_LOWER_BOUND
#define LENGTH_LOCK_LOWER_BOUND (0.1)
#endif

/**
 * \def LENGTH_LOCK_UPPER_BOUND
 * Upper bound for the length lock.
 *
 * The closer this value becomes to 1, the greater lock range,
 * but also suffers more from the non-linearity of the home-made
 * HV amplifer.
 */
#ifndef LENGTH_LOCK_UPPER_BOUND
#define LENGTH_LOCK_UPPER_BOUND (0.9)
#endif

/**@}*/

/**
 * @name Current lock
 *
 * PID constants and anti-windup for the current lock
 * @{
 */
#ifndef CURRENT_KP
#define CURRENT_KP (0.6)
#endif

#ifndef CURRENT_KI
#define CURRENT_KI (0.02)
#endif

#ifndef CURRENT_KD
#define CURRENT_KD (0.06)
#endif

/**
 * \def CURRENT_LOCK_LOWER_BOUND
 * Lower bound for the current lock. 
 * 
 * The closer this value becomes to 0, the greater lock range, 
 * but also suffers more from slow detection of unlock condition. 
 */
#ifndef CURRENT_LOCK_LOWER_BOUND
#define CURRENT_LOCK_LOWER_BOUND (0.4)
#endif

/**
 * \def CURRENT_LOCK_UPPER_BOUND
 * Upper bound for the current lock. 
 *  
 * The closer this value becomes to 1, the greater lock range, 
 * but also suffers more from slow detection of unlock condition. 
 */
#ifndef CURRENT_LOCK_UPPER_BOUND
#define CURRENT_LOCK_UPPER_BOUND (0.6)
#endif

/**@}*/

/**
 * @name Peak detection and relock strategy 
 * @{
 */

/**
 * \def SETP_TO_MIN_RATIO
 * The ratio between locking setpoint and the maximum transmission found.
 *
 * The value should be bounded by [0, 1], and the closer it gets to 1,
 * the more ideal slave output would become, and the more easily the unlock
 * happens.
 */
#ifndef SETP_TO_MIN_RATIO
#define SETP_TO_MIN_RATIO (0.99)
#endif

/**
 * \def RELOCK_WITH_SWEEP
 * 
 * Relock the slave by first drive the current high, hold for a pre-defined amount of time, and then sweep down the current. 
 * 
 * \note This is the default relocking method.  
 */
#ifndef RELOCK_WITH_STEP
#define RELOCK_WITH_SWEEP
#endif 

/**
 * \def RELOCK_WITH_STEP
 * 
 * Relock the slave by first drive the current high, hold for a pre-defined amount of time, then step down the current to a pre-defined value, and hold for another pre-defined amount of time. 
 * 
 * \note This is *NOT* the default relocking method. To activate it, 
 * either define it during compilation with "-DRELOCK_WITH_STEP" flag, 
 * or uncomment the #undef line below.   
 */
#ifndef RELOCK_WITH_STEP
#define RELOCK_WITH_STEP 
#undef RELOCK_WITH_STEP
#endif

/**
 * \def RELOCK_HIGH_CURRENT
 * 
 * The higher value that the current should drive to. 
 */
#define RELOCK_HIGH_CURRENT (.9)

/**
 * \def RELOCK_LOW_CURRENT
 * 
 * The higher value that the current should drive to. 
 * 
 * \note This macro is only used in strategy RELOCK_WITH_STEP.  
 */
#define RELOCK_LOW_CURRENT (.5)

/**
 * \def RELOCK_HIGH_HOLDTIME
 * 
 * The amount of time (in microseconds) in the high current before driving back to the lock point. The minimum amount of time needed depends on the thermal effect inside the laser diode. 
 */
#define RELOCK_HIGH_HOLDTIME (180000)

/**
 * \def RELOCK_LOW_HOLDTIME
 * 
 * The amount of time (in microseconds) in the low current before checking relock condition. The minimum amount of time needed depends on the thermal effect inside the laser diode. 
 * 
 * \note This macro is only used in strategy RELOCK_WITH_STEP.  
 */
#define RELOCK_LOW_HOLDTIME (40000)

/**
 * \def RELOCK_RATIO_TO_SETPOINT
 * 
 * 
 */
#define RELOCK_TO_SETPOINT_RATIO (0.98)
#endif