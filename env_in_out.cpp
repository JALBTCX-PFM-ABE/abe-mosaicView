
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


double settings_version = 2.00;


/*!
  These functions store and retrieve the program settings (environment) from a .ini file.  On both Linux and Windows
  the file will be called mosaicView.ini and will be stored in a directory called ABE.config.  On Linux, the
  ABE.config directory will be stored in your $HOME directory.  On Windows, it will be stored in your $USERPROFILE
  folder.  If you make a change to the way a variable is used and you want to force the defaults to be restored just
  change the settings_version to a newer number (I've been using the program version number from version.hpp - which
  you should be updating EVERY time you make a change to the program!).  You don't need to change the
  settings_version though unless you want to force the program to go back to the defaults (which can annoy your
  users).  So, the settings_version won't always match the program version.
*/

void envin (OPTIONS *options, QMainWindow *mainWindow)
{
  double saved_version = 0.0;
  QString string;


  //  Get the INI file name

#ifdef NVWIN3X
  QString ini_file = QString (getenv ("USERPROFILE")) + "/ABE.config/mosaicView.ini";
#else
  QString ini_file = QString (getenv ("HOME")) + "/ABE.config/mosaicView.ini";
#endif

  QSettings settings (ini_file, QSettings::IniFormat);
  settings.beginGroup ("mosaicView");

  saved_version = settings.value (QString ("settings version"), saved_version).toDouble ();


  //  If the settings version has changed we need to leave the values at the new defaults since they may have changed.

  if (settings_version != saved_version) return;


  options->coast = settings.value (QString ("coast flag"), options->coast).toBool ();

  options->position_form = settings.value (QString ("position form"), options->position_form).toInt ();

  options->zoom_percent = settings.value (QString ("zoom percentage"), options->zoom_percent).toInt ();

  int32_t red = settings.value (QString ("marker color/red"), options->marker_color.red ()).toInt ();
  int32_t green = settings.value (QString ("marker color/green"), options->marker_color.green ()).toInt ();
  int32_t blue = settings.value (QString ("marker color/blue"), options->marker_color.blue ()).toInt ();
  int32_t alpha = settings.value (QString ("marker color/alpha"), options->marker_color.alpha ()).toInt ();
  options->marker_color.setRgb (red, green, blue, alpha);


  red = settings.value (QString ("coast color/red"), options->coast_color.red ()).toInt ();
  green = settings.value (QString ("coast color/green"), options->coast_color.green ()).toInt ();
  blue = settings.value (QString ("coast color/blue"), options->coast_color.blue ()).toInt ();
  alpha = settings.value (QString ("coast color/alpha"), options->coast_color.alpha ()).toInt ();
  options->coast_color.setRgb (red, green, blue, alpha);


  red = settings.value (QString ("feature color/red"), options->feature_color.red ()).toInt ();
  green = settings.value (QString ("feature color/green"), options->feature_color.green ()).toInt ();
  blue = settings.value (QString ("feature color/blue"), options->feature_color.blue ()).toInt ();
  alpha = settings.value (QString ("feature color/alpha"), options->feature_color.alpha ()).toInt ();
  options->feature_color.setRgb (red, green, blue, alpha);


  red = settings.value (QString ("feature info color/red"), options->feature_info_color.red ()).toInt ();
  green = settings.value (QString ("feature info color/green"), options->feature_info_color.green ()).toInt ();
  blue = settings.value (QString ("feature info color/blue"), options->feature_info_color.blue ()).toInt ();
  alpha = settings.value (QString ("feature info color/alpha"), options->feature_info_color.alpha ()).toInt ();
  options->feature_info_color.setRgb (red, green, blue, alpha);


  red = settings.value (QString ("feature poly color/red"), options->feature_poly_color.red ()).toInt ();
  green = settings.value (QString ("feature poly color/green"), options->feature_poly_color.green ()).toInt ();
  blue = settings.value (QString ("feature poly color/blue"), options->feature_poly_color.blue ()).toInt ();
  alpha = settings.value (QString ("feature poly color/alpha"), options->feature_poly_color.alpha ()).toInt ();
  options->feature_poly_color.setRgb (red, green, blue, alpha);


  red = settings.value (QString ("highlight color/red"), options->highlight_color.red ()).toInt ();
  green = settings.value (QString ("highlight color/green"), options->highlight_color.green ()).toInt ();
  blue = settings.value (QString ("highlight color/blue"), options->highlight_color.blue ()).toInt ();
  alpha = settings.value (QString ("highlight color/alpha"), options->highlight_color.alpha ()).toInt ();
  options->highlight_color.setRgb (red, green, blue, alpha);


  red = settings.value (QString ("rect color/red"), options->rect_color.red ()).toInt ();
  green = settings.value (QString ("rect color/green"), options->rect_color.green ()).toInt ();
  blue = settings.value (QString ("rect color/blue"), options->rect_color.blue ()).toInt ();
  alpha = settings.value (QString ("rect color/alpha"), options->rect_color.alpha ()).toInt ();
  options->rect_color.setRgb (red, green, blue, alpha);

  

  options->display_feature = settings.value (QString ("view feature positions"), 
                                            options->display_feature).toBool ();

  options->display_children = settings.value (QString ("view sub-feature positions"), 
                                              options->display_children).toBool ();

  options->display_feature_info = settings.value (QString ("view feature info"), 
                                                 options->display_feature_info).toBool ();

  options->display_feature_poly = settings.value (QString ("view feature polygon"), 
                                                 options->display_feature_poly).toBool ();

  options->feature_diameter = (float) settings.value (QString ("feature diameter"), 
                                                          (double) options->feature_diameter).toDouble ();


  options->last_rock_feature_desc = settings.value (QString ("last rock feature descriptor index"), 
                                                   options->last_rock_feature_desc).toInt ();

  options->last_offshore_feature_desc = 
    settings.value (QString ("last offshore feature descriptor index"), 
                    options->last_offshore_feature_desc).toInt ();

  options->last_light_feature_desc = settings.value (QString ("last light feature descriptor index"), 
                                                    options->last_light_feature_desc).toInt ();

  options->last_lidar_feature_desc = settings.value (QString ("last lidar feature descriptor index"), 
                                                    options->last_lidar_feature_desc).toInt ();

  options->last_feature_description = settings.value (QString ("last feature description"), 
                                                      options->last_feature_description).toString ();

  options->last_feature_remarks = settings.value (QString ("last feature remarks"), 
                                                  options->last_feature_remarks).toString ();

  options->feature_diameter = (float) settings.value (QString ("feature diameter"), 
                                                          (double) options->feature_diameter).toDouble ();

  options->screenshot_delay = settings.value (QString ("screenshot delay"), options->screenshot_delay).toInt ();

  options->new_feature = settings.value (QString ("new feature counter"), options->new_feature).toInt ();

  options->startup_message = settings.value (QString ("startup message flag"), options->startup_message).toBool ();

  mainWindow->restoreState (settings.value (QString ("main window state")).toByteArray (), 
                            NINT (settings_version * 100.0));

  mainWindow->restoreGeometry (settings.value (QString ("main window geometry")).toByteArray ());

  settings.endGroup ();


  //  We need to get the font from the global settings since it will be used in (hopefully) all of the ABE map GUI applications.

#ifdef NVWIN3X
  QString ini_file2 = QString (getenv ("USERPROFILE")) + "/ABE.config/" + "globalABE.ini";
#else
  QString ini_file2 = QString (getenv ("HOME")) + "/ABE.config/" + "globalABE.ini";
#endif

  QSettings settings2 (ini_file2, QSettings::IniFormat);
  settings2.beginGroup ("globalABE");


  QString defaultFont = options->font.toString ();
  QString fontString = settings2.value (QString ("ABE map GUI font"), defaultFont).toString ();
  options->font.fromString (fontString);


  settings2.endGroup ();
}


void envout (OPTIONS *options, QMainWindow *mainWindow)
{
  //  Get the INI file name

#ifdef NVWIN3X
  QString ini_file = QString (getenv ("USERPROFILE")) + "/ABE.config/mosaicView.ini";
#else
  QString ini_file = QString (getenv ("HOME")) + "/ABE.config/mosaicView.ini";
#endif

  QSettings settings (ini_file, QSettings::IniFormat);
  settings.beginGroup ("mosaicView");


  settings.setValue (QString ("settings version"), settings_version);


  settings.setValue (QString ("coast flag"), options->coast);

  settings.setValue (QString ("position form"), options->position_form);

  settings.setValue (QString ("zoom percentage"), options->zoom_percent);


  settings.setValue (QString ("marker color/red"), options->marker_color.red ());
  settings.setValue (QString ("marker color/green"), options->marker_color.green ());
  settings.setValue (QString ("marker color/blue"), options->marker_color.blue ());
  settings.setValue (QString ("marker color/alpha"), options->marker_color.alpha ());


  settings.setValue (QString ("coast color/red"), options->coast_color.red ());
  settings.setValue (QString ("coast color/green"), options->coast_color.green ());
  settings.setValue (QString ("coast color/blue"), options->coast_color.blue ());
  settings.setValue (QString ("coast color/alpha"), options->coast_color.alpha ());


  settings.setValue (QString ("rect color/red"), options->rect_color.red ());
  settings.setValue (QString ("rect color/green"), options->rect_color.green ());
  settings.setValue (QString ("rect color/blue"), options->rect_color.blue ());
  settings.setValue (QString ("rect color/alpha"), options->rect_color.alpha ());


  settings.setValue (QString ("feature color/red"), options->feature_color.red ());
  settings.setValue (QString ("feature color/green"), options->feature_color.green ());
  settings.setValue (QString ("feature color/blue"), options->feature_color.blue ());
  settings.setValue (QString ("feature color/alpha"), options->feature_color.alpha ());


  settings.setValue (QString ("feature info color/red"), options->feature_info_color.red ());
  settings.setValue (QString ("feature info color/green"), options->feature_info_color.green ());
  settings.setValue (QString ("feature info color/blue"), options->feature_info_color.blue ());
  settings.setValue (QString ("feature info color/alpha"), options->feature_info_color.alpha ());


  settings.setValue (QString ("feature poly color/red"), options->feature_poly_color.red ());
  settings.setValue (QString ("feature poly color/green"), options->feature_poly_color.green ());
  settings.setValue (QString ("feature poly color/blue"), options->feature_poly_color.blue ());
  settings.setValue (QString ("feature poly color/alpha"), options->feature_poly_color.alpha ());


  settings.setValue (QString ("highlight color/red"), options->highlight_color.red ());
  settings.setValue (QString ("highlight color/green"), options->highlight_color.green ());
  settings.setValue (QString ("highlight color/blue"), options->highlight_color.blue ());
  settings.setValue (QString ("highlight color/alpha"), options->highlight_color.alpha ());


  settings.setValue (QString ("view feature positions"), options->display_feature);
  settings.setValue (QString ("view feature info"), options->display_feature_info);
  settings.setValue (QString ("view feature polygon"), options->display_feature_poly);

  settings.setValue (QString ("feature diameter"), (double) options->feature_diameter);

  settings.setValue (QString ("last rock feature descriptor index"), options->last_rock_feature_desc);

  settings.setValue (QString ("last offshore feature descriptor index"), options->last_offshore_feature_desc);

  settings.setValue (QString ("last light feature descriptor index"), options->last_light_feature_desc);

  settings.setValue (QString ("last lidar feature descriptor index"), options->last_lidar_feature_desc);

  settings.setValue (QString ("feature diameter"), (double) options->feature_diameter);

  settings.setValue (QString ("screenshot delay"), options->screenshot_delay);

  settings.setValue (QString ("new feature counter"), options->new_feature);

  settings.setValue (QString ("startup message flag"), options->startup_message);

  settings.setValue (QString ("main window state"), mainWindow->saveState (NINT (settings_version * 100.0)));

  settings.setValue (QString ("main window geometry"), mainWindow->saveGeometry ());

  settings.endGroup ();
}
