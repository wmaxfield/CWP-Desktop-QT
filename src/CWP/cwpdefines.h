#ifndef CWPDEFINES_H
#define CWPDEFINES_H


#define MY_PROGRAM_NAME "cwp"
#ifndef int16_t
typedef short int16_t;
#endif


#ifndef BOOL
#define BOOL unsigned short
#endif

#ifndef UIDeviceOrientation
#define UIDeviceOrientation int
#endif
#ifndef UIInterfaceOrientation
#define UIInterfaceOrientation int
#endif

#define UIBarButtonItem QPushButton
#define UILabel QLabel

#ifndef Nil
#define Nil nullptr
#endif

#ifndef YES
#define YES 1
#endif

#ifndef NO
#define NO 0
#endif


#define STUDENT_OR_TEACHER 1
#define CWP_TEACHER 1
#if STUDENT_OR_TEACHER
#define STUDENT 1
// uncomment for debug version
//#define DEBUG_VERSION 1
#endif

#if RELEASE_VERSION
    #define ENABLE_DEBUG 1  /* set to 0 to disable debug screens and menus */
    #define DEBUG_HTML   1
    #define DEBUG_MENUS  2  /* set each bit in this long word for debug flavors*/
#else
    #define ENABLE_DEBUG 1  /* set to 0 to disable debug screens and menus */
    #define DEBUG_HTML   1
    #define DEBUG_MENUS  2  /* set each bit in this long word for debug flavors*/
#endif

#endif // CWPDEFINES_H
