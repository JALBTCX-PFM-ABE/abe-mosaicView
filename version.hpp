
/*********************************************************************************************

    This is public domain software that was developed by or for the U.S. Naval Oceanographic
    Office and/or the U.S. Army Corps of Engineers.

    This is a work of the U.S. Government. In accordance with 17 USC 105, copyright protection
    is not available for any work of the U.S. Government.

    Neither the United States Government, nor any employees of the United States Government,
    nor the author, makes any warranty, express or implied, without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, or assumes any liability or
    responsibility for the accuracy, completeness, or usefulness of any information,
    apparatus, product, or process disclosed, or represents that its use would not infringe
    privately-owned rights. Reference herein to any specific commercial products, process,
    or service by trade name, trademark, manufacturer, or otherwise, does not necessarily
    constitute or imply its endorsement, recommendation, or favoring by the United States
    Government. The views and opinions of authors expressed herein do not necessarily state
    or reflect those of the United States Government, and shall not be used for advertising
    or product endorsement purposes.
*********************************************************************************************/


/****************************************  IMPORTANT NOTE  **********************************

    Comments in this file that start with / * ! or / / ! are being used by Doxygen to
    document the software.  Dashes in these comment blocks are used to create bullet lists.
    The lack of blank lines after a block of dash preceeded comments means that the next
    block of dash preceeded comments is a new, indented bullet list.  I've tried to keep the
    Doxygen formatting to a minimum but there are some other items (like <br> and <pre>)
    that need to be left alone.  If you see a comment that starts with / * ! or / / ! and
    there is something that looks a bit weird it is probably due to some arcane Doxygen
    syntax.  Be very careful modifying blocks of Doxygen comments.

*****************************************  IMPORTANT NOTE  **********************************/




#ifndef VERSION

#define     VERSION     "PFM Software - mosaicView V1.48 - 10/23/17"

#endif

/*

    Version 1.0
    Jan C. Depner
    10/05/07

    First working version.


    Version 1.01
    Jan C. Depner
    10/23/07

    Now honors the CHILD_PROCESS_FORCE_EXIT key.


    Version 1.02
    Jan C. Depner
    01/01/08

    Uses multiple movable objects as added to nvMap.


    Version 1.03
    Jan C. Depner
    01/04/08

    Now uses the parent process ID of the bin viewer plus _pfm or _abe for the shared memory ID's.  This removes the single instance
    per user restriction from ABE.


    Version 1.04
    Jan C. Depner
    01/14/08

    Uses check_target_schema (from utility library) to make sure that the schema in target files is correct.


    Version 1.05
    Jan C. Depner
    04/01/08

    Added acknowledgments to the Help pulldown menu.


    Version 1.06
    Jan C. Depner
    04/07/08

    Replaced single .h and .hpp files from utility library with include of nvutility.h and nvutility.hpp


    Version 1.07
    Jan C. Depner
    04/14/08

    Minor code cleanup.


    Version 1.08
    Jan C. Depner
    05/22/08

    Changed the window icon to match the Windows .ico icon.


    Version 1.09
    Jan C. Depner
    07/15/08

    Removed pfmShare shared memory usage and replaced with abeShare.


    Version 1.10
    Jan C. Depner
    08/28/08

    We were reading the targets twice and setting the cursor tracker twice on open.


    Version 1.11
    Jan C. Depner
    12/04/08

    Changed confidence level of new targets to 2 since they will be coming from a mosaic.  Also changed
    equipType to "other" (4) in the the Platform subelement.


    Version 1.12
    Jan C. Depner
    12/15/08

    Added screen capture to updateTarget.


    Version 1.20
    Jan C. Depner
    04/08/09

    Replaced support of NAVO standard target (XML) files with support for Binary Feature Data (BFD) files.


    Version 1.21
    Jan C. Depner
    04/13/09

    Use NINT instead of typecasting to NV_INT32 when saving Qt window state.  Integer truncation was inconsistent on Windows.


    Version 1.22
    Jan C. Depner
    04/15/09

    Added the ability to delete feature polygons.  Also added pencil cursor.


    Version 1.23
    Jan C. Depner
    04/23/09

    Changed the acknowledgments help to include Qt and a couple of others.


    Version 1.24
    Jan C. Depner
    04/27/09

    Replaced QColorDialog::getRgba with QColorDialog::getColor.


    Version 1.25
    Jan C. Depner
    05/21/09

    Fixed key press event screwup.  Set all QFileDialogs to use List mode instead of Detail mode.


    Version 1.26
    Jan C. Depner
    05/22/09

    Dealing with multiple resize events when "Display content in resizing windows" is 
    enabled.


    Version 1.27
    Jan C. Depner
    06/24/09

    Changed the BFD names to avoid collision with GNU Binary File Descriptor library.


    Version 1.28
    Jan C. Depner
    07/28/09

    Changed use of _sleep to Sleep on MinGW (Windows).


    Version 1.29
    Jan C. Depner
    09/11/09

    Fixed getColor calls so that cancel works.  Never forget!


    Version 1.30
    Jan C. Depner
    04/29/10

    Fixed posfix and fixpos calls to use numeric constants instead of strings for type.


    Version 1.31
    Jan C. Depner
    08/17/10

    Replaced our kludgy old UTM transform with calls to the PROJ 4 library functions.  All hail the PROJ 4 developers!


    Version 1.32
    Jan C. Depner
    09/08/10

    Changes to handle Qt 4.6.3.


    Version 1.33
    Jan C. Depner
    01/06/11

    Correct problem with the way I was instantiating the main widget.  This caused an intermittent error on Windows7/XP.


    Version 1.34
    Jan C. Depner
    04/15/11

    Fixed the geoTIFF reading by switching to using GDAL instead of Qt.  Hopefully Qt will get fixed eventually.


    Version 1.35
    Jan C. Depner
    11/30/11

    Converted .xpm icons to .png icons.


    Version 1.36
    Jan C. Depner
    12/29/11

    Fixed screenshot.cpp to grab both screens on Windows.  It will also scale the windows if
    they aren't the same height.


    Version 1.37
    Jan C. Depner (PFM Software)
    12/09/13

    Switched to using .ini file in $HOME (Linux) or $USERPROFILE (Windows) in the ABE.config directory.  Now
    the applications qsettings will not end up in unknown places like ~/.config/navo.navy.mil/blah_blah_blah on
    Linux or, in the registry (shudder) on Windows.


    Version 1.38
    Jan C. Depner (PFM Software)
    12/17/13

    Removed the landmask code.  Now that we have a really good coastline based on SWBD (and a mask built from it)
    we don't really need to draw the landmask anymore.  If you want to see where the landmask will be, just look
    at the coastline.


    Version 1.39
    Jan C. Depner (PFM Software)
    02/26/14

    Cleaned up "Set but not used" variables that show up using the 4.8.2 version of gcc.


    Version 1.40
    Jan C. Depner (PFM Software)
    02/28/14

    Reverted to using Qt to read geoTIFFs.  It quit working in Qt 4.7.2 but is working again.  It's
    much faster than using GDAL and it's probably a lot more bulletproof.


    Version 1.41
    Jan C. Depner (PFM Software)
    03/19/14

    - Straightened up the Open Source acknowledgments.


    Version 1.42
    Jan C. Depner (PFM Software)
    05/27/14

    - Added the new LGPL licensed GSF library to the acknowledgments.


    Version 1.43
    Jan C. Depner (PFM Software)
    07/01/14

    - Replaced all of the old, borrowed icons with new, public domain icons.  Mostly from the Tango set
      but a few from flavour-extended and 32pxmania.


    Version 1.44
    Jan C. Depner (PFM Software)
    07/23/14

    - Switched from using the old NV_INT64 and NV_U_INT32 type definitions to the C99 standard stdint.h and
      inttypes.h sized data types (e.g. int64_t and uint32_t).


    Version 1.45
    Jan C. Depner (PFM Software)
    02/13/15

    - To give better feedback to shelling programs in the case of errors I've added the program name to all
      output to stderr.


    Version 1.46
    Jan C. Depner (PFM Software)
    08/08/16

    - Now gets font from globalABE.ini as set in Preferences by pfmView.
    - To avoid possible confusion, removed translation setup from QSettings in env_in_out.cpp.


    Version 1.47
    Jan C. Depner (PFM Software)
    08/20/16

    - Added support for Binary Feature Data (BFD) 3.0 feature type field.


    Version 1.48
    Jan C. Depner (PFM Software)
    10/23/17

    - A bunch of changes to support doing translations in the future.  There is a generic
      mosaicView_xx.ts file that can be run through Qt's "linguist" to translate to another language.

*/
