
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



QString bGrpText = 
  prefs::tr ("Select the format in which you want all geographic positions to be displayed.");

QString markerColorText = 
  prefs::tr ("Click this button to change the color that is used to draw the position marker.");

QString coastColorText = 
  prefs::tr ("Click this button to change the color that is used to plot the coasts.  After "
             "selecting a color the map will be redrawn.");

QString rectColorText = 
  prefs::tr ("Click this button to change the color that is used to plot the zoom rectangle.");

QString highlightColorText = 
  prefs::tr ("Click this button to change the color that is used to highlight the features that match "
             "the text search string.  "
             "After selecting a color the map will be redrawn.");

QString featureColorText = 
  prefs::tr ("Click this button to change the color that is used to plot features.  "
             "After selecting a color the map will be redrawn.");

QString featureInfoColorText = 
  prefs::tr ("Click this button to change the color that is used to write the feature information "
             "next to the featuresplot features.  After selecting a color the map will be redrawn.");

QString featurePolyColorText = 
  prefs::tr ("Click this button to change the color that is used to plot the polygonal areas that "
             "are associated with specific features.  Note that if you set the alpha channel to "
             "less than 255 the area will be plotted as a filled polygon with the semi-transparency "
             "that is set for the alpha channel (0 = transparent, 255 = opaque).  After selecting a "
             "color the map will be redrawn.");

QString zoomPercentText = 
  prefs::tr ("Set the percentage value for zooming the mosaic in or out.");

QString featureDiameterText = 
  prefs::tr ("Set the diameter of the add feature circle.  This value is in meters.");

QString textSearchText = 
  prefs::tr ("Set the feature text search string.  The program will do a case insensitive search of all "
             "displayed feature descriptions and remarks for this text string.  Features that contain the "
             "text string will be highlighted.");

QString restoreDefaultsText = 
  prefs::tr ("Click this button to restore all of the preferences to the original default values.");

QString closePrefsText = 
  prefs::tr ("Click this button to discard all preference changes and close the dialog.");

QString applyPrefsText = 
  prefs::tr ("Click this button to apply all changes and close the dialog.");
