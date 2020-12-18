
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



#include "mosaicView.hpp"
#include "mosaicViewHelp.hpp"
#include "acknowledgments.hpp"


void overlayFlag (nvMap *map, OPTIONS *options, MISC *misc);
char *get_geotiff (char *mosaic_file, MISC *misc);


/***************************************************************************\

    Module :        mosaicView

    Programmer :    Jan C. Depner

    Date :          10/05/07

    Purpose :       This is a special purpose program that is shelled from
                    pfmView in order to view GeoTIFF files.  It was originally 
                    designed to work with CHARTS photomosaics but will work
                    with any geographic GeoTIFF file.  In addition to viewing
                    GeoTIFFs it also allows you to view and edit Binary Feature 
                    Data (BFD) files.  

    Arguments :     argc               -  command line argument count
                    argv               -  command line arguments
                    parent             -  QWidget parent

    Caveats :       This program is never run from the command line.  If you
                    want to view multiple GeoTIFFs and other files it is best
                    to use the areaCheck program.

\***************************************************************************/


mosaicView::mosaicView (int32_t argc, char **argv, QWidget * parent):
  QMainWindow (parent, 0)
{
  extern char *optarg;
  extern int optind;


  void envin (OPTIONS *options, QMainWindow *mainWindow);
  void set_defaults (MISC *misc, OPTIONS *options, uint8_t restore);


  strcpy (misc.progname, argv[0]);
  prefs_dialog = NULL;
  prev_poly_lat = -91.0;
  prev_poly_lon = -181.0;
  popup_active = NVFalse;
  double_click = NVFalse;
  misc.full_res_image = NULL;
  misc.drawing = NVTrue;
  rb_rectangle = -1;
  rb_polygon = -1;
  mv_marker = -1;
  mv_tracker = -1;
  mv_circle = -1;
  active_window_id = getpid ();
  need_redraw = NVFalse;
  redraw_count = 0;


  QResource::registerResource ("/icons.rcc");


  //  Have to set the focus policy or keypress events don't work properly at first in Focus Follows Mouse mode

  setFocusPolicy (Qt::WheelFocus);


  //  Make the "marker" cursor painter path.

  marker = QPainterPath ();

  marker.moveTo (0, 0);
  marker.lineTo (30, 0);
  marker.lineTo (30, 20);
  marker.lineTo (0, 20);
  marker.lineTo (0, 0);

  marker.moveTo (0, 10);
  marker.lineTo (4, 10);

  marker.moveTo (30, 10);
  marker.lineTo (26, 10);

  marker.moveTo (15, 0);
  marker.lineTo (15, 3);

  marker.moveTo (15, 20);
  marker.lineTo (15, 17);


  //  Set the main icon

  setWindowIcon (QIcon (":/icons/mosaicView.png"));


  zoomCursor = QCursor (QPixmap (":/icons/zoom_cursor.png"), 11, 11);
  editFeatureCursor = QCursor (QPixmap (":/icons/edit_feature_cursor.png"), 15, 15);
  deleteFeatureCursor = QCursor (QPixmap (":/icons/delete_feature_cursor.png"), 15, 15);
  addFeatureCursor = QCursor (QPixmap (":/icons/add_feature_cursor.png"), 15, 15);
  pencilCursor = QCursor (QPixmap (":/icons/pencil_cursor.png"), 10, 26);


  int32_t option_index = 0;
  int32_t key = 0;
  while (NVTrue) 
    {
      static struct option long_options[] = {{"actionkey00", required_argument, 0, 0},
                                             {"actionkey01", required_argument, 0, 0},
                                             {"actionkey02", required_argument, 0, 0},
                                             {"shared_memory_key", required_argument, 0, 0},
                                             {0, no_argument, 0, 0}};

      char c = (char) getopt_long (argc, argv, "o", long_options, &option_index);
      if (c == -1) break;

      switch (c) 
        {
        case 0:

          char tmp;

          switch (option_index)
            {
            case 0:
	      sscanf (optarg, "%1c", &tmp);
	      ac[option_index] = (uint32_t) tmp;
              break;

            case 1:
	      sscanf (optarg, "%1c", &tmp);
	      ac[option_index] = (uint32_t) tmp;
              break;

            case 2:
	      sscanf (optarg, "%1c", &tmp);
	      ac[option_index] = (uint32_t) tmp;
              break;

            case 3:
              sscanf (optarg, "%d", &key);
              break;
            }
          break;
        }
    }


  /* Make sure we got the mandatory file name argument.  This is put last on the argument list after parsing
     the options and is pointed to by optind.  If optind is not equal to argc - 1 then we either got
     no filename argument or too many non-option arguments.  */

  if (!key || optind != (argc - 1))
    {
      QMessageBox msgBox;
      msgBox.setText (tr ("Usage: mosaicView GEOTIFF_FILE --shared_memory_id=SHARED_MEMORY_ID"));
      QPushButton *exitButton = msgBox.addButton (tr ("Exit"), QMessageBox::ActionRole);

      msgBox.exec ();


      //  Dummy line.  No matter what you do we're exiting.

      if (msgBox.clickedButton () == exitButton) {};

      exit (-1);
    }


  strcpy (mosaic_file, argv[optind]);


  this->setWindowTitle (QString ("mosaicView : ") + QString (mosaic_file));


  char retval[512];
  strcpy (retval, get_geotiff (mosaic_file, &misc));


  if (strcmp (retval, "Success"))
    {
      QMessageBox msgBox;
      msgBox.setText (QString (retval));
      QPushButton *exitButton = msgBox.addButton (tr ("Exit"), QMessageBox::ActionRole);

      msgBox.exec ();


      //  Dummy line.  No matter what you do we're exiting.

      if (msgBox.clickedButton () == exitButton) {};

      exit (-1);
    }


  //  This is the "tools" toolbar.  We have to do this here so that we can restore the toolbar location(s).

  QToolBar *tools = addToolBar (tr ("Tools"));
  tools->setObjectName (tr ("mosaicView main toolbar"));


  set_defaults (&misc, &options, NVFalse);


  envin (&options, this);


  // Set the application font

  QApplication::setFont (options.font);


  //  Set the window size and location from the defaults

  this->resize (options.width, options.height);
  this->move (options.window_x, options.window_y);


  statusBar ()->setSizeGripEnabled (false);
  statusBar ()->showMessage (VERSION);


  QFrame *frame = new QFrame (this, 0);

  setCentralWidget (frame);


  mapdef.projection = CYLINDRICAL_EQUIDISTANT;
  mapdef.draw_width = 800;
  mapdef.draw_height = 600;
  mapdef.frame = NVTrue;

  mapdef.overlap_percent = 5;
  mapdef.grid_inc_x = 0.0;
  mapdef.grid_inc_y = 0.0;
  mapdef.grid_thickness = 2;
  mapdef.zoom_percent = options.zoom_percent;

  mapdef.coasts = options.coast;
  mapdef.coast_thickness = 2;
  mapdef.landmask = NVFalse;

  mapdef.border = 5;
  mapdef.coast_color = options.coast_color;
  mapdef.grid_color = Qt::blue;
  mapdef.background_color = Qt::black;

  mapdef.initial_bounds.min_x = misc.geotiff_area.min_x;
  mapdef.initial_bounds.min_y = misc.geotiff_area.min_y;
  mapdef.initial_bounds.max_x = misc.geotiff_area.max_x;
  mapdef.initial_bounds.max_y = misc.geotiff_area.max_y;


  //  Make the map.

  map = new nvMap (this, &mapdef);
  map->setWhatsThis (mapText);


  //  Connect to the signals from the map class.

  connect (map, SIGNAL (mousePressSignal (QMouseEvent *, double, double)), this, 
           SLOT (slotMousePress (QMouseEvent *, double, double)));
  connect (map, SIGNAL (mouseReleaseSignal (QMouseEvent *, double, double)), this,
           SLOT (slotMouseRelease (QMouseEvent *, double, double)));
  connect (map, SIGNAL (mouseDoubleClickSignal (QMouseEvent *, double, double)), this,
           SLOT (slotMouseDoubleClick (QMouseEvent *, double, double)));
  connect (map, SIGNAL (mouseMoveSignal (QMouseEvent *, double, double)), this,
           SLOT (slotMouseMove (QMouseEvent *, double, double)));
  connect (map, SIGNAL (resizeSignal (QResizeEvent *)), this, SLOT (slotResize (QResizeEvent *)));
  connect (map, SIGNAL (preRedrawSignal (NVMAP_DEF)), this, SLOT (slotPreRedraw (NVMAP_DEF)));
  connect (map, SIGNAL (postRedrawSignal (NVMAP_DEF)), this, SLOT (slotPostRedraw (NVMAP_DEF)));


  /******************************************* IMPORTANT NOTE ABOUT SHARED MEMORY **************************************** \

      This is a little note about the use of shared memory within the Area-Based Editor (ABE) programs.  If you read
      the Qt documentation (or anyone else's documentation) about the use of shared memory they will say "Dear [insert
      name of omnipotent being of your choice here], whatever you do, always lock shared memory when you use it!".
      The reason they say this is that access to shared memory is not atomic.  That is, reading shared memory and then
      writing to it is not a single operation.  An example of why this might be important - two programs are running,
      the first checks a value in shared memory, sees that it is a zero.  The second program checks the same location
      and sees that it is a zero.  These two programs have different actions they must perform depending on the value
      of that particular location in shared memory.  Now the first program writes a one to that location which was
      supposed to tell the second program to do something but the second program thinks it's a zero.  The second program
      doesn't do what it's supposed to do and it writes a two to that location.  The two will tell the first program 
      to do something.  Obviously this could be a problem.  In real life, this almost never occurs.  Also, if you write
      your program properly you can make sure this doesn't happen.  In ABE we almost never lock shared memory because
      something much worse than two programs getting out of sync can occur.  If we start a program and it locks shared
      memory and then dies, all the other programs will be locked up.  When you look through the ABE code you'll see
      that we very rarely lock shared memory, and then only for very short periods of time.  This is by design.

  \******************************************* IMPORTANT NOTE ABOUT SHARED MEMORY ****************************************/


  //  Get the ABE shared memory area.  The key is the master process ID plus "_abe".

  QString skey;
  skey.sprintf ("%d_abe", key);

  misc.abeShare = new QSharedMemory (skey);

  if (!misc.abeShare->attach (QSharedMemory::ReadWrite))
    {
      QMessageBox::warning (this, "mosaicView", tr ("Unable to attach to ABE shared memory segment!"));

      exit (-1);
    }

  misc.abe_share = (ABE_SHARE *) misc.abeShare->data ();


  //  Layouts, what fun!

  QVBoxLayout *vBox = new QVBoxLayout (frame);


  vBox->addWidget (map);


  //  Button, button, who's got the buttons?

  bQuit = new QToolButton (this);
  bQuit->setIcon (QIcon (":/icons/quit.png"));
  bQuit->setToolTip (tr ("Quit"));
  bQuit->setWhatsThis (quitText);
  connect (bQuit, SIGNAL (clicked ()), this, SLOT (slotQuit ()));
  tools->addWidget (bQuit);


  tools->addSeparator ();
  tools->addSeparator ();


  QButtonGroup *editModeGrp = new QButtonGroup (this);
  connect (editModeGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotEditMode (int)));
  editModeGrp->setExclusive (true);


  bZoomIn = new QToolButton (this);
  bZoomIn->setIcon (QIcon (":/icons/icon_zoomin.png"));
  bZoomIn->setCheckable (true);
  bZoomIn->setToolTip (tr ("Zoom in"));
  bZoomIn->setWhatsThis (zoomInText);
  editModeGrp->addButton (bZoomIn, ZOOM_IN_AREA);
  tools->addWidget (bZoomIn);

  bZoomOut = new QToolButton (this);
  bZoomOut->setIcon (QIcon (":/icons/icon_zoomout.png"));
  bZoomOut->setToolTip (tr ("Zoom out"));
  bZoomOut->setWhatsThis (zoomOutText);
  connect (bZoomOut, SIGNAL (clicked ()), this, SLOT (slotZoomOut ()));
  tools->addWidget (bZoomOut);


  tools->addSeparator ();
  tools->addSeparator ();


  bAddFeature = new QToolButton (this);
  bAddFeature->setIcon (QIcon (":/icons/addfeature.png"));
  bAddFeature->setToolTip (tr ("Add a feature"));
  bAddFeature->setWhatsThis (addFeatureText);
  editModeGrp->addButton (bAddFeature, ADD_FEATURE);
  bAddFeature->setCheckable (true);
  bAddFeature->setEnabled (options.display_feature);
  tools->addWidget (bAddFeature);


  bDeleteFeature = new QToolButton (this);
  bDeleteFeature->setIcon (QIcon (":/icons/deletefeature.png"));
  bDeleteFeature->setToolTip (tr ("Delete a feature"));
  bDeleteFeature->setWhatsThis (deleteFeatureText);
  editModeGrp->addButton (bDeleteFeature, DELETE_FEATURE);
  bDeleteFeature->setCheckable (true);
  bDeleteFeature->setEnabled (options.display_feature);
  tools->addWidget (bDeleteFeature);


  bEditFeature = new QToolButton (this);
  bEditFeature->setIcon (QIcon (":/icons/editfeature.png"));
  bEditFeature->setToolTip (tr ("Edit a feature"));
  bEditFeature->setWhatsThis (editFeatureText);
  editModeGrp->addButton (bEditFeature, EDIT_FEATURE);
  bEditFeature->setCheckable (true);
  bEditFeature->setEnabled (options.display_feature);
  tools->addWidget (bEditFeature);


  tools->addSeparator ();
  tools->addSeparator ();


  bDisplayFeature = new QToolButton (this);
  bDisplayFeature->setIcon (QIcon (":/icons/displayfeature.png"));
  bDisplayFeature->setToolTip (tr ("Flag feature data"));
  bDisplayFeature->setWhatsThis (displayFeatureText);
  bDisplayFeature->setCheckable (true);
  bDisplayFeature->setChecked (options.display_feature);
  connect (bDisplayFeature, SIGNAL (clicked ()), this, SLOT (slotDisplayFeature ()));
  tools->addWidget (bDisplayFeature);


  bDisplayChildren = new QToolButton (this);
  bDisplayChildren->setIcon (QIcon (":/icons/displayfeaturechildren.png"));
  bDisplayChildren->setToolTip (tr ("Display feature group members"));
  bDisplayChildren->setWhatsThis (displayChildrenText);
  bDisplayChildren->setCheckable (true);
  bDisplayChildren->setChecked (options.display_children);
  connect (bDisplayChildren, SIGNAL (clicked ()), this, SLOT (slotDisplayChildren ()));
  tools->addWidget (bDisplayChildren);


  bDisplayFeatureInfo = new QToolButton (this);
  bDisplayFeatureInfo->setIcon (QIcon (":/icons/displayfeatureinfo.png"));
  bDisplayFeatureInfo->setToolTip (tr ("Display feature information"));
  bDisplayFeatureInfo->setWhatsThis (displayFeatureInfoText);
  bDisplayFeatureInfo->setCheckable (true);
  bDisplayFeatureInfo->setChecked (options.display_feature_info);
  connect (bDisplayFeatureInfo, SIGNAL (clicked ()), this, SLOT (slotDisplayFeatureInfo ()));
  tools->addWidget (bDisplayFeatureInfo);


  bDisplayFeaturePoly = new QToolButton (this);
  bDisplayFeaturePoly->setIcon (QIcon (":/icons/displayfeaturepoly.png"));
  bDisplayFeaturePoly->setToolTip (tr ("Display feature polygonal area"));
  bDisplayFeaturePoly->setWhatsThis (displayFeaturePolyText);
  bDisplayFeaturePoly->setCheckable (true);
  bDisplayFeaturePoly->setChecked (options.display_feature_poly);
  connect (bDisplayFeaturePoly, SIGNAL (clicked ()), this, SLOT (slotDisplayFeaturePoly ()));
  tools->addWidget (bDisplayFeaturePoly);


  tools->addSeparator ();
  tools->addSeparator ();


  bCoast = new QToolButton (this);
  bCoast->setIcon (QIcon (":/icons/coast.png"));
  bCoast->setToolTip (tr ("Toggle coastline display"));
  bCoast->setWhatsThis (coastText);
  bCoast->setCheckable (true);
  bCoast->setChecked (options.coast);
  connect (bCoast, SIGNAL (clicked ()), this, SLOT (slotCoast ()));
  tools->addWidget (bCoast);


  tools->addSeparator ();
  tools->addSeparator ();


  bPrefs = new QToolButton (this);
  bPrefs->setIcon (QIcon (":/icons/prefs.png"));
  bPrefs->setToolTip (tr ("Change application preferences"));
  bPrefs->setWhatsThis (prefsText);
  connect (bPrefs, SIGNAL (clicked ()), this, SLOT (slotPrefs ()));
  tools->addWidget (bPrefs);


  tools->addSeparator ();
  tools->addSeparator ();


  QAction *bHelp = QWhatsThis::createAction (this);
  bHelp->setIcon (QIcon (":/icons/contextHelp.png"));
  tools->addAction (bHelp);


  //  Right click popup menu

  popupMenu = new QMenu (map);

  popup0 = popupMenu->addAction ("");
  connect (popup0, SIGNAL (triggered ()), this, SLOT (slotPopupMenu0 ()));
  popup1 = popupMenu->addAction ("");
  connect (popup1, SIGNAL (triggered ()), this, SLOT (slotPopupMenu1 ()));
  popup2 = popupMenu->addAction ("");
  connect (popup2, SIGNAL (triggered ()), this, SLOT (slotPopupMenu2 ()));
  popup3 = popupMenu->addAction ("");
  connect (popup3, SIGNAL (triggered ()), this, SLOT (slotPopupMenu3 ()));
  popupMenu->addSeparator ();
  popup4 = popupMenu->addAction (tr ("Help"));
  connect (popup4, SIGNAL (triggered ()), this, SLOT (slotPopupHelp ()));
  popupMenu->show ();


  //  Setup the file menu.

  QAction *fileQuitAction = new QAction (tr ("&Quit"), this);
  fileQuitAction->setShortcut (tr ("Ctrl+Q"));
  fileQuitAction->setStatusTip (tr ("Exit from application"));
  connect (fileQuitAction, SIGNAL (triggered ()), qApp, SLOT (closeAllWindows ()));

  QMenu *fileMenu = menuBar ()->addMenu (tr ("&File"));
  fileMenu->addAction (fileQuitAction);


  //  Setup the help menu.

  QAction *aboutAct = new QAction (tr ("&About"), this);
  aboutAct->setShortcut (tr ("Ctrl+A"));
  aboutAct->setStatusTip (tr ("Information about mosaicView"));
  connect (aboutAct, SIGNAL (triggered ()), this, SLOT (about ()));

  QAction *acknowledgments = new QAction (tr ("A&cknowledgments"), this);
  acknowledgments->setShortcut (tr ("Ctrl+c"));
  acknowledgments->setStatusTip (tr ("Information about supporting libraries"));
  connect (acknowledgments, SIGNAL (triggered ()), this, SLOT (slotAcknowledgments ()));

  QAction *aboutQtAct = new QAction (tr ("About Qt"), this);
  aboutQtAct->setStatusTip (tr ("Information about Qt"));
  connect (aboutQtAct, SIGNAL (triggered ()), this, SLOT (aboutQt ()));

  QMenu *helpMenu = menuBar ()->addMenu (tr ("&Help"));
  helpMenu->addAction (aboutAct);
  helpMenu->addSeparator ();
  helpMenu->addAction (acknowledgments);
  helpMenu->addAction (aboutQtAct);


  misc.function = ZOOM_IN_AREA;
  bZoomIn->setChecked (true);
  setFunctionCursor (misc.function);
  setMainButtons (NVFalse);


  //  If we have a new feature, add it here.

  new_feature = 0;
  if (options.new_feature < new_feature)
    {
      QMessageBox::information (this, tr ("mosaicView New feature"), 
                                tr ("<br><br><b><i><center>New Feature</center></i></b><br><br>"
                                    "Blah, blah, blah, yackity smackity."));
      options.new_feature = new_feature;
    }


  //  Display the startup info message the first time through.

  if (options.startup_message)
    {
      QString startupMessageText = 
        tr ("The following action keys are available in pfmView and/or pfmEdit:\n\n"
            "%1 = Zoom mosaic to the displayed area in pfmView/pfmEdit\n"
            "%2 = Zoom the mosaic view out by 10%\n"
            "%3 = Zoom the mosaic view in by 10%\n\n"
            "You can change these key values in the pfmView/pfmEdit\n"
            "Preferences->Ancillary Programs window.\n\n\n"
            "mosaicView Preferences window.").arg (QString (ac[0])).arg (QString (ac[1])).arg (QString (ac[2]));

      QMessageBox::information (this, tr ("mosaicView Startup Message"), startupMessageText);
    }


  //  Try to open the feature file and read the features into memory.

  if (strcmp (misc.abe_share->open_args[0].target_path, "NONE"))
    {
      if (misc.bfd_open) binaryFeatureData_close_file (misc.bfd_handle);
      misc.bfd_open = NVFalse;

      if ((misc.bfd_handle = binaryFeatureData_open_file (misc.abe_share->open_args[0].target_path, &misc.bfd_header, BFDATA_UPDATE)) >= 0)
        {
          if (binaryFeatureData_read_all_short_features (misc.bfd_handle, &misc.feature) < 0)
            {
              QString msg = QString (binaryFeatureData_strerror ());
              QMessageBox::warning (this, "mosaicView", tr ("Unable to read feature records\nReason: %1").arg (msg));
              binaryFeatureData_close_file (misc.bfd_handle);
            }
          else
            {
              misc.bfd_open = NVTrue;
            }
        }
    }


  //  Set the tracking timer to every 50 milliseconds.

  trackCursor = new QTimer (this);
  connect (trackCursor, SIGNAL (timeout ()), this, SLOT (slotTrackCursor ()));
  trackCursor->start (50);


  map->setCursor (Qt::ArrowCursor);

  map->enableSignals ();
}



mosaicView::~mosaicView ()
{
}



void 
mosaicView::setMainButtons (uint8_t enabled)
{
  bCoast->setEnabled (enabled);


  NVMAP_DEF l_mapdef = map->getMapdef ();
  if (enabled && l_mapdef.zoom_level > 0)
    {
      bZoomOut->setEnabled (enabled);
    }
  else
    {
      bZoomOut->setEnabled (false);
    }

  bZoomIn->setEnabled (enabled);
  bPrefs->setEnabled (enabled);
  bDisplayFeature->setEnabled (enabled);


  //  No point in having a feature info button if we're not displaying features

  if (enabled && options.display_feature)
    {
      bDisplayChildren->setEnabled (enabled);
      bDisplayFeatureInfo->setEnabled (enabled);
      bDisplayFeaturePoly->setEnabled (enabled);
      bAddFeature->setEnabled (enabled);
      bDeleteFeature->setEnabled (enabled);
      bEditFeature->setEnabled (enabled);
    }
  else
    {
      bDisplayChildren->setEnabled (false);
      bDisplayFeatureInfo->setEnabled (false);
      bDisplayFeaturePoly->setEnabled (false);
      bAddFeature->setEnabled (false);
      bDeleteFeature->setEnabled (false);
      bEditFeature->setEnabled (false);
    }


  switch (misc.function)
    {
    case ADD_FEATURE:
      bAddFeature->setChecked (true);
      break;

    case EDIT_FEATURE:
      bEditFeature->setChecked (true);
      break;

    case DELETE_FEATURE:
      bDeleteFeature->setChecked (true);
      break;
    }
}



void
mosaicView::slotCoast ()
{
  if (bCoast->isChecked ())
    {
      options.coast = NVTrue;
      map->setCoasts (NVMAP_AUTO_COAST);
    }
  else
    {
      options.coast = NVFalse;
      map->setCoasts (NVMAP_NO_COAST);
    }

  redrawMap ();
}



void 
mosaicView::slotPrefs ()
{
  if (prefs_dialog) prefs_dialog->close ();
  prefs_dialog = new prefs (this, &options, &misc);

  connect (prefs_dialog, SIGNAL (dataChangedSignal ()), this, SLOT (slotPrefDataChanged ()));
}



//  Changed some of the preferences

void 
mosaicView::slotPrefDataChanged ()
{
  map->setCoastColor (options.coast_color);
  map->setMaskColor (options.coast_color);
  map->setZoomPercent (options.zoom_percent);

  bDisplayFeature->setChecked (options.display_feature);
  bDisplayChildren->setChecked (options.display_children);
  bDisplayFeatureInfo->setChecked (options.display_feature_info);
  bDisplayFeaturePoly->setChecked (options.display_feature_poly);


  //  Compute feature circle diameter in pixels

  double tmplat, tmplon, dz;
  int32_t x[2], y[2], z[2];
  newgp (misc.abe_share->open_args[0].head.mbr.min_y, misc.abe_share->open_args[0].head.mbr.min_x, 0.0, options.feature_diameter,
         &tmplat, &tmplon);
  map->map_to_screen (1, &misc.abe_share->open_args[0].head.mbr.min_x, &misc.abe_share->open_args[0].head.mbr.min_y, &dz, &x[0], &y[0], 
                      &z[0]);
  map->map_to_screen (1, &tmplon, &tmplat, &dz, &x[1], &y[1], &z[1]);
  feature_circle_pixels = abs (y[1] - y[0]);
  if (feature_circle_pixels < 10) feature_circle_pixels = 10;


  redrawMap ();
}



void 
mosaicView::discardMovableObjects ()
{
  map->closeMovingPath (&mv_circle);
  map->closeMovingPath (&mv_marker);
  map->closeMovingPath (&mv_tracker);
  map->discardRubberbandRectangle (&rb_rectangle);
  map->discardRubberbandPolygon (&rb_polygon);
}



void 
mosaicView::leftMouse (int32_t mouse_x __attribute__ ((unused)), int32_t mouse_y __attribute__ ((unused)),
                       double lon, double lat)
{
  //  If the popup menu is up discard this mouse press

  if (popup_active)
    {
      popup_active = NVFalse;
      return;
    }


  int32_t *px, *py;
  double *mx, *my;
  NV_F64_XYMBR bounds;
  QString file, string;
  static QDir dir = QDir (".");
  QDateTime current_time = QDateTime::currentDateTime ();
  int32_t year = current_time.date ().year ();
  int32_t day = current_time.date ().dayOfYear ();
  int32_t hour = current_time.time ().hour ();
  int32_t minute = current_time.time ().minute ();
  float second = current_time.time ().second ();



  //  Actions based on the active function

  switch (misc.function)
    {
    case ZOOM_IN_AREA:
      if (map->rubberbandRectangleIsActive (rb_rectangle))
        {
          map->closeRubberbandRectangle (rb_rectangle, lon, lat, &px, &py, &mx, &my);

          bounds.min_x = 999999.0;
          bounds.min_y = 999999.0;
          bounds.max_x = -999999.0;
          bounds.max_y = -999999.0;
          for (int32_t i = 0 ; i < 4 ; i++)
            {
              if (mx[i] < bounds.min_x) bounds.min_x = mx[i];
              if (my[i] < bounds.min_y) bounds.min_y = my[i];
              if (mx[i] > bounds.max_x) bounds.max_x = mx[i];
              if (my[i] > bounds.max_y) bounds.max_y = my[i];
            }

	  discardMovableObjects ();
          map->zoomIn (bounds);
          bZoomOut->setEnabled (true);
        }
      else
        {
          map->anchorRubberbandRectangle (&rb_rectangle, lon, lat, options.rect_color, LINE_WIDTH, Qt::SolidLine);
        }
      break;


    case DEFINE_FEATURE_POLY_AREA:
      if (map->rubberbandPolygonIsActive (rb_polygon))
        {
          map->vertexRubberbandPolygon (rb_polygon, lon, lat);
        }
      else
        {
          map->anchorRubberbandPolygon (&rb_polygon, lon, lat, options.rect_color, LINE_WIDTH, NVTrue, Qt::SolidLine);
        }
      break;


    case EDIT_FEATURE:
      if (misc.nearest_feature != -1)
        {
          editFeatureNum (misc.nearest_feature);
        }
      break;


    case ADD_FEATURE:
      memset (&misc.new_record, 0, sizeof (BFDATA_RECORD));

      inv_cvtime (year - 1900, day, hour, minute, second, &misc.new_record.event_tv_sec, &misc.new_record.event_tv_nsec);

      misc.new_record.record_number = misc.bfd_header.number_of_records;
      misc.new_record.length = 0.0;
      misc.new_record.width = 0.0;
      misc.new_record.height = 0.0;
      misc.new_record.confidence_level = 3;
      misc.new_record.depth = 0.0;
      misc.new_record.horizontal_orientation = 0.0;
      misc.new_record.vertical_orientation = 0.0;
      strcpy (misc.new_record.description, "");
      strcpy (misc.new_record.remarks, "");
      misc.new_record.latitude = lat;
      misc.new_record.longitude = lon;
      strcpy (misc.new_record.analyst_activity, "NAVOCEANO BHY");
      misc.new_record.equip_type = 3;
      misc.new_record.nav_system = 1;
      misc.new_record.platform_type = 4;
      misc.new_record.sonar_type = 3;

      misc.new_record.poly_count = 0;

      editFeatureNum (-1);
      break;


    case DELETE_FEATURE:
      if (misc.nearest_feature != -1)
        {
          //  Delete feature at nearest_feature

          BFDATA_RECORD bfd_record;
          if (binaryFeatureData_read_record (misc.bfd_handle, misc.nearest_feature, &bfd_record) < 0)
            {
              QString msg = QString (binaryFeatureData_strerror ());
              QMessageBox::warning (this, tr ("Delete Feature"), tr ("Unable to read feature record\nReason: %1").arg (msg));
              break;
            }


          //  Zero out the confidence value

          bfd_record.confidence_level = misc.feature[misc.nearest_feature].confidence_level = 0;


          if (binaryFeatureData_write_record (misc.bfd_handle, misc.nearest_feature, &bfd_record, NULL, NULL) < 0)
            {
              QString msg = QString (binaryFeatureData_strerror ());
              QMessageBox::warning (this, tr ("Delete Feature"), tr ("Unable to update feature record\nReason: %1").arg (msg));
              break;
            }


          redrawMap ();


          //  If the mosaic viewer or 3D viewer was running, tell it to redraw.

          misc.abe_share->key = FEATURE_FILE_MODIFIED;

          redrawMap ();


          //  Let the shelling program know that we have modified the feature file.

          misc.abe_share->key = FEATURE_FILE_MODIFIED;
        }
      break;
    }
}



void 
mosaicView::midMouse (int32_t mouse_x __attribute__ ((unused)), int32_t mouse_y __attribute__ ((unused)),
                      double lon __attribute__ ((unused)), double lat __attribute__ ((unused)))
{
  //  Actions based on the active function

  switch (misc.function)
    {
    case ZOOM_IN_AREA:
      map->discardRubberbandRectangle (&rb_rectangle);
      break;

    case DEFINE_FEATURE_POLY_AREA:
      map->discardRubberbandPolygon (&rb_polygon);
      prev_poly_lon = -181.0;
      misc.feature_polygon_flag = -1;
      break;
    }

  if (misc.function != ADD_FEATURE && misc.function != EDIT_FEATURE && misc.function != DELETE_FEATURE)
    {
      misc.function = misc.save_function;
      setMainButtons (NVTrue);
      setFunctionCursor (misc.function);

      map->setToolTip ("");      
    }
}



void 
mosaicView::slotMouseDoubleClick (QMouseEvent *e __attribute__ ((unused)), double lon, double lat)
{
  int32_t count, *px, *py;
  double *mx, *my;
  QString file, string;
  QStringList filters;
  QStringList files;
  static QDir dir = QDir (".");


  //  Flip the double_click flag.  The right-click menu sets this to NVTrue so it will flip to NVFalse.
  //  Left-click sets it to NVFalse so it will filp to NVTrue;

  double_click = !double_click;


  //  Actions based on the active function

  switch (misc.function)
    {
    case DEFINE_FEATURE_POLY_AREA:
      map->closeRubberbandPolygon (rb_polygon, lon, lat, &count, &px, &py, &mx, &my);

      for (int32_t i = 0 ; i < count ; i++)
        {
          misc.polygon_x[i] = mx[i];
          misc.polygon_y[i] = my[i];
        }
      misc.poly_count = count;

      map->discardRubberbandPolygon (&rb_polygon);

      map->setToolTip ("");      
      misc.function = misc.save_function;

      setFunctionCursor (misc.function);

      misc.feature_polygon_flag = 1;
      break;
    }
}



void 
mosaicView::rightMouse (int32_t mouse_x, int32_t mouse_y, double lon, double lat)
{
  QString tmp;

  menu_cursor_lon = lon;
  menu_cursor_lat = lat;
  menu_cursor_x = mouse_x;
  menu_cursor_y = mouse_y;


  QPoint pos (mouse_x, mouse_y);


  //  Popups need global positioning

  QPoint global_pos = map->mapToGlobal (pos);


  switch (misc.function)
    {
    case ZOOM_IN_AREA:
      popup0->setText (tr ("Close rectangle and zoom in"));
      popup1->setText (tr ("Discard rectangle"));
      popup2->setVisible (false);
      popup3->setVisible (false);
      popup_active = NVTrue;
      popupMenu->popup (global_pos);
      break;

    case DEFINE_FEATURE_POLY_AREA:
      popup0->setText (tr ("Close polygon and save to feature file"));
      popup1->setText (tr ("Discard polygon"));
      popup2->setVisible (false);
      popup3->setVisible (false);
      popup_active = NVTrue;
      popupMenu->popup (global_pos);
      break;

    case DELETE_FEATURE:
      popup0->setText (tr ("Delete feature"));
      popup1->setText (tr ("Cancel"));
      popup2->setVisible (false);
      popup3->setVisible (false);
      popup_active = NVTrue;
      popupMenu->popup (global_pos);
      break;

    case EDIT_FEATURE:
      popup0->setText (tr ("Edit feature"));
      popup1->setText (tr ("Highlight features (text search)"));
      popup2->setText (tr ("Cancel"));
      popup2->setVisible (true);
      popup3->setVisible (false);
      popup_active = NVTrue;
      popupMenu->popup (global_pos);
      break;

    case ADD_FEATURE:
      popup0->setText (tr ("Add feature"));
      popup1->setText (tr ("Set feature circle radius"));
      popup2->setText (tr ("Cancel"));
      popup2->setVisible (true);
      popup3->setVisible (false);
      popup_active = NVTrue;
      popupMenu->popup (global_pos);
      break;
    }
}



void 
mosaicView::slotPopupMenu0 ()
{
  popup_active = NVFalse;

  switch (misc.function)
    {
    case ZOOM_IN_AREA:
    case DELETE_FEATURE:
    case ADD_FEATURE:
    case EDIT_FEATURE:
      leftMouse (menu_cursor_x, menu_cursor_y, menu_cursor_lon, menu_cursor_lat);
      break;

    case DEFINE_FEATURE_POLY_AREA:

      //  Fake out slotMouseDoubleClick so that it won't decrement the point count.

      double_click = NVTrue;

      slotMouseDoubleClick (NULL, menu_cursor_lon, menu_cursor_lat);
      break;
    }
}



void 
mosaicView::slotPopupMenu1 ()
{
  popup_active = NVFalse;

  if (misc.function == ADD_FEATURE)
    {
      bool ok;
      double res = QInputDialog::getDouble (this, "mosaicView", tr ("Enter a new feature circle diameter:"), options.feature_diameter, 10.0, 1000.0, 2, &ok);
      if (ok)
        {
           options.feature_diameter = (float) res;


           //  Compute feature circle diameter in pixels

           double tmplat, tmplon, dz;
           int32_t x[2], y[2], z[2];
           newgp (misc.abe_share->open_args[0].head.mbr.min_y, misc.abe_share->open_args[0].head.mbr.min_x, 0.0, options.feature_diameter,
                  &tmplat, &tmplon);
           map->map_to_screen (1, &misc.abe_share->open_args[0].head.mbr.min_x, &misc.abe_share->open_args[0].head.mbr.min_y, &dz, &x[0], &y[0], 
                               &z[0]);
           map->map_to_screen (1, &tmplon, &tmplat, &dz, &x[1], &y[1], &z[1]);
           feature_circle_pixels = abs (y[1] - y[0]);
           if (feature_circle_pixels < 10) feature_circle_pixels = 10;
        }
     }
  else if (misc.function == EDIT_FEATURE)
    {
      bool ok;
      QString text = QInputDialog::getText (this, "mosaicView", tr ("Enter text to search for:"), QLineEdit::Normal, QString::null, &ok);
      if (ok && !text.isEmpty ())
        {
          misc.feature_search_string = text;
        }
      else
        {
          misc.feature_search_string = "";
        }
      redrawMap ();
    }
  else
    {
      midMouse (menu_cursor_x, menu_cursor_y, menu_cursor_lon, menu_cursor_lat);
    }
}



void 
mosaicView::slotPopupMenu2 ()
{
  popup_active = NVFalse;

  if (misc.function == ADD_FEATURE || misc.function == EDIT_FEATURE)
    midMouse (menu_cursor_x, menu_cursor_y, menu_cursor_lon, menu_cursor_lat);
}



void 
mosaicView::slotPopupMenu3 ()
{
  popup_active = NVFalse;
}



void 
mosaicView::slotPopupHelp ()
{
  switch (misc.function)
    {
    case ZOOM_IN_AREA:
      QWhatsThis::showText (QCursor::pos ( ), zoomInText, map);
      break;

    case DEFINE_FEATURE_POLY_AREA:
      QWhatsThis::showText (QCursor::pos ( ), defineFeaturePolyAreaText, map);
      break;

    case DELETE_FEATURE:
      QWhatsThis::showText (QCursor::pos ( ), deleteFeatureText, map);
      break;

    case ADD_FEATURE:
      QWhatsThis::showText (QCursor::pos ( ), addFeatureText, map);
      break;

    case EDIT_FEATURE:
      QWhatsThis::showText (QCursor::pos ( ), editFeatureText, map);
      break;
    }
  midMouse (menu_cursor_x, menu_cursor_y, menu_cursor_lon, menu_cursor_lat);
}



//  Signal from the map class.

void 
mosaicView::slotMousePress (QMouseEvent *e, double lon, double lat)
{
  if (e->button () == Qt::LeftButton) leftMouse (e->x (), e->y (), lon, lat);
  if (e->button () == Qt::MidButton) midMouse (e->x (), e->y (), lon, lat);
  if (e->button () == Qt::RightButton) rightMouse (e->x (), e->y (), lon, lat);
}



//  Mouse press signal prior to signals being enabled from the map class.

void 
mosaicView::slotPreliminaryMousePress (QMouseEvent *e)
{
  QPoint pos = QPoint (e->x (), e->y ());

  QWhatsThis::showText (pos, mapText, map);
}



//  Signal from the map class.

void 
mosaicView::slotMouseRelease (QMouseEvent * e, double lon __attribute__ ((unused)), double lat __attribute__ ((unused)))
{
  if (e->button () == Qt::LeftButton) popup_active = NVFalse;
  if (e->button () == Qt::MidButton) popup_active = NVFalse;
  if (e->button () == Qt::RightButton) {};
}



//  Signal from the map class.

void
mosaicView::slotMouseMove (QMouseEvent *e, double lon, double lat)
{
  char                 ltstring[25], lnstring[25], hem;
  QString              string, geo_string, num_string, stddev_string, avg_string, min_string, max_string;
  double               deg, min, sec;
  NV_I32_COORD2        hot = {-1, -1}, ixy, prev_xy = {-1, -1};



  //  Let other ABE programs know which window we're in.

  misc.abe_share->active_window_id = active_window_id;


  //  Get rid of the tracking cursor from slotTrackCursor ().  But only if it already exists, otherwise we
  //  will be creating a new one (which we don't want to do).

  if (mv_tracker >= 0) map->closeMovingPath (&mv_tracker);


  ixy.x = e->x ();
  ixy.y = e->y ();


  //  Track the cursor position for other ABE programs

  misc.abe_share->cursor_position.y = lat;
  misc.abe_share->cursor_position.x = lon;


  if (lat >= misc.displayed_area.min_y && lat <= misc.displayed_area.max_y && 
      lon >= misc.displayed_area.min_x && lon <= misc.displayed_area.max_x)
    {
      strcpy (ltstring, fixpos (lat, &deg, &min, &sec, &hem, POS_LAT, options.position_form));
      strcpy (lnstring, fixpos (lon, &deg, &min, &sec, &hem, POS_LON, options.position_form));

      geo_string = tr ("Lat: %1  Lon: %2").arg (ltstring).arg (lnstring);
    }


  misc.nearest_feature = -1;


  double anc_lat, anc_lon, az, dist, min_dist, dz;
  int32_t xyz_x, xyz_y, xyz_z;
  QBrush b1;
  QColor c1;


  //  Actions based on the active function

  switch (misc.function)
    {
    case ZOOM_IN_AREA:
      if (map->rubberbandRectangleIsActive (rb_rectangle))
        {
          map->dragRubberbandRectangle (rb_rectangle, lon, lat);

          map->getRubberbandRectangleAnchor (rb_rectangle, &anc_lon, &anc_lat);

          invgp (NV_A0, NV_B0, anc_lat, anc_lon, lat, lon, &dist, &az);


          string = tr (" Distance from anchor (m): %L1").arg (dist, 0, 'f', 2);
          statusBar ()->showMessage (string);
        }
      else
        {
          statusBar ()->showMessage (geo_string);
        }
      break;


    case DEFINE_FEATURE_POLY_AREA:
      if (map->rubberbandPolygonIsActive (rb_polygon))
        {
          map->dragRubberbandPolygon (rb_polygon, lon, lat);

          map->getRubberbandPolygonCurrentAnchor (rb_polygon, &anc_lon, &anc_lat);

          invgp (NV_A0, NV_B0, anc_lat, anc_lon, lat, lon, &dist, &az);

 
          string = tr (" Distance from anchor (m): %L1").arg (dist, 0, 'f', 2);
          statusBar ()->showMessage (string);
        }
      else
        {
          statusBar ()->showMessage (geo_string);
        }
      break;


    case EDIT_FEATURE:
    case DELETE_FEATURE:
      min_dist = 999999999.0;
      for (uint32_t i = 0 ; i < misc.bfd_header.number_of_records ; i++)
        {
          if (misc.feature[i].confidence_level)
            {
              if (misc.feature[i].latitude >= misc.displayed_area.min_y && misc.feature[i].latitude <= misc.displayed_area.max_y && 
                  misc.feature[i].longitude >= misc.displayed_area.min_x && misc.feature[i].longitude <= misc.displayed_area.max_x)
                {
                  map->map_to_screen (1, &misc.feature[i].longitude, &misc.feature[i].latitude, &dz, &xyz_x, &xyz_y, &xyz_z);

                  dist = sqrt ((double) ((ixy.y - xyz_y) * (ixy.y - xyz_y)) + 
                               (double) ((ixy.x - xyz_x) * (ixy.x - xyz_x)));

                  if (dist < min_dist)
                    {
                      misc.nearest_feature = i;
                      hot.x = xyz_x;
                      hot.y = xyz_y;
                      min_dist = dist;
                    }
                }
            }
        }

      map->setMovingPath (&mv_marker, marker, hot.x, hot.y, LINE_WIDTH, options.marker_color, b1, NVFalse, Qt::SolidLine);

      statusBar ()->showMessage (geo_string);

      break;


    case ADD_FEATURE:
      hot = ixy;

      map->setMovingCircle (&mv_circle, hot.x, hot.y, feature_circle_pixels, 0.0, 360.0, LINE_WIDTH,
                            options.marker_color, NVFalse, Qt::SolidLine);

      statusBar ()->showMessage (geo_string);

      break;
    }


  //  Set the previous cursor.
             
  prev_xy = ixy;
}



//  Timer - timeout signal.  Very much like an X workproc.  This tracks the cursor in the associated
//  programs (pfmEdit and pfmView) not in this window.  This is active whenever the mouse leaves this
//  window.

void
mosaicView::slotTrackCursor ()
{
  char               ltstring[25], lnstring[25];
  QString            geo_string;
  QString            string;
  static uint8_t     first = NVTrue;
  static NVMAP_DEF   mpdf;


  //  Since this is always a child process of something we want to exit if we see the CHILD_PROCESS_FORCE_EXIT key

  if (misc.abe_share->key == CHILD_PROCESS_FORCE_EXIT) slotQuit ();


  if (misc.drawing) return;


  //  If we received a bunch of resize signals (see slotResize) and we're not currently drawing
  //  the map, we want to make sure we haven't received a resize for about two seconds.  If that's the
  //  case we will redraw.

  if (need_redraw)
    {
      redraw_count++;
      if (redraw_count > 40)
        {
          redrawMap ();
          need_redraw = NVFalse;
          redraw_count = 0;
        }
    }


  //  If we have changed the feature file in another program, we need to load the new GeoTIFF
  //  and redraw.

  if (misc.abe_share->key == MOSAICVIEW_FORCE_RELOAD || misc.abe_share->key == PFM_LAYERS_CHANGED)
    {
      misc.abe_share->key = 0;
      misc.abe_share->modcode = NO_ACTION_REQUIRED; 

      this->setWindowTitle (QString ("mosaicView : ") + QString (misc.abe_share->open_args[0].image_path));

      char retval[512];
      strcpy (retval, get_geotiff (misc.abe_share->open_args[0].image_path, &misc));

      misc.displayed_area = misc.geotiff_area;
      map->resetBounds (misc.geotiff_area);

      redrawMap ();
    }


  //  Zoom to bounds contained in PFM shared memory.

  if (misc.abe_share->key == ac[0])
    {
      misc.abe_share->key = 0;
      misc.abe_share->modcode = NO_ACTION_REQUIRED;

      NV_F64_XYMBR tmp_mbr = misc.abe_share->displayed_area;
      if (tmp_mbr.min_y < misc.geotiff_area.min_y) tmp_mbr.min_y = misc.geotiff_area.min_y;
      if (tmp_mbr.min_x < misc.geotiff_area.min_x) tmp_mbr.min_x = misc.geotiff_area.min_x;
      if (tmp_mbr.max_y > misc.geotiff_area.max_y) tmp_mbr.max_y = misc.geotiff_area.max_y;
      if (tmp_mbr.max_x > misc.geotiff_area.max_x) tmp_mbr.max_x = misc.geotiff_area.max_x;

      discardMovableObjects ();
      map->zoomIn (tmp_mbr);
    }


  //  Zoom in by 10%.

  if (misc.abe_share->key == ac[1])
    {
      misc.abe_share->key = 0;
      misc.abe_share->modcode = NO_ACTION_REQUIRED;

      discardMovableObjects ();
      map->zoomInPercent ();
    }


  //  Zoom out by 10%.

  if (misc.abe_share->key == ac[2])
    {
      misc.abe_share->key = 0;
      misc.abe_share->modcode = NO_ACTION_REQUIRED; 

      map->zoomOutPercent ();
    }


  //  If we have changed the feature file contents in another program, we need to draw the GeoTIFF.

  if (misc.abe_share->key == FEATURE_FILE_MODIFIED)
    {
      //  Try to open the feature file and read the features into memory.

      if (strcmp (misc.abe_share->open_args[0].target_path, "NONE"))
        {
          if (misc.bfd_open) binaryFeatureData_close_file (misc.bfd_handle);
          misc.bfd_open = NVFalse;

          if ((misc.bfd_handle = binaryFeatureData_open_file (misc.abe_share->open_args[0].target_path, &misc.bfd_header, BFDATA_UPDATE)) >= 0)
            {
              if (binaryFeatureData_read_all_short_features (misc.bfd_handle, &misc.feature) < 0)
                {
                  QString msg = QString (binaryFeatureData_strerror ());
                  QMessageBox::warning (this, "mosaicView", tr ("Unable to read feature records\nReason: %1").arg (msg));
                  binaryFeatureData_close_file (misc.bfd_handle);
                }
              else
                {
                  misc.bfd_open = NVTrue;
                }
            }
        }

      redrawMap ();


      //  Wait 2 seconds so that all associated programs will see the modified flag.

#ifdef NVWIN3X
      Sleep (2000);
#else
      sleep (2);
#endif
      misc.abe_share->key = 0;
      misc.abe_share->modcode = NO_ACTION_REQUIRED; 
    }


  //  If this is the first time through, we need to draw the GeoTIFF.

  if (first)
    {
      redrawMap ();
      first = NVFalse;
    }


  double lat = misc.abe_share->cursor_position.y;
  double lon = misc.abe_share->cursor_position.x;

  if (misc.abe_share->active_window_id != active_window_id && lat >= misc.displayed_area.min_y && lat <= misc.displayed_area.max_y && 
      lon >= misc.displayed_area.min_x && lon <= misc.displayed_area.max_x)
    {
      double deg, min, sec;
      char       hem;

      strcpy (ltstring, fixpos (lat, &deg, &min, &sec, &hem, POS_LAT, options.position_form));
      strcpy (lnstring, fixpos (lon, &deg, &min, &sec, &hem, POS_LON, options.position_form));

      geo_string = tr ("Lat: %1  Lon: %2").arg (ltstring).arg (lnstring);

      statusBar ()->showMessage (geo_string);


      QBrush b1;
      map->setMovingPath (&mv_tracker, marker, lon, lat, 2, options.marker_color, b1, NVFalse, Qt::SolidLine);
    }
}



//  Resize signal from the map class.

void
mosaicView::slotResize (QResizeEvent *e __attribute__ ((unused)))
{
  if (misc.drawing) return;


  //  If the windowing system is doing "Display content in resizing windows" we'll get about a million
  //  resize callbacks all stacked up.  What we want to do is use the trackCursor function to wait for
  //  2 seconds of inactivity (i.e. no resizes) and then redraw the map.  We use the redraw_count to 
  //  time it.  I thought this was easier than starting and killing a one-shit timer every time we
  //  came through this section of code.

  need_redraw = NVTrue;
  redraw_count = 0;

  map->setCursor (Qt::WaitCursor);
  qApp->processEvents ();
}



//  Signal from the map class.

void 
mosaicView::slotPreRedraw (NVMAP_DEF map_mapdef)
{
  double             lat[2], lon[2], dz[2];
  int32_t            start_y, end_y, start_x, end_x, z[2];
  static int32_t     x[2], y[2];


  misc.drawing = NVTrue;


  qApp->setOverrideCursor (Qt::WaitCursor);
  qApp->processEvents ();


  mapdef = map_mapdef;


  //  Set the bounds

  if (mapdef.bounds[mapdef.zoom_level].min_y < misc.geotiff_area.min_y)
    {
      start_y = 0;
      lat[0] = misc.geotiff_area.min_y;
    }
  else
    {
      start_y = (int32_t) ((misc.geotiff_area.max_y - mapdef.bounds[mapdef.zoom_level].max_y) / misc.lat_step);
      lat[0] = mapdef.bounds[mapdef.zoom_level].min_y;
    }

  if (mapdef.bounds[mapdef.zoom_level].max_y > misc.geotiff_area.max_y)
    {
      end_y = misc.mosaic_height;
      lat[1] = misc.geotiff_area.max_y;
    }
  else
    {
      end_y = (int32_t) ((misc.geotiff_area.max_y - mapdef.bounds[mapdef.zoom_level].min_y) / misc.lat_step);
      lat[1] = mapdef.bounds[mapdef.zoom_level].max_y;
    }

  if (mapdef.bounds[mapdef.zoom_level].min_x < misc.geotiff_area.min_x)
    {
      start_x = 0;
      lon[0] = misc.geotiff_area.min_x;
    }
  else
    {
      start_x = (int32_t) ((mapdef.bounds[mapdef.zoom_level].min_x - misc.geotiff_area.min_x) / misc.lon_step);
      lon[0] = mapdef.bounds[mapdef.zoom_level].min_x;
    }

  if (mapdef.bounds[mapdef.zoom_level].max_x > misc.geotiff_area.max_x)
    {
      end_x = misc.mosaic_width;
      lon[1] = misc.geotiff_area.max_x;
    }
  else
    {
      end_x = (int32_t) ((mapdef.bounds[mapdef.zoom_level].max_x - misc.geotiff_area.min_x) / misc.lon_step);
      lon[1] = mapdef.bounds[mapdef.zoom_level].max_x;
    }


  if (start_y < 0)
    {
      end_y = (int32_t) ((misc.geotiff_area.max_y - lat[0]) / misc.lat_step);
      start_y = 0;
    }

  if (end_y > misc.mosaic_height)
    {
      end_y = misc.mosaic_height;
      start_y = (int32_t) ((misc.geotiff_area.max_y - lat[1]) / misc.lat_step);
    }


  //  Number of rows and columns of the GeoTIFF to paint.

  int32_t rows = end_y - start_y;
  int32_t cols = end_x - start_x;


  map->map_to_screen (2, lon, lat, dz, x, y, z);


  int32_t wide = x[1] - x[0];
  int32_t high = y[0] - y[1];


  QPixmap sub_image = QPixmap::fromImage (misc.full_res_image->copy (start_x, start_y, cols, rows));
  QPixmap scaled_image = sub_image.scaled (wide, high, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);


  //  Blast the pixmap to the screen

  map->drawPixmap (x[0], y[1], &scaled_image, 0, 0, wide, high, NVTrue);


  //  Force the repaint.

  map->flush ();
}



//  Signal from the map class.

void 
mosaicView::slotPostRedraw (NVMAP_DEF map_mapdef)
{
  map->setCursor (Qt::ArrowCursor);


  if (options.display_feature) overlayFlag (map, &options, &misc);


  //  Let the parent that shelled us know that we're up and running.

  if (misc.abe_share->key == WAIT_FOR_START) misc.abe_share->key = 0;


  misc.displayed_area = map_mapdef.bounds[map_mapdef.zoom_level];


  setFunctionCursor (misc.function);

  setMainButtons (NVTrue);

  qApp->restoreOverrideCursor ();

  misc.drawing = NVFalse;
}



void
mosaicView::keyPressEvent (QKeyEvent *e)
{
  char key[20];
  strcpy (key, e->text ().toLatin1 ());

  if (key[0] == (char) ac[1])
    {
      map->zoomOutPercent ();
    }

  if (key[0] == (char) ac[2])
    {
      discardMovableObjects ();
      map->zoomInPercent ();
    }
}



void
mosaicView::redrawMap ()
{
  discardMovableObjects ();
  map->redrawMap (NVTrue);
}



//  A bunch of slots.

void 
mosaicView::slotQuit ()
{
  void envout (OPTIONS *options, QMainWindow *mainWindow);


  //  Use frame geometry to get the absolute x and y.

  QRect tmp = this->frameGeometry ();
  options.window_x = tmp.x ();
  options.window_y = tmp.y ();


  //  Use geometry to get the width and height.

  tmp = this->geometry ();
  options.width = tmp.width ();
  options.height = tmp.height ();


  envout (&options, this);


  //  Get rid of the shared memory.

  misc.abeShare->detach ();


  exit (0);
}



void
mosaicView::slotDisplayFeature ()
{
  if (bDisplayFeature->isChecked ())
    {
      options.display_feature = NVTrue;
      bDisplayChildren->setEnabled (true);
      bDisplayFeatureInfo->setEnabled (true);
      bDisplayFeaturePoly->setEnabled (true);
      bAddFeature->setEnabled (true);
      bDeleteFeature->setEnabled (true);
      bEditFeature->setEnabled (true);
      overlayFlag (map, &options, &misc);
    }
  else
    {
      options.display_feature = NVFalse;
      bDisplayChildren->setEnabled (false);
      bDisplayFeatureInfo->setEnabled (false);
      bDisplayFeaturePoly->setEnabled (false);
      bAddFeature->setEnabled (false);
      bDeleteFeature->setEnabled (false);
      bEditFeature->setEnabled (false);
      redrawMap ();
    }
}



void
mosaicView::slotDisplayChildren ()
{
  if (bDisplayChildren->isChecked ())
    {
      options.display_children = NVTrue;
      overlayFlag (map, &options, &misc);
    }
  else
    {
      options.display_children = NVFalse;
      redrawMap ();
    }
}



void
mosaicView::slotDisplayFeatureInfo ()
{
  if (bDisplayFeatureInfo->isChecked ())
    {
      options.display_feature_info = NVTrue;
      overlayFlag (map, &options, &misc);
    }
  else
    {
      options.display_feature_info = NVFalse;
      redrawMap ();
    }
}



void
mosaicView::slotDisplayFeaturePoly ()
{
  if (bDisplayFeaturePoly->isChecked ())
    {
      options.display_feature_poly = NVTrue;
      overlayFlag (map, &options, &misc);
    }
  else
    {
      options.display_feature_poly = NVFalse;
      redrawMap ();
    }
}



void 
mosaicView::slotEditFeatureDataChanged ()
{
  //  Try to open the feature file and read the features into memory.

  if (strcmp (misc.abe_share->open_args[0].target_path, "NONE"))
    {
      if (misc.bfd_open) binaryFeatureData_close_file (misc.bfd_handle);
      misc.bfd_open = NVFalse;

      if ((misc.bfd_handle = binaryFeatureData_open_file (misc.abe_share->open_args[0].target_path, &misc.bfd_header, BFDATA_UPDATE)) >= 0)
        {
          if (binaryFeatureData_read_all_short_features (misc.bfd_handle, &misc.feature) < 0)
            {
              QString msg = QString (binaryFeatureData_strerror ());
              QMessageBox::warning (this, "mosaicView", tr ("Unable to read feature records\nReason: %1").arg (msg));
              binaryFeatureData_close_file (misc.bfd_handle);
            }
          else
            {
              misc.bfd_open = NVTrue;
            }
        }
    }


  //  Let the shelling program know that we have modified the feature file.

  misc.abe_share->key = FEATURE_FILE_MODIFIED;


  redrawMap ();
}



void 
mosaicView::slotEditFeatureDefinePolygon ()
{
  map->setToolTip (tr ("Left click to define polygon vertices, double click to define last vertex, middle click to abort"));
  misc.save_function = misc.function;
  misc.function = DEFINE_FEATURE_POLY_AREA;
  setFunctionCursor (misc.function);
}



void 
mosaicView::editFeatureNum (int32_t feature_number)
{
  editFeature_dialog = new editFeature (this, &options, &misc, feature_number);
  connect (editFeature_dialog, SIGNAL (dataChangedSignal ()), this, SLOT (slotEditFeatureDataChanged ()));
  connect (editFeature_dialog, SIGNAL (definePolygonSignal ()), this, SLOT (slotEditFeatureDefinePolygon ()));

  editFeature_dialog->show ();
}



void
mosaicView::slotZoomOut ()
{
  NVMAP_DEF l_mapdef = map->getMapdef ();
  if (l_mapdef.zoom_level)
    {
      discardMovableObjects ();
      map->zoomOut ();

      l_mapdef = map->getMapdef ();
      if (l_mapdef.zoom_level == 0) bZoomOut->setEnabled (false);
    }
}



void
mosaicView::slotEditMode (int id)
{
  QString msc;


  switch (id)
    {
    case ZOOM_IN_AREA:
    case DELETE_FEATURE:
    case EDIT_FEATURE:
      misc.function = id;
      misc.save_function = misc.function;
      break;

    case ADD_FEATURE:

      //  Compute feature circle diameter in pixels

      double tmplat, tmplon, dz;
      int32_t x[2], y[2], z[2];
      newgp (misc.abe_share->open_args[0].head.mbr.min_y, misc.abe_share->open_args[0].head.mbr.min_x, 0.0, options.feature_diameter,
             &tmplat, &tmplon);
      map->map_to_screen (1, &misc.abe_share->open_args[0].head.mbr.min_x, &misc.abe_share->open_args[0].head.mbr.min_y, &dz, &x[0], &y[0], 
                          &z[0]);
      map->map_to_screen (1, &tmplon, &tmplat, &dz, &x[1], &y[1], &z[1]);
      feature_circle_pixels = abs (y[1] - y[0]);
      if (feature_circle_pixels < 10) feature_circle_pixels = 10;

      misc.save_function = misc.function;
      misc.function = id;
      break;
    }
  setFunctionCursor (misc.function);
}



void 
mosaicView::setFunctionCursor (int32_t function)
{
  switch (function)
    {
    case ZOOM_IN_AREA:
      map->setToolTip (tr ("Left click to start rectangle, middle click to abort, right click for menu"));
      map->setCursor (zoomCursor);
      break;
    case DEFINE_FEATURE_POLY_AREA:
      map->setCursor (pencilCursor);
      break;
    case DELETE_FEATURE:
      map->setCursor (deleteFeatureCursor);
      break;
    case EDIT_FEATURE:
      map->setCursor (editFeatureCursor);
      break;
    case ADD_FEATURE:
      map->setCursor (addFeatureCursor);
      break;
    }
}



void
mosaicView::about ()
{
  QMessageBox::about (this, VERSION,
                      tr ("mosaicView - GeoTIFF viewer.\n\nAuthor : Jan C. Depner (jan@pfmabe.software)"));
}


void
mosaicView::slotAcknowledgments ()
{
  QMessageBox::about (this, VERSION, acknowledgmentsText);
}



void
mosaicView::aboutQt ()
{
  QMessageBox::aboutQt (this, VERSION);
}
