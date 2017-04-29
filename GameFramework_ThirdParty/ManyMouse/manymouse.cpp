/*
 * ManyMouse foundation code; apps talks to this and it talks to the lowlevel
 *  code for various platforms.
 *
 * Please see the file LICENSE.txt in the source's root directory.
 *
 *  This file written by Ryan C. Gordon.
 */

#include <stdlib.h>
#include "manymouse.h"

static const char *manymouse_copyright =
    "ManyMouse " MANYMOUSE_VERSION " copyright (c) 2005-2008 Ryan C. Gordon.";

extern const ManyMouseDriver *ManyMouseDriver_windows;

/*
 * These have to be in the favored order...obviously it doesn't matter if the
 *  Linux driver comes before or after the Windows one, since one won't
 *  exist on a given platform, but things like Mac OS X's hidmanager (which
 *  only works on OS X 10.5 and later) should come before Mac OS X's
 *  hidutilities (which works on older systems, but may stop working in 10.6
 *  and later). In the Mac OS X case, you want to try the newer tech, and if
 *  it's not available (on 10.4 or earlier), fall back to trying the legacy
 *  code.
 */
static const ManyMouseDriver **mice_drivers[] =
{
    #if 0  /* FIXME: re-add this when the code is written! */
    &ManyMouseDriver_xinput,
    #endif

    &ManyMouseDriver_windows,
};


static const ManyMouseDriver *driver = NULL;

int ManyMouse_Init(void)
{
    const int upper = (sizeof (mice_drivers) / sizeof (mice_drivers[0]));
    int i;
    int retval = -1;

    /* impossible test to keep manymouse_copyright linked into the binary. */
    if (manymouse_copyright == NULL)
        return(-1);

    if (driver != NULL)
        return(-1);

    for (i = 0; (i < upper) && (driver == NULL); i++)
    {
        const ManyMouseDriver *this_driver = *(mice_drivers[i]);
        if (this_driver != NULL) /* if not built for this platform, skip it. */
        {
            const int mice = this_driver->init();
            if (mice > retval)
                retval = mice; /* may move from "error" to "no mice found". */

            if (mice > 0)
                driver = this_driver;
        } /* if */
    } /* for */

    return(retval);
} /* ManyMouse_Init */


void ManyMouse_Quit(void)
{
    if (driver != NULL)
        driver->quit();
    driver = NULL;
} /* ManyMouse_Quit */


const char *ManyMouse_DeviceName(unsigned int index)
{
    if (driver != NULL)
        return(driver->name(index));
    return(NULL);
} /* ManyMouse_PollEvent */


int ManyMouse_PollEvent(ManyMouseEvent *event)
{
    if (driver != NULL)
        return(driver->poll(event));
    return(0);
} /* ManyMouse_PollEvent */

/* end of manymouse.c ... */

