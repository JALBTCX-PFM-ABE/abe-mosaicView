
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



QString prefsText = 
  mosaicView::tr ("<img source=\":/icons/prefs.png\"> Click this button to change program preferences.  Right now "
                  "that's just the position format.  Maybe more later...");

QString quitText = 
  mosaicView::tr ("<img source=\":/icons/quit.png\"> Click this button to <b><em>exit</em></b> from the program.  "
                  "You can also use the <b>Quit</b> entry in the <b>File</b> menu.");

QString mapText = 
  mosaicView::tr ("This is the mosaicView program, a companion to the <b>pfmView</b> and <b>pfmEdit</b> programs.  "
                  "It is used for viewing GeoTIFF mosaics of either CHARTS down-looking digital camera images or "
                  "sidescan images.  The mosaics must be EPSG:4326 form (lat/lon, WGS84) GeoTIFFs.<br><br>"
                  "The box cursor will track the movement of the cursor in the <b>pfmView</b> or <b>pfmEdit</b> window.  "
                  "There are a couple of remote action keys that can be redefined in the <b>Preferences</b> dialog of "
                  "<b>pfmEdit</b>.  The default values for these are:<br>"
                  "<ul>"
                  "<li>z - zoom the mosaic display to match (as much as possible) the <b>pfmView/pfmEdit</b> display</li>"
                  "<li>+ - zoom in by the percentage defined in the <b>mosaicView Preferences</b> dialog (default 10)</li>"
                  "<li>- - zoom out by the percentage defined in the <b>mosaicView Preferences</b> dialog (default 10)</li>"
                  "</ul>"
                  "The options need to be listed in the above order in the <b>Preferences-Ancillary Programs</b> "
                  "dialog of <b>pfmEdit</b>.  For instance - z,+,-");

QString zoomOutText = 
  mosaicView::tr ("<img source=\":/icons/icon_zoomout.png\"> Click this button to zoom out one level.");
QString zoomInText = 
  mosaicView::tr ("<img source=\":/icons/icon_zoomin.png\"> Click this button to define a rectangular zoom area.  "
                  "After clicking the button the cursor will change to the zoomCursor "
                  "<img source=\":/icons/zoom_cursor.png\">.  Click the left mouse button to define a starting point for a "
                  "rectangle.  Move the mouse to define the zoom in bounds.  Left click again to finish the operation.  To "
                  "abort the zoom in operation click the middle mouse button.");

QString displayFeatureText = 
  mosaicView::tr ("<img source=\":/icons/displayfeature.png\"> Click this button to display features.  You can highlight "
                  "features that have descriptions or remarks containing specific text strings by setting "
                  "the feature search string in the preferences dialog <img source=\":/icons/prefs.png\">.");
QString displayChildrenText = 
  mosaicView::tr ("<img source=\":/icons/displayfeaturechildren.png\"> Click this button to display feature sub-records.  "
                  "Feature sub-records are features that have been grouped under a master feature record.  Features "
                  "can be grouped and un-grouped in the edit feature dialog.");
QString displayFeatureInfoText = 
  mosaicView::tr ("<img source=\":/icons/displayfeatureinfo.png\"> Click this button to write the description and "
                  "remarks fields of features next to any displayed features.  This button is "
                  "meaningless unless <b>Flag Feature Data</b> <img source=\":/icons/displayfeature.png\"> is toggled "
                  "on.");
QString displayFeaturePolyText = 
  mosaicView::tr ("<img source=\":/icons/displayfeaturepoly.png\"> Click this button to draw any associated polygonal "
                  "areas with the displayed features.  This button is meaningless unless <b>Flag Feature Data</b> "
                  "<img source=\":/icons/displayfeature.png\"> is toggled on.");

QString defineFeaturePolyAreaText = 
  mosaicView::tr ("Left clicking on a location will cause that point to be the first polygon vertex.  Moving "
                  "the cursor will cause a line to appear.  Select the next vertex by left clicking on a "
                  "location again.  Select as many points as needed.  To select the last point double click "
                  "on its location or right click to bring up the popup menu and select "
                  "<b>Accept polygon</b>.  You can discard the polygon at any time by either clicking the "
                  "center mouse button or selecting <b>Discard polygon</b> from the right-click popup menu.  "
                  "After either accepting or discarding the polygon the feature dialog will reappear.  If you "
                  "have accepted the polygon the length and orientation will be recomputed using the "
                  "long axis of the polygonal area.  If you have discarded the polygon these fields will "
                  "remain unchanged.");

QString addFeatureText = 
  mosaicView::tr ("<img source=\":/icons/addfeature.png\"> Click this button to add a feature.  When selected the cursor will "
                  "become a feature within a circle.  The circle radius is defined in the preferences dialog.  Left clicking "
                  "on the display will add a feature at the shoalest point within the circle.  If the highest point versus "
                  "the surrounding data does not meet the feature criteria then you will be offered a dialog that will allow "
                  "you to force the addition of the feature.<br><br>"
                  "<b>NOTE: This mode will remain active unless one of the other modes "
                  "(like edit feature <img source=\":/icons/editfeature.png\"> or zoomIn "
                  "<img source=\":/icons/icon_zoomin.png\">) is selected.</b>");

QString deleteFeatureText = 
  mosaicView::tr ("<img source=\":/icons/deletefeature.png\"> Click this button to enter delete feature mode.  When selected "
                  "the cursor will become the delete feature cursor <img source=\":/icons/delete_feature_cursor.png\">.  As "
                  "the cursor is moved the box cursor will snap to the nearest feature.  Left clicking will delete the feature "
                  "under the box cursor.  Actually what happens is that a confidence value of zero is assigned to the "
                  "selected feature.  Features with a confidence value of zero are visible when viewing any of the unedited "
                  "surfaces.<br><br>"
                  "<b>NOTE: This mode will remain active unless one of the other modes "
                  "(like add feature <img source=\":/icons/addfeature.png\"> or zoomIn "
                  "<img source=\":/icons/icon_zoomin.png\">) is selected.</b>");

QString editFeatureText = 
  mosaicView::tr ("<img source=\":/icons/editfeature.png\"> Click this button to enter edit feature mode.  When selected the "
                  "cursor will become the edit feature cursor <img source=\":/icons/edit_feature_cursor.png\">.  As the cursor "
                  "is moved the box cursor will snap to the nearest feature.  Left clicking will bring up the edit feature "
                  "dialog.<br><br>"
                  "<b>Cool feature: You can right click and select <i>Highlight features (text search)</i> to highlight "
                  "features where the description or remarks contain the text string (verbatim, case insensitive).  This "
                  "can be handy for finding all of the features on screen that are of a certain type.</b><br><br>" 
                  "<b>NOTE: This mode will remain active unless one of the other modes "
                  "(like add feature <img source=\":/icons/addfeature.png\"> or zoomIn "
                  "<img source=\":/icons/icon_zoomin.png\">) is selected.</b>");

QString coastText = 
  mosaicView::tr ("<img source=\":/icons/coast.png\"> Click this button to toggle the display of coastline.");

QString linkText = 
  mosaicView::tr ("<img source=\":/icons/unlink.png\"> Click this button to choose from other ABE or geographical GUI "
		  "programs to connect to.  At present the only possible applications are pfmView, mosaicView, and "
		  "areaCheck.  Hopefully, in the near future, we will be adding CNILE to the list.  The connection "
		  "allows cursor tracking and limited data exchange between the applications.  If there are no "
		  "available ABE groups to link to you can create a new group and add yourself to it so that other "
		  "applications can link to this one.");

QString unlinkText = 
  mosaicView::tr ("<img source=\":/icons/link.png\"> Click this button to disconnect from another ABE or "
		  "geographical GUI program.  This option is not available unless you have already linked to another "
		  "program.");
