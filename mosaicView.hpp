
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



/*  mosaicView class definitions.  */

#ifndef MOSAICVIEW_H
#define MOSAICVIEW_H


#include "mosaicViewDef.hpp"
#include "version.hpp"
#include "prefs.hpp"
#include "editFeature.hpp"



class mosaicView:public QMainWindow
{
  Q_OBJECT 


public:

  mosaicView (int32_t argc = 0, char **argv = 0, QWidget *parent = 0);
  ~mosaicView ();

  void redrawMap ();
  void editFeatureNum (int32_t feature_number);


protected:

  OPTIONS         options;

  MISC            misc;

  ABE_SHARE       abe_share;

  prefs           *prefs_dialog;

  editFeature     *editFeature_dialog;

  long            zone;

  uint8_t         double_click, need_redraw, popup_active;

  QPainterPath    marker;

  QTimer          *trackCursor;

  QCheckBox       *sMessage;

  QMouseEvent     *menu_mouse_event;

  nvMap           *map;

  int32_t         feature_circle_pixels, menu_cursor_x, menu_cursor_y, redraw_count;

  QCursor         zoomCursor, editFeatureCursor, deleteFeatureCursor, addFeatureCursor, pencilCursor;

  uint32_t        ac[3];

  NVMAP_DEF       mapdef;

  QMenu           *popupMenu;

  QAction         *bHelp, *popup0, *popup1, *popup2, *popup3, *popup4;

  QToolButton     *bQuit, *bZoomOut, *bZoomIn, *bCoast, *bPrefs, *bDisplayFeature, *bDisplayChildren, *bDisplayFeatureInfo, 
                  *bDisplayFeaturePoly, *bAddFeature, *bDeleteFeature, *bEditFeature, *bLink, *bUnlink, *bOpen;

  int32_t         mv_marker, mv_tracker, mv_circle, rb_rectangle, rb_polygon;

  int32_t         active_window_id;

  uint8_t         shelled;

  double          prev_poly_lat, prev_poly_lon, menu_cursor_lon, menu_cursor_lat;

  char            mosaic_file[512], pf[10];

  int32_t         new_feature;


  void setMainButtons (uint8_t enabled);
  void discardMovableObjects ();
  void setFunctionCursor (int32_t function);


protected slots:

  void slotMouseDoubleClick (QMouseEvent *e, double lon, double lat);
  void slotMousePress (QMouseEvent *e, double lon, double lat);
  void slotPreliminaryMousePress (QMouseEvent *e);
  void slotMouseRelease (QMouseEvent *e, double lon, double lat);
  void slotMouseMove (QMouseEvent *e, double lon, double lat);
  void slotResize (QResizeEvent *e);
  void slotPreRedraw (NVMAP_DEF mapdef);
  void slotPostRedraw (NVMAP_DEF mapdef);

  void slotTrackCursor ();

  void midMouse (int32_t mouse_x, int32_t mouse_y, double lon, double lat);
  void leftMouse (int32_t mouse_x, int32_t mouse_y, double lon, double lat);
  void rightMouse (int32_t mouse_x, int32_t mouse_y, double lon, double lat);

  void slotPopupMenu0 ();
  void slotPopupMenu1 ();
  void slotPopupMenu2 ();
  void slotPopupMenu3 ();
  void slotPopupHelp ();

  void slotEditMode (int id);

  void slotZoomOut ();

  void slotDisplayFeature ();
  void slotDisplayChildren ();
  void slotDisplayFeatureInfo ();
  void slotDisplayFeaturePoly ();
  void slotEditFeatureDataChanged ();
  void slotEditFeatureDefinePolygon ();

  void slotCoast ();

  void slotPrefs ();
  void slotPrefDataChanged ();

  void slotQuit ();

  void about ();
  void slotAcknowledgments ();
  void aboutQt ();


 private:

  void keyPressEvent (QKeyEvent *e);


};

#endif
