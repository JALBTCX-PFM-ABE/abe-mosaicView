
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



#include "prefs.hpp"
#include "prefsHelp.hpp"

prefs::prefs (QWidget * parent, OPTIONS *op, MISC *mi):
  QDialog (parent, (Qt::WindowFlags) Qt::WA_DeleteOnClose)
{
  options = op;
  misc = mi;

  mod_options = *op;
  mod_misc = *mi;

  setModal (true);

  dataChanged = NVFalse;


  setWindowTitle (tr ("mosaicView Preferences"));


  QVBoxLayout *vbox = new QVBoxLayout (this);
  vbox->setMargin (5);
  vbox->setSpacing (5);

  QGroupBox *fbox = new QGroupBox (tr ("Position Format"), this);
  fbox->setWhatsThis (bGrpText);

  QRadioButton *hdms = new QRadioButton (tr ("Hemisphere Degrees Minutes Seconds.decimal"));
  QRadioButton *hdm_ = new QRadioButton (tr ("Hemisphere Degrees Minutes.decimal"));
  QRadioButton *hd__ = new QRadioButton (tr ("Hemisphere Degrees.decimal"));
  QRadioButton *sdms = new QRadioButton (tr ("+/-Degrees Minutes Seconds.decimal"));
  QRadioButton *sdm_ = new QRadioButton (tr ("+/-Degrees Minutes.decimal"));
  QRadioButton *sd__ = new QRadioButton (tr ("+/-Degrees.decimal"));

  bGrp = new QButtonGroup (this);
  bGrp->setExclusive (true);
  connect (bGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotPositionClicked (int)));

  bGrp->addButton (hdms, 0);
  bGrp->addButton (hdm_, 1);
  bGrp->addButton (hd__, 2);
  bGrp->addButton (sdms, 3);
  bGrp->addButton (sdm_, 4);
  bGrp->addButton (sd__, 5);

  QHBoxLayout *fboxSplit = new QHBoxLayout;
  QVBoxLayout *fboxLeft = new QVBoxLayout;
  QVBoxLayout *fboxRight = new QVBoxLayout;
  fboxSplit->addLayout (fboxLeft);
  fboxSplit->addLayout (fboxRight);
  fboxLeft->addWidget (hdms);
  fboxLeft->addWidget (hdm_);
  fboxLeft->addWidget (hd__);
  fboxRight->addWidget (sdms);
  fboxRight->addWidget (sdm_);
  fboxRight->addWidget (sd__);
  fbox->setLayout (fboxSplit);

  vbox->addWidget (fbox, 1);


  QGroupBox *miscBox = new QGroupBox (tr ("Miscellaneous"), this);
  QHBoxLayout *miscBoxLayout = new QHBoxLayout;
  miscBox->setLayout (miscBoxLayout);


  QGroupBox *zoomBox = new QGroupBox (tr ("Zoom percentage"), this);
  QHBoxLayout *zoomBoxLayout = new QHBoxLayout;
  zoomBox->setLayout (zoomBoxLayout);
  zoomPercent = new QSpinBox (zoomBox);
  zoomPercent->setRange (10, 50);
  zoomPercent->setSingleStep (5);
  zoomPercent->setToolTip (tr ("Change the zoom in/out percentage (10-50)"));
  zoomPercent->setWhatsThis (zoomPercentText);
  zoomBoxLayout->addWidget (zoomPercent);
  miscBoxLayout->addWidget (zoomBox);


  QGroupBox *featureBox = new QGroupBox (tr ("Feature Circle Diameter"), this);
  QHBoxLayout *featureBoxLayout = new QHBoxLayout;
  featureBox->setLayout (featureBoxLayout);
  featureDiameter = new QLineEdit (featureBox);
  featureDiameter->setToolTip (tr ("Change the add feature circle diameter (meters)"));
  featureDiameter->setWhatsThis (featureDiameterText);
  featureBoxLayout->addWidget (featureDiameter);
  miscBoxLayout->addWidget (featureBox);


  QGroupBox *textSearchBox = new QGroupBox (tr ("Feature search string"), this);
  QHBoxLayout *textSearchBoxLayout = new QHBoxLayout;
  textSearchBox->setLayout (textSearchBoxLayout);
  textSearch = new QLineEdit (textSearchBox);
  textSearch->setToolTip (tr ("Highlight features based on text search"));
  textSearch->setWhatsThis (textSearchText);
  textSearchBoxLayout->addWidget (textSearch);
  miscBoxLayout->addWidget (textSearchBox);


  vbox->addWidget (miscBox);


  QGroupBox *cbox = new QGroupBox (tr ("Colors"), this);
  QVBoxLayout *cboxLayout = new QVBoxLayout;
  cbox->setLayout (cboxLayout);
  QHBoxLayout *cboxTopLayout = new QHBoxLayout;
  QHBoxLayout *cboxBottomLayout = new QHBoxLayout;
  cboxLayout->addLayout (cboxTopLayout);
  cboxLayout->addLayout (cboxBottomLayout);


  bMarkerColor = new QPushButton (tr ("Marker"), this);
  bMarkerColor->setToolTip (tr ("Change marker color"));
  bMarkerColor->setWhatsThis (markerColorText);
  bMarkerPalette = bMarkerColor->palette ();
  connect (bMarkerColor, SIGNAL (clicked ()), this, SLOT (slotMarkerColor ()));
  cboxTopLayout->addWidget (bMarkerColor);


  bCoastColor = new QPushButton (tr ("Coast"), this);
  bCoastColor->setToolTip (tr ("Change coastline color"));
  bCoastColor->setWhatsThis (coastColorText);
  bCoastPalette = bCoastColor->palette ();
  connect (bCoastColor, SIGNAL (clicked ()), this, SLOT (slotCoastColor ()));
  cboxBottomLayout->addWidget (bCoastColor);


  bRectColor = new QPushButton (tr ("Zoom box"), this);
  bRectColor->setToolTip (tr ("Change zomm rectangle color"));
  bRectColor->setWhatsThis (rectColorText);
  bRectPalette = bRectColor->palette ();
  connect (bRectColor, SIGNAL (clicked ()), this, SLOT (slotRectColor ()));
  cboxBottomLayout->addWidget (bRectColor);


  bFeatureColor = new QPushButton (tr ("Feature"), this);
  bFeatureColor->setToolTip (tr ("Change feature color"));
  bFeatureColor->setWhatsThis (featureColorText);
  bFeaturePalette = bFeatureColor->palette ();
  connect (bFeatureColor, SIGNAL (clicked ()), this, SLOT (slotFeatureColor ()));
  cboxTopLayout->addWidget (bFeatureColor);


  bFeatureInfoColor = new QPushButton (tr ("Feature information"), this);
  bFeatureInfoColor->setToolTip (tr ("Change feature information text color"));
  bFeatureInfoColor->setWhatsThis (featureInfoColorText);
  bFeatureInfoPalette = bFeatureInfoColor->palette ();
  connect (bFeatureInfoColor, SIGNAL (clicked ()), this, SLOT (slotFeatureInfoColor ()));
  cboxBottomLayout->addWidget (bFeatureInfoColor);


  bFeaturePolyColor = new QPushButton (tr ("Feature polygon"), this);
  bFeaturePolyColor->setToolTip (tr ("Change feature polygonal area color"));
  bFeaturePolyColor->setWhatsThis (featurePolyColorText);
  bFeaturePolyPalette = bFeaturePolyColor->palette ();
  connect (bFeaturePolyColor, SIGNAL (clicked ()), this, SLOT (slotFeaturePolyColor ()));
  cboxTopLayout->addWidget (bFeaturePolyColor);


  bHighlightColor = new QPushButton (tr ("Highlight"), this);
  bHighlightColor->setToolTip (tr ("Change feature highlight color"));
  bHighlightColor->setWhatsThis (highlightColorText);
  bHighlightPalette = bHighlightColor->palette ();
  connect (bHighlightColor, SIGNAL (clicked ()), this, SLOT (slotHighlightColor ()));
  cboxBottomLayout->addWidget (bHighlightColor);


  vbox->addWidget (cbox, 1);


  QGroupBox *mBox = new QGroupBox (tr ("Display startup message"), this);
  QHBoxLayout *mBoxLayout = new QHBoxLayout;
  mBox->setLayout (mBoxLayout);
  sMessage = new QCheckBox (mBox);
  sMessage->setToolTip (tr ("Toggle display of startup message when program starts"));
  mBoxLayout->addWidget (sMessage);


  vbox->addWidget (mBox, 1);


  setFields ();


  QHBoxLayout *actions = new QHBoxLayout (0);
  vbox->addLayout (actions);

  QPushButton *bHelp = new QPushButton (this);
  bHelp->setIcon (QIcon (":/icons/contextHelp.png"));
  bHelp->setToolTip (tr ("Enter What's This mode for help"));
  connect (bHelp, SIGNAL (clicked ()), this, SLOT (slotHelp ()));
  actions->addWidget (bHelp);

  actions->addStretch (10);

  bRestoreDefaults = new QPushButton (tr ("Restore Defaults"), this);
  bRestoreDefaults->setToolTip (tr ("Restore all preferences to the default state"));
  bRestoreDefaults->setWhatsThis (restoreDefaultsText);
  connect (bRestoreDefaults, SIGNAL (clicked ()), this, SLOT (slotRestoreDefaults ()));
  actions->addWidget (bRestoreDefaults);

  QPushButton *applyButton = new QPushButton (tr ("OK"), this);
  applyButton->setToolTip (tr ("Accept changes and close dialog"));
  applyButton->setWhatsThis (applyPrefsText);
  connect (applyButton, SIGNAL (clicked ()), this, SLOT (slotApplyPrefs ()));
  actions->addWidget (applyButton);

  QPushButton *closeButton = new QPushButton (tr ("Cancel"), this);
  closeButton->setToolTip (tr ("Discard changes and close dialog"));
  closeButton->setWhatsThis (closePrefsText);
  connect (closeButton, SIGNAL (clicked ()), this, SLOT (slotClosePrefs ()));
  actions->addWidget (closeButton);


  show ();
}



prefs::~prefs ()
{
}



void
prefs::slotRectColor ()
{
  QColor clr;

  clr = QColorDialog::getColor (mod_options.rect_color, this, tr ("mosaicView Rectangle Color"), QColorDialog::ShowAlphaChannel);

  if (clr.isValid ()) mod_options.rect_color = clr;

  setFields ();
}



void
prefs::slotFeatureColor ()
{
  QColor clr;

  clr = QColorDialog::getColor (mod_options.feature_color, this, tr ("mosaicView Feature Color"), QColorDialog::ShowAlphaChannel);

  if (clr.isValid ()) mod_options.feature_color = clr;

  setFields ();
}



void
prefs::slotFeatureInfoColor ()
{
  QColor clr;

  clr = QColorDialog::getColor (mod_options.feature_info_color, this, tr ("mosaicView Feature Information Color"), QColorDialog::ShowAlphaChannel);

  if (clr.isValid ()) mod_options.feature_info_color = clr;

  setFields ();
}



void
prefs::slotFeaturePolyColor ()
{
  QColor clr;

  clr = QColorDialog::getColor (mod_options.feature_poly_color, this, tr ("mosaicView Feature Polygon Color"), QColorDialog::ShowAlphaChannel);

  if (clr.isValid ()) mod_options.feature_poly_color = clr;

  setFields ();
}



void
prefs::slotMarkerColor ()
{
  QColor clr;

  clr = QColorDialog::getColor (mod_options.marker_color, this, tr ("mosaicView Marker Color"), QColorDialog::ShowAlphaChannel);

  if (clr.isValid ()) mod_options.marker_color = clr;

  setFields ();
}



void
prefs::slotCoastColor ()
{
  QColor clr;

  clr = QColorDialog::getColor (mod_options.coast_color, this, tr ("mosaicView Coastline Color"), QColorDialog::ShowAlphaChannel);

  if (clr.isValid ()) mod_options.coast_color = clr;

  setFields ();
}



void
prefs::slotHighlightColor ()
{
  QColor clr;

  clr = QColorDialog::getColor (mod_options.highlight_color, this, tr ("mosaicView Highlight Color"), QColorDialog::ShowAlphaChannel);

  if (clr.isValid ()) mod_options.highlight_color = clr;

  setFields ();
}



void
prefs::slotHelp ()
{
  QWhatsThis::enterWhatsThisMode ();
}



void
prefs::slotRestoreDefaults ()
{
  void set_defaults (MISC *misc, OPTIONS *options, uint8_t restore);


  set_defaults (misc, options, NVTrue);

  mod_options = *options;
  mod_misc = *misc;


  setFields ();


  *options = mod_options;
  *misc = mod_misc;

  emit dataChangedSignal ();

  close ();
}



void
prefs::slotPositionClicked (int id)
{
  mod_options.position_form = id;
}



void 
prefs::setFields ()
{
  QString string;

  bGrp->button (mod_options.position_form)->setChecked (true);


  zoomPercent->setValue (mod_options.zoom_percent);

  string = QString ("%L1").arg (mod_options.feature_diameter, 0, 'f', 2);
  featureDiameter->setText (string);

  textSearch->setText (mod_misc.feature_search_string);


  int32_t hue, sat, val;

  mod_options.coast_color.getHsv (&hue, &sat, &val);
  if (val < 128)
    {
      bCoastPalette.setColor (QPalette::Normal, QPalette::ButtonText, Qt::white);
      bCoastPalette.setColor (QPalette::Inactive, QPalette::ButtonText, Qt::white);
    }
  else
    {
      bCoastPalette.setColor (QPalette::Normal, QPalette::ButtonText, Qt::black);
      bCoastPalette.setColor (QPalette::Inactive, QPalette::ButtonText, Qt::black);
    }
  bCoastPalette.setColor (QPalette::Normal, QPalette::Button, mod_options.coast_color);
  bCoastPalette.setColor (QPalette::Inactive, QPalette::Button, mod_options.coast_color);
  bCoastColor->setPalette (bCoastPalette);


  mod_options.marker_color.getHsv (&hue, &sat, &val);
  if (val < 128)
    {
      bMarkerPalette.setColor (QPalette::Normal, QPalette::ButtonText, Qt::white);
      bMarkerPalette.setColor (QPalette::Inactive, QPalette::ButtonText, Qt::white);
    }
  else
    {
      bMarkerPalette.setColor (QPalette::Normal, QPalette::ButtonText, Qt::black);
      bMarkerPalette.setColor (QPalette::Inactive, QPalette::ButtonText, Qt::black);
    }
  bMarkerPalette.setColor (QPalette::Normal, QPalette::Button, mod_options.marker_color);
  bMarkerPalette.setColor (QPalette::Inactive, QPalette::Button, mod_options.marker_color);
  bMarkerColor->setPalette (bMarkerPalette);


  mod_options.rect_color.getHsv (&hue, &sat, &val);
  if (val < 128)
    {
      bRectPalette.setColor (QPalette::Normal, QPalette::ButtonText, Qt::white);
      bRectPalette.setColor (QPalette::Inactive, QPalette::ButtonText, Qt::white);
    }
  else
    {
      bRectPalette.setColor (QPalette::Normal, QPalette::ButtonText, Qt::black);
      bRectPalette.setColor (QPalette::Inactive, QPalette::ButtonText, Qt::black);
    }
  bRectPalette.setColor (QPalette::Normal, QPalette::Button, mod_options.rect_color);
  bRectPalette.setColor (QPalette::Inactive, QPalette::Button, mod_options.rect_color);
  bRectColor->setPalette (bRectPalette);


  mod_options.feature_color.getHsv (&hue, &sat, &val);
  if (val < 128)
    {
      bFeaturePalette.setColor (QPalette::Normal, QPalette::ButtonText, Qt::white);
      bFeaturePalette.setColor (QPalette::Inactive, QPalette::ButtonText, Qt::white);
    }
  else
    {
      bFeaturePalette.setColor (QPalette::Normal, QPalette::ButtonText, Qt::black);
      bFeaturePalette.setColor (QPalette::Inactive, QPalette::ButtonText, Qt::black);
    }
  bFeaturePalette.setColor (QPalette::Normal, QPalette::Button, mod_options.feature_color);
  bFeaturePalette.setColor (QPalette::Inactive, QPalette::Button, mod_options.feature_color);
  bFeatureColor->setPalette (bFeaturePalette);


  mod_options.feature_info_color.getHsv (&hue, &sat, &val);
  if (val < 128)
    {
      bFeatureInfoPalette.setColor (QPalette::Normal, QPalette::ButtonText, Qt::white);
      bFeatureInfoPalette.setColor (QPalette::Inactive, QPalette::ButtonText, Qt::white);
    }
  else
    {
      bFeatureInfoPalette.setColor (QPalette::Normal, QPalette::ButtonText, Qt::black);
      bFeatureInfoPalette.setColor (QPalette::Inactive, QPalette::ButtonText, Qt::black);
    }
  bFeatureInfoPalette.setColor (QPalette::Normal, QPalette::Button, mod_options.feature_info_color);
  bFeatureInfoPalette.setColor (QPalette::Inactive, QPalette::Button, mod_options.feature_info_color);
  bFeatureInfoColor->setPalette (bFeatureInfoPalette);


  mod_options.feature_poly_color.getHsv (&hue, &sat, &val);
  if (val < 128)
    {
      bFeaturePolyPalette.setColor (QPalette::Normal, QPalette::ButtonText, Qt::white);
      bFeaturePolyPalette.setColor (QPalette::Inactive, QPalette::ButtonText, Qt::white);
    }
  else
    {
      bFeaturePolyPalette.setColor (QPalette::Normal, QPalette::ButtonText, Qt::black);
      bFeaturePolyPalette.setColor (QPalette::Inactive, QPalette::ButtonText, Qt::black);
    }
  bFeaturePolyPalette.setColor (QPalette::Normal, QPalette::Button, mod_options.feature_poly_color);
  bFeaturePolyPalette.setColor (QPalette::Inactive, QPalette::Button, mod_options.feature_poly_color);
  bFeaturePolyColor->setPalette (bFeaturePolyPalette);


  mod_options.highlight_color.getHsv (&hue, &sat, &val);
  if (val < 128)
    {
      bHighlightPalette.setColor (QPalette::Normal, QPalette::ButtonText, Qt::white);
      bHighlightPalette.setColor (QPalette::Inactive, QPalette::ButtonText, Qt::white);
    }
  else
    {
      bHighlightPalette.setColor (QPalette::Normal, QPalette::ButtonText, Qt::black);
      bHighlightPalette.setColor (QPalette::Inactive, QPalette::ButtonText, Qt::black);
    }
  bHighlightPalette.setColor (QPalette::Normal, QPalette::Button, mod_options.highlight_color);
  bHighlightPalette.setColor (QPalette::Inactive, QPalette::Button, mod_options.highlight_color);
  bHighlightColor->setPalette (bHighlightPalette);


  if (mod_options.startup_message)
    {
      sMessage->setCheckState (Qt::Checked);
    }
  else
    {
      sMessage->setCheckState (Qt::Unchecked);
    }
}



void
prefs::slotApplyPrefs ()
{
  if (mod_options.marker_color != options->marker_color) dataChanged = NVTrue;
  if (mod_options.coast_color != options->coast_color) dataChanged = NVTrue;
  if (mod_options.highlight_color != options->highlight_color) dataChanged = NVTrue;
  if (mod_options.feature_color != options->feature_color) dataChanged = NVTrue;
  if (mod_options.feature_info_color != options->feature_info_color) dataChanged = NVTrue;
  if (mod_options.feature_poly_color != options->feature_poly_color) dataChanged = NVTrue;
  if (mod_options.rect_color != options->rect_color) dataChanged = NVTrue;


  //  We don't need to set dataChanged on these since they cause no visible change
  //  in the picture.

  if (sMessage->checkState () == Qt::Checked)
    {
      options->startup_message = NVTrue;
    }
  else
    {
      options->startup_message = NVFalse;
    }

  options->position_form = mod_options.position_form;

  options->zoom_percent = mod_options.zoom_percent;

  float tmp_f;
  QString tmp = featureDiameter->text ();
  int32_t status = sscanf (tmp.toLatin1 (), "%f", &tmp_f);
  if (status == 1) mod_options.feature_diameter = tmp_f;
  if (mod_options.feature_diameter != options->feature_diameter) dataChanged = NVTrue;


  mod_misc.feature_search_string = textSearch->text ();
  if (mod_misc.feature_search_string != misc->feature_search_string) dataChanged = NVTrue;


  setFields ();


  hide ();


  if (dataChanged)
    {
      *options = mod_options;
      *misc = mod_misc;

      emit dataChangedSignal ();
    }

  close ();
}



void
prefs::slotClosePrefs ()
{
  close ();
}
