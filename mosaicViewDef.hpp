
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



#ifndef _MOSAICVIEW_DEF_H_
#define _MOSAICVIEW_DEF_H_


#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>
#include <cerrno>
#include <cmath>


#include "gdal_priv.h"


#include "nvutility.h"
#include "nvutility.hpp"

#include "pfm.h"

#include "binaryFeatureData.h"

#include <QtCore>
#include <QtGui>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif
#include <QSharedMemory>



using namespace std;  // Windoze bullshit - God forbid they should follow a standard



//  Pointer interaction functions.

#define         ZOOM_IN_AREA                1
#define         ADD_FEATURE                 2
#define         DELETE_FEATURE              3
#define         EDIT_FEATURE                4
#define         DEFINE_FEATURE_POLY_AREA    5


#define         NUM_OVERLAYS                10
#define         LINE_WIDTH                  2
#define         POLYGON_POINTS              NVMAP_POLYGON_POINTS


//  The OPTIONS structure contains all those variables that can be saved to the
//  users mosaicView QSettings.


typedef struct
{
  uint8_t     coast;                      //  Coastline flag
  int32_t     position_form;              //  Position format number
  QColor      marker_color;               //  Color to be used for the marker
  QColor      coast_color;                //  Color to be used for coast
  QColor      feature_color;              //  Color to be used for features
  QColor      feature_info_color;         //  Color to be used for feature information text display
  QColor      feature_poly_color;         //  Color to be used for feature polygon display
  QColor      rect_color;                 //  Color to be used for rectangles
  QColor      highlight_color;            //  Color to be used for highlighted objects
  QFont       font;                       //  Font used for all ABE map GUI applications
  uint8_t     display_feature;            //  Flag to show feature location
  uint8_t     display_children;           //  Flag to show grouped feature children locations
  uint8_t     display_feature_info;       //  Flag to show feature info
  uint8_t     display_feature_poly;       //  Flag to show feature polygonal area if it exists
  float       feature_diameter;           //  Add feature circle diameter (meters)
  int32_t     last_rock_feature_desc;     //  Index of last used rock, wreck, obstruction feature description
  int32_t     last_offshore_feature_desc; //  Index of last used offshore installation feature description
  int32_t     last_light_feature_desc;    //  Index of last used light, buoy, beacon feature description
  int32_t     last_lidar_feature_desc;    //  Index of last used lidar feature description
  QString     last_feature_description;   //  String containing the last feature descriptor used
  QString     last_feature_remarks;       //  String containing the last feature remarks used
  int32_t     zoom_percent;               //  Zoom in/out percentage
  int32_t     width;                      //  Main window width
  int32_t     height;                     //  Main window height
  int32_t     window_x;                   //  Main window x position
  int32_t     window_y;                   //  Main window y position
  uint8_t     startup_message;            //  Whether to display the startup message
  int32_t     new_feature;                //  Stupid counter so I can advertise new features once and then move on
  int32_t     screenshot_delay;           //  Delay (in seconds) of the screenshot in the updateFeature dialog.
} OPTIONS;


//  General stuff.

typedef struct
{
  int32_t     function;                   //  Active edit or zoom function
  int32_t     save_function;              //  Save last edit function
  int32_t     poly_count;                 //  Polygon point count
  double      polygon_x[POLYGON_POINTS];  //  Polygon X
  double      polygon_y[POLYGON_POINTS];  //  Polygon Y
  NV_F64_XYMBR displayed_area;            //  displayed area
  uint8_t     drawing;                    //  set if we are drawing the surface
  BFDATA_SHORT_FEATURE *feature;          //  Feature (target) data array
  int32_t     bfd_handle;                 //  BFD (target) file handle
  BFDATA_HEADER  bfd_header;              //  Header for the current BFD file
  BFDATA_RECORD  new_record;              //  Record built when adding a new feature
  uint8_t     bfd_open;                   //  Set if we have an open bfd file
  int32_t     nearest_feature;
  QString     feature_search_string;      //  Text to search for when highlighting features
  int32_t     feature_polygon_flag;
  double      lat_step;
  double      lon_step;
  NV_F64_XYMBR geotiff_area;              //  Bounds of the GeoTIFF file
  int32_t     mosaic_width;
  int32_t     mosaic_height;
  QImage      *full_res_image;
  QSharedMemory *abeShare;                //  ABE's shared memory pointer.
  ABE_SHARE   *abe_share;                 //  Pointer to the ABE_SHARE structure in shared memory.
  char        progname[256];              /*  This is the program name.  It will be used in all output to stderr so that shelling programs
                                              will know what program printed out the error message.  */
} MISC;


#endif
