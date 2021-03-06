
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



#ifndef EDITFEATURE_H
#define EDITFEATURE_H

#include "pfm.h"
#include "pfm_extras.h"
#include "mosaicViewDef.hpp"
#include "screenshot.hpp"


class editFeature:public QDialog
{
  Q_OBJECT 


public:

  editFeature (QWidget *pt = 0, OPTIONS *op = NULL, MISC *mi = NULL, int32_t num = 0);
  ~editFeature ();


  signals:

  void dataChangedSignal ();
  void definePolygonSignal ();


protected:

  OPTIONS         *options;

  MISC            *misc;

  QWidget         *parent;

  BFDATA_RECORD   bfd_record;

  BFDATA_POLYGON  bfd_polygon;

  char            image_name[512];

  int32_t         current_feature, window_x, window_y, window_width, window_height;

  uint8_t         new_feature, assigned_image;

  QDoubleSpinBox  *orientation, *length, *width;

  QSpinBox        *confidence;

  QLineEdit       *remarks, *description;

  QListWidget     *rockBox, *offshoreBox, *lightBox, *lidarBox;

  QDialog         *editFeatureD, *rockD, *offshoreD, *lightD, *lidarD;

  QPushButton     *bView, *bAssign, *bScreen, *bPolygon, *bDelete, *bGroup, *bUngroup;

  QPixmap         helpIcon;

  QTimer          *polyTimer;

  QProcess        *imageProc;

  screenshot      *screen_dialog;


protected slots:

  void slotHelp ();

  void slotSnippetViewerReadyReadStandardError ();
  void slotViewError (QProcess::ProcessError error);
  void slotViewSnippet ();
  void slotAssignSnippet ();
  void slotScreenshotSaved (QString filename);
  void slotScreenshotQuit ();
  void slotScreen ();
  void slotHelpEditFeature ();
  void slotApplyEditFeature ();
  void slotCancelEditFeature ();

  void slotPolygon ();
  void slotPolyTimer ();

  void slotDelete ();
  void slotGroup ();
  void slotUngroup ();

  void slotRocks ();
  void slotRockOKClicked ();
  void slotRockCancelClicked ();

  void slotOffshore ();
  void slotOffshoreOKClicked ();
  void slotOffshoreCancelClicked ();

  void slotLights ();
  void slotLightOKClicked ();
  void slotLightCancelClicked ();

  void slotLidar ();
  void slotLidarOKClicked ();
  void slotLidarCancelClicked ();


 private:
};

#endif
